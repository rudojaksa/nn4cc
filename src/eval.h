// eval.h: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// common error evaluation code

# ifndef EVAL_H
# define EVAL_H

extern   void (*ehdr)(const char*,const data_t*,const int);
extern double (*eval)(mlp_t*,const data_t*,const long long,const int);

extern void checkmode(int,char**);

# endif
