#include <stdio.h>
#include <string.h>

#include <unistd.h>

#include <netinet/in.h>
#include <sys/socket.h>

#include <pthread.h>
#include <semaphore.h>

#include "server.h"


struct thread_start_data {
    int fd;
    long *pvalue;
    pthread_mutex_t *value_mutex;
    sem_t *data_sem;
};

struct session {
    int fd;
    long *pvalue;
    pthread_mutex_t *value_mutex;

    char buffer[BUFFER_SIZE];
    int buffer_usage;
};

enum actions {
    action_up, action_down, action_show
};

static int is_space(char c)
{
    return c == ' ' || c == '\t';
}

static char *strip(char *s)
{
    char *end;

    while (is_space(*s)) {
        s++;
    }
    if (!*s) {
        return s;
    }

    end = s + strlen(s) - 1;
    while (end > s && is_space(*end)) {
        end--;
    }
    *(end + 1) = '\0';
    return s;
}

static void session_send_string(const struct session *sess, const char *str)
{
    write(sess->fd, str, strlen(str));
}

static void session_handle(const struct session *sess, const char *line)
{
    char str[128] = { 'O', 'k', '\n', '\0' };
    enum actions action;

    if (0 == strcmp(line, "show")) {
        action = action_show;
    } else if (0 == strcmp(line, "up")) {
        action = action_up;
    } else if (0 == strcmp(line, "down")) {
        action = action_down;
    } else {
        session_send_string(sess, "Unknown command\n");
        return;
    }

    pthread_mutex_lock(sess->value_mutex);
    switch (action) {
    case action_up:
        (*sess->pvalue)++;
        break;
    case action_down:
        (*sess->pvalue)--;
        break;
    case action_show:
        sprintf(str, "%ld\n", *sess->pvalue);
        break;
    }
    pthread_mutex_unlock(sess->value_mutex);
    session_send_string(sess, str);
}

static void session_check_lf(struct session *sess)
{
    char *lf;
    while ((lf = memchr(sess->buffer, '\n', sess->buffer_usage)) != NULL) {
        char *line;
        int pos = lf - sess->buffer;
        if (pos > 0 && sess->buffer[pos - 1] == '\r') {
            sess->buffer[pos - 1] = '\0';
        }
        sess->buffer[pos] = '\0';

        line = strip(sess->buffer);
        session_handle(sess, line);
        sess->buffer_usage -= pos + 1;
        memmove(sess->buffer, lf + 1, sess->buffer_usage);
    }
}

static void *worker(void *v_data)
{
    int rc;
    struct session sess;
    struct thread_start_data *data = v_data;

    sess.fd = data->fd;
    sess.pvalue = data->pvalue;
    sess.value_mutex = data->value_mutex;
    sess.buffer_usage = 0;
    sem_post(data->data_sem);

    while ((rc = read(sess.fd, sess.buffer + sess.buffer_usage,
                      BUFFER_SIZE - sess.buffer_usage)) > 0)
    {
        sess.buffer_usage += rc;
        session_check_lf(&sess);
        if (sess.buffer_usage >= BUFFER_SIZE) {
            session_send_string(&sess, "Line is too long...\n");
            goto session_close;
        }
    }
session_close:
    close(sess.fd);
    return NULL;
}

int server_init(int port)
{
    int lsd, opt, res;
    struct sockaddr_in addr;

    lsd = socket(AF_INET, SOCK_STREAM, 0);
    if (lsd == -1) {
        perror("socket");
        return -1;
    }

    opt = 1;
    setsockopt(lsd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    res = bind(lsd, (struct sockaddr *)&addr, sizeof(addr));
    if (res == -1) {
        perror("bind");
        return -1;
    }

    listen(lsd, LISTEN_QLEN);
    return lsd;
}

int server_run(int lsd)
{
    long value = 0;
    pthread_mutex_t value_mutex = PTHREAD_MUTEX_INITIALIZER;
    struct thread_start_data tsdata;
    sem_t data_sem;

    tsdata.pvalue = &value;
    tsdata.value_mutex = &value_mutex;
    tsdata.data_sem = &data_sem;
    sem_init(&data_sem, 0, 0);

    for (;;) {
        pthread_t th;

        tsdata.fd = accept(lsd, NULL, NULL);
        if (tsdata.fd == -1) {
            perror("accept");
            continue;
        }
        pthread_create(&th, NULL, worker, &tsdata);
        pthread_detach(th);
        sem_wait(&data_sem);
    }
    return 0;
}
