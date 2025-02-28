#include <limits.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>


#define MAX(A, B) ((A) > (B) ? (A) : (B))

#ifndef INIT_SESSION_ARRAY_SIZE
#define INIT_SESSION_ARRAY_SIZE 16
#endif

#ifndef LISTEN_QLEN
#define LISTEN_QLEN 16
#endif

#ifndef CLIENT_BUFFER_SIZE
#define CLIENT_BUFFER_SIZE 2048
#endif

struct session {
    int fd;
    char buffer[CLIENT_BUFFER_SIZE];
    int buffer_usage;
};

struct server {
    int lsd;
    long value;
    struct session **session_array;
    int session_array_size;
};

static const char ok[] = "Ok\n";
static const char unknown[] = "Unknown command\n";

static const char up[] = "up";
static const char down[] = "down";
static const char show[] = "show";

static int is_space(char c)
{
    return c == ' ' || c == '\t';
}

static int find_newline_position(const char *buffer, int size)
{
    int i;

    for (i = 0; i < size; i++) {
        if (buffer[i] == '\n') {
            return i;
        }
    }
    return -1;
}

static int memequals(const char *m1, int len1, const char *m2, int len2)
{
    return len1 == len2 && strncmp(m1, m2, len1) == 0;
}

static struct session *make_new_session(int fd)
{
    struct session *sess = malloc(sizeof(*sess));
    sess->fd = fd;
    sess->buffer_usage = 0;
    return sess;
}

static void session_send_string(const struct session *sess, const char *s)
{
    write(sess->fd, s, strlen(s));
}

static void session_process_request(struct server *serv,
                                    const struct session *sess, int pos)
{
    int len;
    const char *start = sess->buffer,
               *end = sess->buffer + pos;

    for (; start < end && is_space(*start); start++)
        {}
    for (; end > start && is_space(*end); end--)
        {}
    len = end - start + 1;

    if (memequals(start, len, up, sizeof(up) - 1)) {
        serv->value++;
        session_send_string(sess, ok);
    } else if (memequals(start, len, down, sizeof(down) - 1)) {
        serv->value--;
        session_send_string(sess, ok);
    } else if (memequals(start, len, show, sizeof(show) - 1)) {
        char buf[64];
        snprintf(buf, sizeof(buf), "%ld\n", serv->value);
        session_send_string(sess, buf);
    } else {
        session_send_string(sess, unknown);
    }
}

static int server_init(struct server *serv, int port)
{
    int lsd, i, res, opt;
    struct sockaddr_in addr;

    lsd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (lsd == -1) {
        perror("socket");
        return 0;
    }

    opt = 1;
    setsockopt(lsd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    res = bind(lsd, (struct sockaddr *)&addr, sizeof(addr));
    if (res == -1) {
        perror("bind");
        return 0;
    }

    listen(lsd, LISTEN_QLEN);

    serv->lsd = lsd;

    serv->session_array = malloc(sizeof(*serv->session_array) *
                                 INIT_SESSION_ARRAY_SIZE);
    serv->session_array_size = INIT_SESSION_ARRAY_SIZE;
    for (i = 0; i < serv->session_array_size; i++) {
        serv->session_array[i] = NULL;
    }

    serv->value = 0;
    return 1;
}

static void server_accept_client(struct server *serv)
{
    int i, fd;

    fd = accept(serv->lsd, NULL, NULL);
    if (fd == -1) {
        perror("accept");
        return;
    }

    if (fd >= serv->session_array_size) {
        int newlen = serv->session_array_size;
        for (; fd >= newlen; newlen += INIT_SESSION_ARRAY_SIZE)
            {}
        serv->session_array = realloc(serv->session_array,
                                      newlen * sizeof(*serv->session_array));
        for (i = serv->session_array_size; i < newlen; i++) {
            serv->session_array[i] = NULL;
        }
        serv->session_array_size = newlen;
    }

    serv->session_array[fd] = make_new_session(fd);
}

static int server_handle_client(struct server *serv, int fd)
{
    int rc, pos;
    struct session *sess = serv->session_array[fd];

    rc = read(fd, sess->buffer + sess->buffer_usage,
              sizeof(sess->buffer) - sess->buffer_usage);
    if (rc <= 0) {
        return 0;
    }
    sess->buffer_usage += rc;

    while (-1 != (pos = find_newline_position(sess->buffer,
                                              sess->buffer_usage)))
    {
        if (sess->buffer[pos - 1] == '\r') {
            sess->buffer[pos - 1] = ' ';
        }
        session_process_request(serv, sess, pos - 1);
        sess->buffer_usage -= pos + 1;
        memmove(sess->buffer, sess->buffer + pos + 1, sess->buffer_usage);
    }

    if (sess->buffer_usage >= sizeof(sess->buffer)) {
        session_send_string(sess, "The line is too long!\n");
        return 0;
    }
    return 1;
}

static void server_close_client(struct server *serv, int fd)
{
    close(fd);
    free(serv->session_array[fd]);
    serv->session_array[fd] = NULL;
}

static int server_run(struct server *serv)
{
    for (;;) {
        int fd, res;
        fd_set readfds;
        int maxfd = serv->lsd;

        FD_ZERO(&readfds);
        FD_SET(serv->lsd, &readfds);
        for (fd = 0; fd < serv->session_array_size; fd++) {
            if (serv->session_array[fd]) {
                FD_SET(fd, &readfds);
                maxfd = MAX(maxfd, fd);
            }
        }

        res = select(maxfd + 1, &readfds, NULL, NULL, NULL);
        if (res == -1) {
            perror("select");
            return 4;
        }

        if (FD_ISSET(serv->lsd, &readfds)) {
            server_accept_client(serv);
        }

        for (fd = 0; fd < serv->session_array_size; fd++) {
            if (serv->session_array[fd] && FD_ISSET(fd, &readfds)) {
                int is_client = server_handle_client(serv, fd);
                if (!is_client) {
                    server_close_client(serv, fd);
                }
            }
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    int ok, res;
    long port;
    char *endptr;
    struct server serv;

    if (argc != 2) {
        fprintf(stderr, "Expected: %s <port>\n", argv[0]);
        return 1;
    }

    port = strtol(argv[1], &endptr, 10);
    if (!*argv[1] || *endptr || port < 0 || port > USHRT_MAX) {
        fprintf(stderr, "The port is invalid\n");
        return 2;
    }

    ok = server_init(&serv, port);
    if (!ok) {
        return 3;
    }

    res = server_run(&serv);
    return res;
}
