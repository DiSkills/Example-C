#ifndef stackdbl_h
#define stackdbl_h

struct stackdbl {
    unsigned size,
             max_size;
    double *elements;
};

struct stackdbl *stackdbl_init();

void stackdbl_destroy(struct stackdbl **pstack);

int stackdbl_empty(const struct stackdbl *stack);

void stackdbl_push(double data, struct stackdbl *stack);

double stackdbl_pop(struct stackdbl *stack);

#endif
