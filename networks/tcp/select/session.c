#include <stdlib.h>

#include "session.h"

struct session *session_init(int fd)
{
    struct session *sess = malloc(sizeof(*sess));
    sess->fd = fd;
    sess->buffer_usage = 0;
    return sess;
}
