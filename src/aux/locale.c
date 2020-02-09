// locale.c: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// locale related

# include <locale.h>

// set ',' as thousands separator in locale for printf %'d
void thousandssep() {
  setlocale(LC_NUMERIC,"en_US"); }

