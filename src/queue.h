// queue.h: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// queue for multithreaded scheduler

# ifndef QUEUE_H
# define QUEUE_H

# include "CONFIG.h"

extern void seize_files();
extern void relax_files();
extern void seize_stderr();
extern void relax_stderr();

extern int qlen();
extern int qinsert(weights_t* const);

extern void checkqueue(const int,char**);

# endif
