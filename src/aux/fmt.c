// fmt.c: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// formatting support

# include <stdarg.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>

// ------------------------------------------------------------------------------------- DYNAMIC FORMAT

// helper function used to copy va for length measurement
static char *dfmt__(char **buf, int *size, const char* const fmt, va_list va) {

  // measure length of buffer using printf of va copy
  va_list v; va_copy(v,va); 
  int l = vsnprintf(0,0,fmt,v) + 1;
  va_end(v);

  // allocate memory
  if(!(*buf)) *buf = malloc(l*sizeof(char)); 
  if(!(*buf)) return 0;

  // reallocate
  if(l>*size) *buf = realloc(*buf,l*sizeof(char));
  if(!(*buf)) return 0;
  *size = l;

  // print to allocated memory
  vsnprintf(*buf,l,fmt,va);						
  return *buf; }

// dynamic format, allocate/reallocate buf
char *dfmt_(char **buf, int *size, const char* const fmt, ...) {
  va_list va;
  va_start(va,fmt);
  dfmt__(buf,size,fmt,va);
  va_end(va);
  return *buf; }

// -------------------------------------------------------------------------------------- STATIC FORMAT

// static format
char *sfmt_(char *buf, const int size, const char* const fmt, ...) {
  va_list va;
  va_start(va,fmt);
  if(!buf) return 0;
  vsnprintf(buf,size,fmt,va);
  va_end(va);
  return buf; }

// ------------------------------------------------------------------------------------ FLOAT TO STRING

// nice-format floating point numbers, 2nd arg if given is requested precision
// buf can be static 32-chars buffer, or we will allocate it if zero
// f2s(buf,num,prec) or f2s(buf,num)
char *f2s_(char *buf, const float num, ...) {

  // precision requested?
  va_list va; va_start(va,num); int prec = va_arg(va,int); va_end(va);

  // allocate if needed
  if(!buf) buf = malloc(32*sizeof(char));

  // sprintf number
  if(prec == -1) snprintf(buf,32,"%f",num);
  else           snprintf(buf,32,"%.*f",prec,num);

  // decimal point there?
  int dec=0;
  for(int i=0; i<32 && buf[i]; i++) if(buf[i]=='.') { dec=1; break; }

  // remove trailing zeros
  if(dec) {
    int l = strlen(buf)-1;
    for(int i=l; i>0; i--) {
      if(buf[i] == '.') break;
      if(buf[i] >= '1') break;
      if(buf[i] == '0') { buf[i]=0; l--; }}
    if(buf[l] == '.') buf[l]=0; }

  return buf; }

// ----------------------------------------------------------------------------------------------------
