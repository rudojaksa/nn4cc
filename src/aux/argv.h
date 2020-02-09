// argv.h: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// arguments parser

# ifndef ARGV__H
# define ARGV__H

extern   int argo_(const int,char**,const char*);
extern   int argi_(const int,char**,const char*,const int);
extern  int argll_(const int,char**,const char* const,const int);
extern float argf_(const int,char**,const char*,const float);
extern char *args_(const int,char**,const char*,const char*);

# define  argo(sw)       argo_(argc,argv,sw)
# define  argi(sw,dflt)  argi_(argc,argv,sw,dflt)
# define argll(sw,dflt) argll_(argc,argv,sw,dflt)
# define  argf(sw,dflt)  argf_(argc,argv,sw,dflt)
# define  args(sw,dflt)  args_(argc,argv,sw,dflt)

# endif
