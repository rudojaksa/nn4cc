// log.h: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// logging interface

# ifndef LOG_H
# define LOG_H

# include "data.h"

extern sch_t *scheduler(data_t**);
extern void logend_();
# define logend(sch) logend_()

# endif
