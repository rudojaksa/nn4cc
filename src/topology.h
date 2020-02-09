// topology.h: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// topology routines

# ifndef TOPOLOGY_H
# define TOPOLOGY_H

# include "CONFIG.h"

# ifdef MLP5_H
# define XAT_(a,b,c,d,e,f) #a"-"#b"-"#c"-"#d"-"#e"-"#f
# define CAT_(a,b,c,d,e,f) XAT_(a,b,c,d,e,f)
# define XA2_(a,b,B,c,C,d,D,e,E,f,F) #a"-"#b#B"-"#c#C"-"#d#D"-"#e#E"-"#f#F
# define CA2_(a,b,B,c,C,d,D,e,E,f,F) XA2_(a,b,B,c,C,d,D,e,E,f,F)
# define Topology CAT_(Nin,Nh1,Nh2,Nh3,Nh4,Nou)
# define Function CAT_(input,Fh1,Fh2,Fh3,Fh4,Fou)
# define tID CA2_(Nin,Nh1,Fh1,Nh2,Fh2,Nh3,Fh3,Nh4,Fh4,Nou,Fou)
# endif

# ifdef MLP4_H
# define XAT_(a,b,c,d,e) #a"-"#b"-"#c"-"#d"-"#e
# define CAT_(a,b,c,d,e) XAT_(a,b,c,d,e)
# define XA2_(a,b,B,c,C,d,D,e,E) #a"-"#b#B"-"#c#C"-"#d#D"-"#e#E
# define CA2_(a,b,B,c,C,d,D,e,E) XA2_(a,b,B,c,C,d,D,e,E)
# define Topology CAT_(Nin,Nh1,Nh2,Nh3,Nou)
# define Function CAT_(input,Fh1,Fh2,Fh3,Fou)
# define tID CA2_(Nin,Nh1,Fh1,Nh2,Fh2,Nh3,Fh3,Nou,Fou)
# endif

# ifdef MLP3_H
# define XAT_(a,b,c,d) #a"-"#b"-"#c"-"#d
# define CAT_(a,b,c,d) XAT_(a,b,c,d)
# define XA2_(a,b,B,c,C,d,D) #a"-"#b#B"-"#c#C"-"#d#D
# define CA2_(a,b,B,c,C,d,D) XA2_(a,b,B,c,C,d,D)
# define Topology CAT_(Nin,Nh1,Nh2,Nou)
# define Function CAT_(input,Fh1,Fh2,Fou)
# define tID CA2_(Nin,Nh1,Fh1,Nh2,Fh2,Nou,Fou)
# endif

# ifdef MLP2_H
# define XAT_(a,b,c) #a"-"#b"-"#c
# define CAT_(a,b,c) XAT_(a,b,c)
# define XA2_(a,b,B,c,C) #a"-"#b#B"-"#c#C
# define CA2_(a,b,B,c,C) XA2_(a,b,B,c,C)
# define Topology CAT_(Nin,Nh1,Nou)
# define Function CAT_(input,Fh1,Fou)
# define tID CA2_(Nin,Nh1,Fh1,Nou,Fou)
# endif

# ifdef MLP1_H
# define XAT_(a,b) #a"-"#b
# define CAT_(a,b) XAT_(a,b)
# define XA2_(a,b,B) #a"-"#b#B
# define CA2_(a,b,B) XA2_(a,b,B)
# define Topology CAT_(Nin,Nou)
# define Function CAT_(input,Fou)
# define tID CA2_(Nin,Nou,Fou)
# endif

extern void checktopology(int,char**);

# endif

