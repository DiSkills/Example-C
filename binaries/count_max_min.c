#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>


struct data_t {
    int count,
        max,
        min;
};


void process_file(int fd, struct data_t *data)
{
    int number;

    data->count = 0;
    while (read(fd, &number, sizeof(number)) > 0) {
        if (data->count == 0) {
            data->min = number;
            data->max = number;
        } else {
            data->min = data->min > number ? number : data->min;
            data->max = data->max < number ? number : data->max;
        }
        data->count++;
    }
}


int main(int argc, char **argv)
{
    FILE *dest;
    int i,
        res;

    if (argc < 3) {
        fputs("Too few arguments\n", stderr);
        return 1;
    }

    dest = fopen(argv[argc - 1], "w");
    if (!dest) {
        perror(argv[argc - 1]);
        return 2;
    }

    for (i = 1; i < argc - 1; i++) {
        struct data_t data;
        int fd = open(argv[i], O_RDONLY);
        if (fd == -1) {
            perror(argv[i]);
            continue;
        }

        process_file(fd, &data);
        fprintf(dest, "%s %d %d %d\n",
                argv[i], data.count, data.min, data.max);

        close(fd);
    }

    res = fclose(dest);
    if (res == -1) {
        perror(argv[argc - 1]);
        return 3;
    }
    return 0;
}
