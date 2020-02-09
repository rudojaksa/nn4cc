// topology.c: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// topology routines

# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <stdio.h>
# include "CONFIG.h"
# include "VERSION.h"
# include "aux.h"
# include "verbose.h"
# include "sugar.h"
# include "topology.h"

void checktopology(int argc, char *argv[]) {
  char *Newtop = 0;
  Newtop = args("-t",0);

  if(Newtop) {

    // fix the new topology topogram
    while(replace(Newtop,len(Newtop)," ","-")) {} // just to be sure, as we rely on the " "
    if(Verbose) verbose0("");
    xverbose("old topology",Topology);
    xverbose("new topology",Newtop);

    // check recompile script
    fmtbuf(cmd,1024);
    sfmt(cmd,"%s/nntrain-recomp",MKDIR);
    replace(cmd,1024,"^~",getenv("HOME"));
    if(!exists(cmd)) error("nntrain-recomp skript not found: %s",cmd);

    // recompile
    char *quiet;
    if(Quiet) quiet = "-q"; else quiet = "";
    sfmt(cmd,"%s/nntrain-recomp %s %s %s %s",MKDIR,quiet,tID,Newtop,MKDIR);
    xverbose("recompile",CM_"%s"CD_,cmd);
    long t0 = gettime();
    int ret = system(cmd);
    float td = gettime() - t0;
    if(ret == -1) serror("cant run %s",cmd);
    if(ret==33280) exit(130); // hack to propagate ctrl-c

    // rerun
    if(ret==0) {

      // forward the time through -_tt argv (only when -q)
      if(Quiet) {
	for(int i=1; i<argc-1; i++) {
	  if(argv[i][0]==0 && argv[i+1][0]==0) {
	    dynfmt(32);
	    argv[i] = "-_tt";
	    argv[i+1] = dfm("%.6f",td);
	    break; }}}

      // rerun, now -t is removed from argv, and -_tt is forwarded
      newfmt(64);
      execv(fmt("%s/nntrain",MKDIR),argv); }}}

