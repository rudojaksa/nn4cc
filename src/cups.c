// cups.c: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// Connections Updated per Second

# include <stdlib.h>
# include "cups.h"

// compute (online-like) cups
cups_t *getcups(cups_t *cups, const double secs, const int Nweights, const long long Npats) {
  if(secs < 0.000001) cups->secs = 0.000001; else cups->secs = secs;	// min. 1 usec
  cups->cons = Nweights * Npats;					// no. of connections updates
  cups->cups = cups->cons / cups->secs;
  return cups; }

