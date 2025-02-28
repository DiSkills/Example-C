#ifndef SESSION_H
#define SESSION_H

#ifndef INIT_SESSION_BUFFER_SIZE
#define INIT_SESSION_BUFFER_SIZE 512
#endif

struct session {
    int fd;
    char buffer[INIT_SESSION_BUFFER_SIZE];
    int buffer_usage;
};

struct session *session_init(int fd);

#endif
