// cups.h: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// Connections Updated per Second

# ifndef CUPS_H
# define CUPS_H

typedef struct cups_s {
  double secs;
  double cons;
  double cups;
} cups_t;

extern cups_t *getcups(cups_t*,const double,const int,const long long);

# endif
