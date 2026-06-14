#ifndef UTILS_H
# define UTILS_H

#include <pthread.h>

typedef struct s_global
{
    int     burnout;
    int     compile;
    int     debug;
    int     refactor;
    int     compiles_required;
    char    *scheduler;
    int     time;
};

typedef struct s_dongle
{
    char *queue;
    pthread_mutex_t mutex;
}   t_dongle;


#endif
