// metric.c: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// formatting support

# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include "fmt.h"

// ----------------------------------------------------------------------------------------------------------------------------

// return numbers as string using metric name
char *f2m(char *buf, const float num) { char z[32];
  if(!buf) buf = malloc(32*sizeof(char)); // allocate if needed
  if     (num > 1e18)	snprintf(buf,32,"%s exa", f2s(z,num/1e18,1));
  else if(num > 1e17)	snprintf(buf,32,"%s peta",f2s(z,num/1e15,0));
  else if(num > 1e15)	snprintf(buf,32,"%s peta",f2s(z,num/1e15,1));
  else if(num > 1e14)	snprintf(buf,32,"%s tera",f2s(z,num/1e12,0));
  else if(num > 1e12)	snprintf(buf,32,"%s tera",f2s(z,num/1e12,1));
  else if(num > 1e11)	snprintf(buf,32,"%s giga",f2s(z,num/1e9,0));
  else if(num > 1e9)	snprintf(buf,32,"%s giga",f2s(z,num/1e9,1));
  else if(num > 1e8)	snprintf(buf,32,"%s mega",f2s(z,num/1e6,0));
  else if(num > 1e6)	snprintf(buf,32,"%s mega",f2s(z,num/1e6,1));
  else if(num > 1e5)	snprintf(buf,32,"%s kilo",f2s(z,num/1e3,0));
  else			snprintf(buf,32,"%s kilo",f2s(z,num/1e3,1));
  return buf; }

// return numbers as string using long-scale name
char *f2ls(char *buf, const float num) { char z[32];
  if(!buf) buf = malloc(32*sizeof(char)); // allocate if needed
  if     (num > 1e18)	snprintf(buf,32,"%s trillion",f2s(z,num/1e18,1));
  else if(num > 1e17)	snprintf(buf,32,"%s billiard",f2s(z,num/1e15,0));
  else if(num > 1e15)	snprintf(buf,32,"%s billiard",f2s(z,num/1e15,1));
  else if(num > 1e14)	snprintf(buf,32,"%s billion", f2s(z,num/1e12,0));
  else if(num > 1e12)	snprintf(buf,32,"%s billion", f2s(z,num/1e12,1));
  else if(num > 1e11)	snprintf(buf,32,"%s milliard",f2s(z,num/1e9,0));
  else if(num > 1e9)	snprintf(buf,32,"%s milliard",f2s(z,num/1e9,1));
  else if(num > 1e8)	snprintf(buf,32,"%s million", f2s(z,num/1e6,0));
  else if(num > 1e6)	snprintf(buf,32,"%s million", f2s(z,num/1e6,1));
  else if(num > 1e5)	snprintf(buf,32,"%s thousand",f2s(z,num/1e3,0));
  else if(num > 1e4)	snprintf(buf,32,"%s thousand",f2s(z,num/1e3,1));
  else if(num > 1e3)	snprintf(buf,32,"%s",	      f2s(z,round(num/100)*100,0)); // 3300 instead of 3276
  else if(num > 1e2)	snprintf(buf,32,"%s",	      f2s(z,num,0));
  else			snprintf(buf,32,"%s",	      f2s(z,num,1));
  return buf; }

// return numbers as string using metric shortcut unit prefix (aka G in 32.2 GB)
char *f2ms(char *buf, const float num, const char *unit) { char z[32];
  if(!buf) buf = malloc(32*sizeof(char)); // allocate if needed
  if     (num > 1e18)	snprintf(buf,32,"%s E%s",f2s(z,num/1e18,2),unit);
  else if(num > 1e17)	snprintf(buf,32,"%s P%s",f2s(z,num/1e15,1),unit);
  else if(num > 1e15)	snprintf(buf,32,"%s P%s",f2s(z,num/1e15,2),unit);
  else if(num > 1e14)	snprintf(buf,32,"%s T%s",f2s(z,num/1e12,1),unit);
  else if(num > 1e12)	snprintf(buf,32,"%s T%s",f2s(z,num/1e12,2),unit);
  else if(num > 1e11)	snprintf(buf,32,"%s G%s",f2s(z,num/1e9,1),unit);
  else if(num > 1e9)	snprintf(buf,32,"%s G%s",f2s(z,num/1e9,2),unit);
  else if(num > 1e8)	snprintf(buf,32,"%s M%s",f2s(z,num/1e6,1),unit);
  else if(num > 1e6)	snprintf(buf,32,"%s M%s",f2s(z,num/1e6,2),unit);
  else if(num > 1e5)	snprintf(buf,32,"%s k%s",f2s(z,num/1e3,1),unit);
  else			snprintf(buf,32,"%s k%s",f2s(z,num/1e3,2),unit);
  return buf; }

// ----------------------------------------------------------------------------------------------------------------------------

