#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "session.h"

static int is_space(char c)
{
    return c == ' ' || c == '\t';
}

static void session_send_string(const struct session *sess, const char *str)
{
    write(sess->fd, str, strlen(str));
}

static void session_handle(const struct session *sess, char *line)
{
    char *start, *end;

    start = line;
    end = line + strlen(line) - 1;
    while (start < end && is_space(*start)) {
        start++;
    }
    while (end > start && is_space(*end)) {
        end--;
    }

    if (start >= end) {
        return;
    }
    *(end + 1) = '\0';

    if (0 == strcmp(start, "show")) {
        char str[128];
        sprintf(str, "%ld\n", *sess->pvalue);
        session_send_string(sess, str);
        return;
    } else if (0 == strcmp(start, "up")) {
        (*sess->pvalue)++;
    } else if (0 == strcmp(start, "down")) {
        (*sess->pvalue)--;
    } else {
        session_send_string(sess, "Unknown command\n");
        return;
    }
    session_send_string(sess, "Ok\n");
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

        line = strdup(sess->buffer);
        sess->buffer_usage -= pos + 1;
        memmove(sess->buffer, lf + 1, sess->buffer_usage);
        session_handle(sess, line);
        free(line);
    }
}

struct session *session_init(int fd, long *pvalue)
{
    struct session *sess = malloc(sizeof(*sess));
    sess->fd = fd;
    sess->buffer_usage = 0;
    sess->pvalue = pvalue;
    session_send_string(sess, "up/down/show\n");
    return sess;
}

void session_del(struct session *sess)
{
    sess->fd = -1;
    sess->buffer_usage = 0;
    sess->pvalue = NULL;
}

int session_receive(struct session *sess)
{
    int rc = read(sess->fd, sess->buffer + sess->buffer_usage,
            sizeof(sess->buffer) - sess->buffer_usage);
    if (rc == -1) {
        perror("read");
        return 0;
    }
    if (rc == 0) {
        return 0;
    }
    sess->buffer_usage += rc;

    session_check_lf(sess);
    if (sess->buffer_usage >= sizeof(sess->buffer)) {
        session_send_string(sess, "Line is too long...\n");
        return 0;
    }
    return 1;
}
