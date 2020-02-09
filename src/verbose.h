// verbose.h: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// verbose messaging support

# ifndef VERBOSE_H
# define VERBOSE_H
# include "aux/color.h"

// verbose switch-off (TODO)
# ifndef VERBOSE
# define VERBOSE 1
# endif

// extra verbose mode
extern int Verbose;

# define KLEN 15 // verbose length of key/pat-prefix

extern void       error(const char*,...);
extern void      serror(const char*,...);

extern void     journal(const char*,const char*,...);

extern void    xverbose(const char*,const char*,...);
extern void    verbose (const char*,const char*,...);
extern void    verbose1(const char*,const char*,...);
extern void    verbose2(const char*,...);
extern void    verbose0(const char*,...);

extern void       blink(const long long,const int,const char*,...);
extern void     unblink(const char*,...);
extern void lastunblink(const char*,...);
extern void  patunblink(const long long,const char*,...);
extern void cpatunblink(const char*,const long long,const char*,...);
extern void  keyunblink(const char*,const char*,...);

# endif
