// files.h: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// files identification interface

# ifndef FILES_H
# define FILES_H

# define MAXFILES 64	// max number of input files

// files characteristics
typedef struct file_s {
  const char *path;	// filename
  const char *base;	// basename
  int type;		// filetype: DAT/CSV
  int role;		// roletype: TRAIN/TEST1/TEST2
} file_t;

extern file_t **checkfiles(int,char**);

# endif
