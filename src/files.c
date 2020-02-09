// files.c: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// files identification interface

# include <stdlib.h>
# include <stdio.h>
# include "global.h"
# include "aux.h"
# include "verbose.h"
# include "files.h"

// ----------------------------------------------------------------------------------------- CHECKFILES

// cycle i through file[0]..file[n]
#define foreachfile(i) for(int i=0; i<MAXFILES && file[i]; i++)

// return new file instance
static file_t *newfile(const char *path, int role) {
  file_t *file = malloc(sizeof(file_t));
  file->path = sdup(path);
  file->base = basename(path);
  file->role = role;
  file->type = 0;
  return file; }

// return array of filled file_t list of files
file_t **checkfiles(int argc, char *argv[]) {
  file_t **file = calloc(MAXFILES+1,sizeof(void*)); // one more for end-zero
  int role = 0;	// current default role (0=undefined)
  int j = 0;	// file index

  // scan argvs for filenames
  for(int i=1; i<argc; i++) {
    if(argv[i][0] == 0) continue;
    if(sam("-train",argv[i])) { role=TRAIN; argv[i][0]=0; continue; }
    if(sam("-test", argv[i])) { role=TEST1; argv[i][0]=0; continue; }
    if(sam("-test1",argv[i])) { role=TEST1; argv[i][0]=0; continue; }
    if(sam("-test2",argv[i])) { role=TEST2; argv[i][0]=0; continue; }
    if(empty(argv[i])) continue; // not a file or empty file
    file[j++] = newfile(argv[i],role);
    if(j >= MAXFILES) error("too many input data files");
    argv[i][0] = 0; }

  // try default filenames if none provided by arguments
  if(!file[0]) {
    if(!empty("train.dat")) file[j++] = newfile("train.dat",TRAIN);
    if(!empty("test.dat"))  file[j++] = newfile("test.dat" ,TEST1);
    if(!empty("test1.dat")) file[j++] = newfile("test1.dat",TEST1);
    if(!empty("test2.dat")) file[j++] = newfile("test2.dat",TEST2); }

  // identify files roles
  // 1st round: explicite names
  foreachfile(i) {
    if(file[i]->role) continue; // role already known
    if     (bgn("train",file[i]->base)) file[i]->role = TRAIN;
    else if(bgn("test2",file[i]->base)) file[i]->role = TEST2;
    else if(bgn("test", file[i]->base)) file[i]->role = TEST1; }

  // 2nd round: e vs. t files
  // TODO: if not Ntrain,Ntest and all files start on t or e and at least one t, then....

  // 3rd round: everything undecided will be training file
  foreachfile(i) {
    if(file[i]->role) continue; // role already known
    file[i]->role = TRAIN; }

  // worng args
  int ok = 1;
  for(int i=1; i<argc; i++) {
    if(argv[i][0] == 0) continue;
    verbose0(CR_" %*s %s"_CD,KLEN,"wrong arg:",argv[i]);
    ok = 0; }
  if(!ok) { fprintf(stderr,"\n"); exit(1); }

  // no-training-file problem
  int ntrain = 0; foreachfile(i) if(file[i]->role == TRAIN) ntrain++;
  int ntest2 = 0; foreachfile(i) if(file[i]->role == TEST2) ntest2++;
  if(!ntrain) error("missing training file");

  // verbose
  if(Verbose) {
    char s[1024];
    s[0]=0; foreachfile(i) if(file[i]->role==TRAIN) snprintf(s+len(s),1024-len(s),CK_"%s"CD_" ",file[i]->path);
    if(s[0]) xverbose("train",s);
    s[0]=0; foreachfile(i) if(file[i]->role==TEST1) snprintf(s+len(s),1024-len(s),CK_"%s"CD_" ",file[i]->path);
    char *k="test"; if(ntest2) k="test1"; if(s[0]) xverbose(k,s);
    s[0]=0; foreachfile(i) if(file[i]->role==TEST2) snprintf(s+len(s),1024-len(s),CK_"%s"CD_" ",file[i]->path);
    if(s[0]) xverbose("test2",s); }

  return file; }

// ----------------------------------------------------------------------------------------------------
