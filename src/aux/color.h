// color.h: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// terminal colors support

# ifndef COLOR__H
# define COLOR__H

// 0 BLACK
// 1 RED
// 2 GREEN
// 3 YELLOW
// 4 BLUE
// 5 MAGENTA
// 6 CYAN
// 7 WHITE

#define Esc "\033"
#define CK_ Esc"[90m"
#define CR_ Esc"[31m"
#define CG_ Esc"[32m"
#define CY_ Esc"[93m"
#define CB_ Esc"[94m"
#define CM_ Esc"[35m"
#define CC_ Esc"[36m"
#define CW_ Esc"[37m"
#define CD_ Esc"[0m"
#define _CD Esc"[0m"

#define CKY_ Esc"[43;30m"
#define CKC_ Esc"[46;30m"
#define CWC_ Esc"[46;37m"

extern void rmesc(char*);
extern int esclen(const char*);
extern int esc1len(const char*);

#endif
