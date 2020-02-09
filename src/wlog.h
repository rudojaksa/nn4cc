// wlog.h: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// weights logger code

# ifndef WLOG_H
# define WLOG_H

extern int Wlog;

extern void checkwlog(int,char**);
extern void whdr(const char* const);
extern void wlog(const long long,const float* const);

# endif
