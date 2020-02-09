// data.h: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// blockwise load training/test1/test2 datasets from files

# ifndef DATA_H
# define DATA_H

# include <sys/types.h>	// off_t
# include "global.h"	// enum file
# include "files.h"	// file_t

# define BSIZE  1048576 // block size for read (1M = 2^20 = 1048576)
# define XSIZE 33554432 // initial data size  (32M = 2^25 = 33554432)

// data
typedef struct data_s {
  enum file role;	// roletype: TRAIN/TEST1/TEST2
  const char *name;	// name of role
  file_t **f;		// files
  int Nf;		// no. of files
  int fi;		// current file index
  int fd;		// file descriptor (0 or -1 if not open)
  off_t fo;		// file offset
  char buf[BSIZE+1];	// string buffer for block read from file
  int bo;		// buffer offset
  float *x;		// data itself
  long M;		// allocated size of x
  long N;		// no. of numbers = current length of x
  long r;		// no. of rows
  int c;		// no. of columns
} data_t;

extern data_t **dataload(file_t**,const int);

# endif
