// str.h: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// string functions

# ifndef STR__H
# define STR__H

extern int str_len(const char* const) __attribute__ ((pure));
extern char *str_dup(const char* const) __attribute__ ((malloc));
extern int sam(const char* const, const char* const) __attribute__ ((pure));
extern int bgn(const char* const, const char* const) __attribute__ ((pure));

# define  len(s) str_len(s)
# define sdup(s) str_dup(s)

/*
# include <string.h>
# define  len(s) strlen(s)
# define sdup(s) strdup(s)
*/

extern void rmchar(char*,const char);

# endif
