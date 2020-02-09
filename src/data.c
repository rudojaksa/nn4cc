// data.c: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// blockwise load training/test1/test2 datasets from files

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include "global.h"
# include "aux.h"
# include "files.h"
# include "verbose.h"
# include "data.h"

// ----------------------------------------------------------------------------------------- DAT PARSER
// TODO: parse also CSV

// parse m chars from s
static void dataparse(data_t *data, const int m) {
  char *s = data->buf; // alias
  for(int i=0; i<m; i++) {
	
    // skip white
    while(i<m && (s[i]==' '||s[i]=='\n'||s[i]=='\t')) i++;

    // skip comments
    while(i<m && s[i]=='#') {
      while(i<m && s[i]!='\n') i++; 
      while(i<m && (s[i]==' '||s[i]=='\n'||s[i]=='\t')) i++; }

    // read header and skip comments again
    // TODO: do it before dataload core...

    // no more data below m
    if(i>=m) return;

    // reallocate x for next row
    if(data->N + data->c > data->M) {
      while(data->N + data->c > data->M) data->M *= 2;
      verbose("realloc","%ld floats",data->M);
      data->x = realloc(data->x,sizeof(float) * data->M); }

    // read row = read c numbers
    int N0 = data->N;
    for(int j=0; j<data->c; j++) {
      while(i<m && (s[i]==0||s[i]==' '||s[i]=='\t')) i++;	// eat space and 0
      int i0=i;							// begin of number
      char si=0;						// character after the number
      while(i<m) {						// find end of number
	if(s[i]==' '||s[i]=='\t') { si=s[i]; s[i]=0; break; }	// end of number
	if(s[i]=='\n' && j==data->c-1) { si=s[i]; s[i]=0; break; } // last number in row can be followed by \n
	if(s[i]=='\n') error("missing column %d from %d: %s (row %ld)",j+1,data->c,data->f[data->fi]->path,data->r); // TODO...
	i++; }							// 
      data->x[data->N++] = strtof(s+i0,0);			// decode number
      if(i<m) s[i]=si; } 					// return original character

    // eat the rest of line
    while(i<m && s[i]!='\n') i++;

    // increment rows counter
    if(data->N > N0) { data->r++; }}}				// increment no. of rows

// ----------------------------------------------------------------------------------------- FILES LOAD
// TODO: extend to allow during-the-training partial loading for very long data

// dataload helper, for a block-read of single data-set from multiple files
static void _datafill(data_t *data) {
  newfmt(32);
  while(data->fi < data->Nf) { // go through all files
    verbose1(fmt("%s read",data->name),CG_"%s"CD_" ",data->f[data->fi]->path);

    long t0 = gettime();
    data->fd = open(data->f[data->fi]->path,O_RDONLY);
    if(data->fd == -1) serror("can't open %s",data->f[data->fi]->path);

    // init these on start of every new file
    ssize_t fn = 0; // bytes read from file in current block
    data->bo = 0;   // string offset
    data->fo = 0;   // file offset
    do {

      // read a block
      fn = pread(data->fd, data->buf + data->bo, BSIZE - data->bo, data->fo);
      if(fn == -1) serror("read error %s",data->f[data->fi]->path);

      // identify the last nonfinished line, m>0 is enough and makes m zero on fail
      int m = fn+data->bo; if(fn>0) for(;m>0;m--) if(data->buf[m] == '\n') { m++; break; }

      // parse m chars
      dataparse(data,m);

      // copy the end of block to the start of buffer and set a new offset
      if(m) {
	for(int i=m; i<fn+data->bo; i++) data->buf[i-m] = data->buf[i];
	data->buf[fn+data->bo-m] = 0;
	data->bo = fn+data->bo-m; }
      else {
	data->bo = 0; }

      // go the full next block forward
      data->fo += fn;
    } while(fn > 0); // end when nothing read

    if(data->fd > 0) close(data->fd);
    verbose2(CK_"%.2f sec"CD_,gettime()-t0);
    data->fi++; }}

// ------------------------------------------------------------------------------------------ DATA LOAD

// load three data-sets training/test1/test2 from files into memory
data_t **dataload(file_t **file, const int columns) {
  newfmt(32);

  // identify number of files for every role
  int ntrain = 0; for(int i=0; file[i]; i++) if(file[i]->role == TRAIN) ntrain++;
  int ntest1 = 0; for(int i=0; file[i]; i++) if(file[i]->role == TEST1) ntest1++;
  int ntest2 = 0; for(int i=0; file[i]; i++) if(file[i]->role == TEST2) ntest2++;

  // init data structures for every role
  data_t **data = calloc(3,sizeof(data_t*));
  for(int i=0; i<3; i++) data[i] = malloc(sizeof(data_t));

  // roles and names
  data[0]->role = TRAIN; data[0]->name = "train";
  data[1]->role = TEST1; if(ntest2) data[1]->name = "test1"; else data[1]->name = "test";
  data[2]->role = TEST2; data[2]->name = "test2";

  // files
  data[0]->Nf = ntrain; data[0]->f = calloc(ntrain+1,sizeof(file_t*));
  data[1]->Nf = ntest1; data[1]->f = calloc(ntest1+1,sizeof(file_t*));
  data[2]->Nf = ntest2; data[2]->f = calloc(ntest2+1,sizeof(file_t*));
  for(int i=0,j=0; file[i]; i++) if(file[i]->role == TRAIN) data[0]->f[j++] = file[i]; 
  for(int i=0,j=0; file[i]; i++) if(file[i]->role == TEST1) data[1]->f[j++] = file[i]; 
  for(int i=0,j=0; file[i]; i++) if(file[i]->role == TEST2) data[2]->f[j++] = file[i]; 

  // constants
  for(int i=0; i<3; i++) {
    data[i]->fi = 0;
    data[i]->fd = 0;
    data[i]->buf[0] = 0;
    data[i]->x = malloc(XSIZE * sizeof(float));
    data[i]->M = XSIZE;
    data[i]->N = 0;
    data[i]->r = 0;
    data[i]->c = columns; }

  // fill data itself
  for(int i=0; i<3; i++) _datafill(data[i]);
  for(int i=0; i<3; i++) if(data[i]->Nf) journal(fmt("%s rows",data[i]->name),"%ld",data[i]->r);
  return data; }

// ----------------------------------------------------------------------------------------------------
