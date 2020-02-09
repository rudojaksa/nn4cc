// fs.c: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// filesystem operations

# ifndef FS__H
# define FS__H

# include <stdio.h>

extern float *falloc(const int);

extern FILE *fopenr(const char*);
extern FILE *fopenw(const char*);
extern FILE *fopena(const char*);

extern char *basename(const char*) __attribute__ ((pure));
extern char *fname(char*);
extern int exists(const char*);
extern int empty(const char*);
extern void newdir(const char*);

# endif