// add neutral (but use thousands for Mega) metric prefix to unit, zero to one decimal values
char *metr_(buf,num,sep,unit)
  char *buf;		// can be static 32-chars buffer, or we will allocate it if zero
  const double num;	// numerical value
  char *sep;		// separator, or zero
  char *unit; {		// unit, or zero
  char z[32];		// aux number-formatting buffer

  if(!buf) buf = malloc(32*sizeof(char)); // allocate if needed
  if(!sep) sep = "";
  if(!unit) unit = "";

  if     (num > 1e18)	snprintf(buf,32,"%s%sE%s",f2s(z,num/1e18,1),sep,unit);	// 1,000,000,000,000,000,000 <  1.0E........
  else if(num > 1e17)	snprintf(buf,32,"%s%sP%s",f2s(z,num/1e15,0),sep,unit);	//   100,000,000,000,000,000 <  100P...999P
  else if(num > 1e15)	snprintf(buf,32,"%s%sP%s",f2s(z,num/1e15,1),sep,unit);	//     1,000,000,000,000,000 <  1.0P...99.9P
  else if(num > 1e14)	snprintf(buf,32,"%s%sT%s",f2s(z,num/1e12,0),sep,unit);	//       100,000,000,000,000 <  100T...999T
  else if(num > 1e12)	snprintf(buf,32,"%s%sT%s",f2s(z,num/1e12,1),sep,unit);	//         1,000,000,000,000 <  1.0T...99.9T
  else if(num > 1e11)	snprintf(buf,32,"%s%sG%s",f2s(z,num/1e9 ,0),sep,unit);	//           100,000,000,000 <  100G...999G
  else if(num > 1e10)	snprintf(buf,32,"%s%sG%s",f2s(z,num/1e9 ,1),sep,unit);	//            10,000,000,000 < 10.0G...99.9G
  else if(num > 1e8)	snprintf(buf,32,"%s%sM%s",f2s(z,num/1e6 ,0),sep,unit);	//	 	 100,000,000 <  100M...9999M
  else if(num > 1e6)	snprintf(buf,32,"%s%sM%s",f2s(z,num/1e6 ,1),sep,unit);	//	 	   1,000,000 <  1.0M...99.9M
  else if(num > 1e5)	snprintf(buf,32,"%s%sk%s",f2s(z,num/1e3 ,0),sep,unit);	//		     100,000 <  100k...999k
  else if(num > 1e3)	snprintf(buf,32,"%s%sk%s",f2s(z,num/1e3 ,1),sep,unit);	//		       1,000 <  1.0k...99.9k
  else if(num > 1e2)	snprintf(buf,32,"%s%s%s", f2s(z,num     ,0),sep,unit);	//		         100 <  100....999
  else if(num > 0.1)	snprintf(buf,32,"%s%s%s", f2s(z,num     ,1),sep,unit);	//		         0.1 <  0.1....99.9
  else			snprintf(buf,32,"%s%s%s", f2s(z,num     ,2),sep,unit);	//		        0.01 < 0.01....0.09

  return buf; }

// ----------------------------------------------------------------------------------------------------------------------------

// add mega prefix to unit, decimal values when <1M
char *mega_(buf,num,sep,unit)
  char *buf;		// can be static 32-chars buffer, or we will allocate it if zero
  const double num;	// numerical value
  char *sep;		// separator, or zero
  char *unit; {		// unit, or zero
  char z[32];		// aux number-formatting buffer

  if(!buf) buf = malloc(32*sizeof(char)); // allocate if needed
  if(!sep) sep = "";
  if(!unit) unit = "";

  if     (num > 1e10)	snprintf(buf,32,"%s%sG%s",f2s(z,num/1e9 ,0),sep,unit);	//             10,000,000,000 <   10G........
  else if(num > 1e6)	snprintf(buf,32,"%s%sM%s",f2s(z,num/1e6 ,0),sep,unit);	//	 	    1,000,000 <    1M...9999M
  else if(num > 1e4)	snprintf(buf,32,"%s%sM%s",f2s(z,num/1e6 ,2),sep,unit);	//		       10,000 < 0.01M...0.99M
  else if(num > 1)	snprintf(buf,32,"%s%s%s", f2s(z,num     ,2),sep,unit);	//		            1 <    1....9999
  else if(num > 0.1)	snprintf(buf,32,"%s%s%s", f2s(z,num     ,1),sep,unit);	//		          0.1 <  0.1....0.9
  else			snprintf(buf,32,"%s%s%s", f2s(z,num     ,2),sep,unit);	//		         0.01 < 0.01....0.09

  return buf; }

// add kilo prefix to unit, decimal values only when <1
// TODO: add histeresis: 1k->0.9k 999->1000
char *kilo_(buf,num,sep,unit)
  char *buf;		// can be static 32-chars buffer, or we will allocate it if zero
  const double num;	// numerical value
  char *sep;		// separator, or zero
  char *unit; {		// unit, or zero
  char z[32];		// aux number-formatting buffer

  if(!buf) buf = malloc(32*sizeof(char)); // allocate if needed
  if(!sep) sep = "";
  if(!unit) unit = "";

  if     (num > 1e7)	snprintf(buf,32,"%s%sM%s",f2s(z,num/1e6 ,0),sep,unit);	//	 	   10,000,000 <   10M........
  else if(num > 1e3)	snprintf(buf,32,"%s%sk%s",f2s(z,num/1e3 ,0),sep,unit);	//		        1,000 <    1k...9999k
  else if(num > 1e2)	snprintf(buf,32,"%s%sk%s",f2s(z,num/1e3 ,1),sep,unit);	//		          100 <  0.1k...0.9k
  else if(num > 1)	snprintf(buf,32,"%s%s%s", f2s(z,num     ,0),sep,unit);	//		            1 <    1....99
  else if(num > 0.1)	snprintf(buf,32,"%s%s%s", f2s(z,num     ,1),sep,unit);	//		          0.1 <  0.1....0.9
  else			snprintf(buf,32,"%s%s%s", f2s(z,num     ,2),sep,unit);	//		         0.01 < 0.01....0.09

  return buf; }

