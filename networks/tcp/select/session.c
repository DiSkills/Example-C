#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "session.h"

static void session_send_string(const struct session *sess, const char *str)
{
    write(sess->fd, str, strlen(str));
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

    /* TODO: check */
    if (sess->buffer_usage >= sizeof(sess->buffer)) {
        session_send_string(sess, "Line is too long...\n");
        return 0;
    }
    return 1;
}
