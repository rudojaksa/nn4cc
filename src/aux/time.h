// time.h: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// date and time

# ifndef TIME__H
# define TIME__H

# include <time.h>

extern double gettime();
extern double thclock();
extern double thidclock(const clockid_t);

extern char *dt2s(char*,const double);
extern char *td2s(char*,const double);

# endif
