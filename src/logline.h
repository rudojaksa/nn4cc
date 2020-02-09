// logline.h: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// write log-line to file

# ifndef LOGLINE_H
# define LOGLINE_H

# include <stdio.h>
# include <stdlib.h>

//  line: array of floats to be logged
// Nline: number of already added numbers
// Mline: how much space is available for numbers (can be made changeable in the dynamic mode)
// Xline: if 1 additional space will be printed on this possition

# define  newlogline(line,length) float line[length]; int N##line=0; const int M##line=length; int X##line[length]={0};
# define  dynlogline(line,length) float *line = malloc(length*sizeof(float)); int N##line=0; const int M##line=length; int *X##line=calloc(length,sizeof(float));
# define freelogline(line) free(line); free(X##line);

extern void add_(float*,int*,const int,int*,const int,const int, ...);

// simply add next numbers
# define add1(l,f1)		add_(l,&N##l,M##l,X##l,0,1,f1)
# define add2(l,f1,f2)		add_(l,&N##l,M##l,X##l,0,2,f1,f2)
# define add3(l,f1,f2,f3)	add_(l,&N##l,M##l,X##l,0,3,f1,f2,f3)
# define add4(l,f1,f2,f3,f4)	add_(l,&N##l,M##l,X##l,0,4,f1,f2,f3,f4)
# define add5(l,f1,f2,f3,f4,f5)	add_(l,&N##l,M##l,X##l,0,5,f1,f2,f3,f4,f5)

// start new line from begin
# define bgn1(l,f1)		add_(l,&N##l,M##l,X##l,1,1,f1)
# define bgn2(l,f1,f2)		add_(l,&N##l,M##l,X##l,1,2,f1,f2)
# define bgn3(l,f1,f2,f3)	add_(l,&N##l,M##l,X##l,1,3,f1,f2,f3)
# define bgn4(l,f1,f2,f3,f4)	add_(l,&N##l,M##l,X##l,1,4,f1,f2,f3,f4)
# define bgn5(l,f1,f2,f3,f4,f5)	add_(l,&N##l,M##l,X##l,1,5,f1,f2,f3,f4,f5)

// add space
# define adds(l) X##l[N##l]=1;

extern void writehead(const char*,const char*);
extern void fplogline_(const float*,const int,const int,int*,const long long,FILE*);
extern void   logline_(const float*,const int,const int,int*,const long long,const char*);

# define fplogline(fp,pat,l) fplogline_(l,N##l,M##l,X##l,pat,fp)
# define logline(path,pat,l) logline_(l,N##l,M##l,X##l,pat,path)

# endif
