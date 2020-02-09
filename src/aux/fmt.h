// fmt.h: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// formatting support

# ifndef FMT__H
# define FMT__H

// static format, rewrites global static buffer
extern char *sfmt_(char*,const int,const char* const,...);
# define newfmt(size) int M_f=size; char _f[size];
# define fmt(format,...) sfmt_(_f,M_f,format,##__VA_ARGS__)

// static format, explicit independent static buffers
# define fmtbuf(buf,size) int M_##buf=size; char buf[size];
# define  fmtsz(buf,size) int M_##buf=size;
# define sfmt(buf,format,...) sfmt_(buf,M_##buf,format,##__VA_ARGS__)

// dynamic format, rewrites buffer too, but allocate/reallocate it when needed
extern char *dfmt_(char**,int*,const char* const,...);
# define dynfmt(size) int M_f=size; char *_f=malloc(M_f*sizeof(char));
# define freefmt() free(_f);
# define dfm(format,...) dfmt_(&_f,&M_f,format,##__VA_ARGS__)



extern char *f2s_(char*,const float,...);
# define f2s(buf,num,...) f2s_(buf,num,##__VA_ARGS__,-1)

extern char  *f2m(char*,const float);
extern char *f2ls(char*,const float);
extern char *f2ms(char*,const float,const char*);

extern char *metr_(char*,const double,char*,char*);
extern char *mega_(char*,const double,char*,char*);
extern char *kilo_(char*,const double,char*,char*);

# define metr(buf,num,unit) metr_(buf,num," ",unit)
# define mega(buf,num,unit) mega_(buf,num,"_",unit)
# define kilo(buf,num,unit) kilo_(buf,num,"_",unit)

# endif
