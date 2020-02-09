// sugar.h: part of nn4cc, (c) R.Jaksa 2019, GPLv3 

# ifndef SUGAR_H
# define SUGAR_H

# define LOGDIR "Log"	// subdir for journal and logs (short to save path-space)
# define PSIZE 1024	// paths size

extern char Logdir[];
extern char Journal[];
extern int Quiet;
extern int Noprogr;
extern int Outputs;

extern char *Name;
extern void checkname(int,char**);

extern float Epochs;
extern float Seconds;
extern long long Pats;
extern void checkepochs(int,char**);

extern float Gamma;
extern void checkgamma(int,char**);

extern void checkargs(int,char**);

# endif