// ----------------------------------------------------------------------------------------------------------------------------


// add mega prefix to unit, decimal values when <1M
char *mega_e(buf,num,sep,unit)
  char *buf;		// can be static 32-chars buffer, or we will allocate it if zero
  const double num;	// numerical value
  char *sep;		// separator, or zero
  char *unit; {		// unit, or zero
  char z[32];		// aux number-formatting buffer

  if(!buf) buf = malloc(32*sizeof(char)); // allocate if needed
  if(!sep) sep = "";
  if(!unit) unit = "";

  if     (num > 1e19)	snprintf(buf,32,"%s%sE%s",f2s(z,num/1e18,0),sep,unit);	// 10,000,000,000,000,000,000 <   10E........
  else if(num > 1e16)	snprintf(buf,32,"%s%sP%s",f2s(z,num/1e15,0),sep,unit);	//     10,000,000,000,000,000 <   10P...9999P
  else if(num > 1e13)	snprintf(buf,32,"%s%sT%s",f2s(z,num/1e12,0),sep,unit);	//         10,000,000,000,000 <   10T...9999T
  else if(num > 1e10)	snprintf(buf,32,"%s%sG%s",f2s(z,num/1e9 ,0),sep,unit);	//             10,000,000,000 <   10G...9999G
  else if(num > 1e6)	snprintf(buf,32,"%s%sM%s",f2s(z,num/1e6 ,0),sep,unit);	//	 	    1,000,000 <    1M...9999M
  else if(num > 1e4)	snprintf(buf,32,"%s%sM%s",f2s(z,num/1e6 ,2),sep,unit);	//		       10,000 < 0.01M...0.99M
  else if(num > 1)	snprintf(buf,32,"%s%s%s",f2s(z,num     ,2),sep,unit);	//		            1 <    1....9999
  else if(num > 0.1)	snprintf(buf,32,"%s%s%s",f2s(z,num     ,1),sep,unit);	//		          0.1 <  0.1....0.9
  else			snprintf(buf,32,"%s%s%s",f2s(z,num     ,2),sep,unit);	//		         0.01 < 0.01....0.09

  return buf; }

// add kilo prefix to unit, decimal values only when <1
char *kilo_e(buf,num,sep,unit)
  char *buf;		// can be static 32-chars buffer, or we will allocate it if zero
  const double num;	// numerical value
  char *sep;		// separator, or zero
  char *unit; {		// unit, or zero
  char z[32];		// aux number-formatting buffer

  if(!buf) buf = malloc(32*sizeof(char)); // allocate if needed
  if(!sep) sep = "";
  if(!unit) unit = "";

  if     (num > 1e19)	snprintf(buf,32,"%s%sE%s",f2s(z,num/1e18,0),sep,unit);	// 10,000,000,000,000,000,000 <   10E........
  else if(num > 1e16)	snprintf(buf,32,"%s%sP%s",f2s(z,num/1e15,0),sep,unit);	//     10,000,000,000,000,000 <   10P...9999P
  else if(num > 1e13)	snprintf(buf,32,"%s%sT%s",f2s(z,num/1e12,0),sep,unit);	//         10,000,000,000,000 <   10T...9999T
  else if(num > 1e10)	snprintf(buf,32,"%s%sG%s",f2s(z,num/1e9 ,0),sep,unit);	//             10,000,000,000 <   10G...9999G
  else if(num > 1e7)	snprintf(buf,32,"%s%sM%s",f2s(z,num/1e6 ,0),sep,unit);	//	 	   10,000,000 <   10M...9999M
  else if(num > 1e3)	snprintf(buf,32,"%s%sk%s",f2s(z,num/1e3 ,0),sep,unit);	//		        1,000 <    1k...9999k
  else if(num > 1e1)	snprintf(buf,32,"%s%sk%s",f2s(z,num/1e3 ,2),sep,unit);	//		           10 < 0.01k...0.99k
  else if(num > 1)	snprintf(buf,32,"%s%s%s",f2s(z,num     ,0),sep,unit);	//		            1 <    1....9
  else if(num > 0.1)	snprintf(buf,32,"%s%s%s",f2s(z,num     ,1),sep,unit);	//		          0.1 <  0.1....0.9
  else			snprintf(buf,32,"%s%s%s",f2s(z,num     ,2),sep,unit);	//		         0.01 < 0.01....0.09

  return buf; }

// ----------------------------------------------------------------------------------------------------------------------------
