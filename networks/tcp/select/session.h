#ifndef SESSION_H
#define SESSION_H

#ifndef INIT_SESSION_BUFFER_SIZE
#define INIT_SESSION_BUFFER_SIZE 512
#endif

struct session {
    int fd;
    char buffer[INIT_SESSION_BUFFER_SIZE];
    int buffer_usage;
    long *pvalue;
};

struct session *session_init(int fd, long *pvalue);
void session_del(struct session *sess);
int session_receive(struct session *sess);

#endif
