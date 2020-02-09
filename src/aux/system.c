// system.c: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// system calls

# include <unistd.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <stdio.h>

// execute command, return stdout output, close stderr & ignore retvalue
char *syst(const char *cmd) {

  int pip[2]; pipe(pip);
  if(!fork()) {
    close(1); close(2); dup2(pip[1],1);
    system(cmd);
    exit(0); }
  close(pip[1]);

  char s[65]; int l=1;		// read buffer
  char *buf=0; int M=0,N=0;	// output buffer
  while(1) {
    l = read(pip[0],&s,64);
    if(l==0) break;
    if(l<0) { exit(0); }
    if(N+l>=M) { M=N+l+1024; buf=realloc(buf,sizeof(char)*M); }
    snprintf(buf+N,M-N,"%s",s);
    N+=l; }

  wait(0);
  close(pip[0]);
  if(N>=M) { buf=realloc(buf,sizeof(char)*(N+1)); buf[N]=0; }
  return buf; }

