// sugar.c: part of nn4cc, (c) R.Jaksa 2019, GPLv3 

# include <stdlib.h>
# include <stdio.h>
# include "VERSION.h"
# include "global.h"
# include "aux.h"
# include "verbose.h"
# include "files.h"
# include "topology.h"
# include "data.h"
# include "sched.h"
# include "queue.h"
# include "eval.h"
# include "wlog.h"
# include "credit.h"
# include "sugar.h"

// ------------------------------------------------------------------------------------ EXPERIMENT NAME

char *Name = 0;
char checkname_date[32];
void checkname(int argc, char *argv[]) {
  dt2s(checkname_date,0);

  // get from argv
  Name = args("-n",0);

  // time based (default)
  if(!Name) Name = checkname_date;

  // %d %t parser
  else {
    while(replace(Name,1024,"%d",checkname_date)) {}
    while(replace(Name,1024,"%t",Topology)) {}}}

// --------------------------------------------------------------------------------------------- LOGDIR
char Logdir[PSIZE];

static void mklogdir() {
  const char *div = "";					// divider
  fmtbuf(s,PSIZE);					// aux buffer
  fmtsz(Logdir,PSIZE);					// result Logdir
  newfmt(PSIZE);					// for system()

  // beautify the logdir name name
  fmtbuf(base,512);					// base-path
  sfmt(base,"%.256s/%.256s",LOGDIR,Name);		// LOGDIR/Name
  rmchar(s,' ');					// remove spaces
  
  // 1. check "name.01", TODO: check all name.XY files using system(ls)
  sfmt(s,"%.512s%s%03d",base,div,1);
  if(exists(s)) for(int i=2;i<100;i++) {		// "name.001" found, so check "name.002" etc.
    sfmt(Logdir,"%.512s%s%03d",base,div,i);
    if(!exists(Logdir)) goto MKLOGDIR; }

  // 2. no "name.01" then, check the "name"
  sfmt(Logdir,"%s",base);
  if(!exists(Logdir)) goto MKLOGDIR;			// neither "name.001", nor "name" exist, go for "name"

  // 3. "name" exists, but "name.001" not, so rename and go for "name.002"
  sfmt(s,"mv '%.500s' '%.500s%s%03d'",base,base,div,1);
  verbose("rename",CM_"%s"CD_,s);
  system(s);
  sfmt(Logdir,"%.512s%s%03d",base,div,2);

 MKLOGDIR:
  xverbose("logdir",CG_"%s"CD_,Logdir);
  newdir(Logdir);

  // create links
  if(!sam(Logdir,base)) {
    xverbose("link",CM_"ln -sf %s %s"CD_,fname(Logdir),base);
    system(fmt("rm -f '%s'; ln -sf '%s' '%s'",base,fname(Logdir),base)); }
  xverbose("link",CM_"ln -sf %s %s/%s"CD_,fname(Logdir),LOGDIR,"last");
  system(fmt("rm -f '%s/%s'; ln -sf '%s' '%s/%s'",LOGDIR,"last",fname(Logdir),LOGDIR,"last")); }

// -------------------------------------------------------------------------------------- LEARNING RATE

float Gamma = 0.08;
void checkgamma(int argc, char *argv[]) {
  char z[32];
  Gamma = argf("-g",Gamma);
  verbose("gamma",f2s(z,Gamma));
  journal("gamma",f2s(z,Gamma)); }

// ------------------------------------------------------------------------------------------------ ALL
char Journal[PSIZE];
float Epochs = -1;
float Seconds = -1;
long long Pats = -1;
int Quiet = 0;
int Outputs = 0;
int Noprogr = 0;

void checkargs(int argc, char *argv[]) {
  char z[32];

  // help
  if(argo("-h")) {
    char *h = sdup(Help);
    while(replace(h,len(h)+1024,"__TOPOLOGY__",Topology)) {}
    printf("%s",h);
    exit(0); }

  /// debug
  /// for(int i=1; i<argc; i++) { fprintf(stderr,"arg%d: %s\n",i,argv[i]); }

  // other
  if(argo("-q"))    Quiet = 1;
  if(argo("-v"))  Verbose = 1;
  if(argo("-np")) Noprogr = 1;
  if(argo("-O"))  Outputs = 1;

  // topology + recompile
  checktopology(argc,argv);

  // start verbose messaging (only after recompile)
  verbose0("");

  // allow logdir by identifiiing the name
  checkname(argc,argv);
  
  // allow journal by making the logdir
  mklogdir();

  // recompile report
  float tt = -1;
  tt = argf("-_tt",tt);
  if(tt > 0) {
    verbose("recompiled",CG_ "%s/gcc.log" CK_ " %.2f sec" CD_,Logdir,tt); }

  // journal
  fmtsz(Journal,PSIZE);
  sfmt(Journal,"%.512s/journal.log",Logdir);
  FILE *fp = fopenw(Journal);
  fclose(fp);
  verbose("journal",CG_"%s"CD_,Journal);
  journal("name",Name);
  journal("logdir",Logdir);

  // topology report
  verbose("topology",Topology " " CK_ Function CD_);
  journal("topogram",tID);
  journal("topology",Topology);
  journal("functions",Function);
  journal("inputs","%d",Nin);
  journal("outputs","%d",Nou);
  journal("weights","%d",Nw);
  // here: activation-functions (topology-variant?) ???

  // epochs / seconds
  Epochs = argf("-e",Epochs);
  Seconds = argf("-secs",Seconds);
  Pats = argll("-pats",Pats);
  if(Epochs>0)  xverbose("epochs","%s",f2s(z,Epochs));
  if(Seconds>0) xverbose("seconds","%s",f2s(z,Seconds));
  if(Pats>0)    xverbose("pats","%lld",Pats);
  if(Epochs>0)  journal("req epochs","%s",f2s(z,Epochs));
  if(Seconds>0) journal("req time","%s sec",f2s(z,Seconds));
  if(Pats>0)    journal("req pats","%lld",Pats);

  // from modules
  checkgamma(argc,argv);
  checkmode(argc,argv);
  checkwlog(argc,argv);
  checkcredit(argc,argv);
  checkqueue(argc,argv);
  checksched(argc,argv);

  // Log/tmp backup
  // TODO: use LOGDIR instead of direct "Log/"
  newfmt(128);
  if(exists("Log/tmp")) {
    char *s = syst("ls Log/tmp | sed 's:^:Log/tmp/:' | xargs echo -n");
    if(s && s[0]) {
      xverbose("Log/tmp backup",CG_ "%s" CK_ " <- %s" CD_,Logdir,s);
      system(fmt("mv Log/tmp/* %s",Logdir)); }
    system("rmdir Log/tmp"); }

  // CONFIG.h backup
  xverbose("CONFIG.h backup",CG_ "%s/CONFIG.h" CK_ " <- %s/CONFIG.h" CD_,Logdir,MKDIR);
  system(fmt("cp -a %s/CONFIG.h %s",MKDIR,Logdir));

 }

// ----------------------------------------------------------------------------------------------------
