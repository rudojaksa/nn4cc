// mlp.h: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// topolgy independent multilayer perceptron code

# ifndef MLP_H
# define MLP_H

# include <math.h>	// exp

// for mlp_rndinit
# define TIMESEED -1

// multilayer perceptron
typedef struct mlp_s {	//
  float x[Nx];		// neurons inputs
  float y[Nx];		// neurons activations (outputs)
  float e[Nx];		// neurons error signal (delta)
  float w[Nx][Nx];	// weights (single weights matrix)
  float yd[Nou];	// desired values on output neurons
} mlp_t;		//

// weights serialized
typedef struct weights_s {
  float *w;		// weights
  int nw;		// no of weights
  int in1;		// index of first weight from first input !! deprecate?
  int inn;		// last weight from last input + 1	  !! deprecate?
  long long pat;	// sampling pattern no.
} weights_t;

# define w(i,j)	mlp->w[i][j]
# define x(i)	mlp->x[i]
# define y(i)	mlp->y[i]
# define e(i)	mlp->e[i]
# define yd(i)	mlp->yd[i-OU1]

// -------------------------------------------------------------- ACTIVATION FUNCTIONS
// activation functions and their 1st derivatives

// sigmoid
# define af_sigm(x) (1.0/(1.0+exp((-1.0)*x)))
# define df_sigm(x) (exp(x)/((1.0+exp(x))*(1.0+exp(x))))

// ReLU
# define af_relu(x) (x<0?0:x)
# define df_relu(x) (x<0?0:1)

// leaky ReLU
# define af_lrelu(x) (x<0?0.01*x:x)
# define df_lrelu(x) (x<0?0.01:1)

// function name composition
# define af_name(func) af_##func
# define df_name(func) df_##func

// ----------------------------------------------------------------- BLOCKS OF NEURONS

// block of neurons: for function definitions
# define blk_t(mlp) mlp_t *mlp, int i1, int in, int j1, int jn

// block of neurons: for function calls
# define blk(BLKi,BLKj) BLKi##1,BLKi##n,BLKj##1,BLKj##n

// cycle through the layer: OU or H1 or IN
# define forblk(BLK,i) for(int i=BLK##1; i<=BLK##n; i++)

// single layer forward propagation
# define blk_ff(mlp,BLKi,BLKj,func) { \
  for(int i=BLKi##1; i<=BLKi##n; i++) { \
    x(i) = w(i,0) * y(0); \
    for(int j=BLKj##1; j<=BLKj##n; j++) x(i) += w(i,j) * y(j); \
    y(i) = af_name(func)(x(i)); }}

// single layer LMS (for the output layer)
# define blk_lms(mlp,BLK,func) {  \
  for(int i=BLK##1; i<=BLK##n; i++) \
    e(i) = (yd(i)-y(i)) * df_name(func)(x(i)); }

// single layer backprop
# define blk_bp(mlp,BLKi,BLKh,func) { \
  for(int i=BLKi##1; i<=BLKi##n; i++) { \
    float S = 0.0; \
    for(int h=BLKh##1; h<=BLKh##n; h++) S += e(h) * w(h,i); \
    e(i) = S * df_name(func)(x(i)); }}

// single layer weights update
static inline void blk_update(blk_t(mlp), float gamma) {
  for(int i=i1; i<=in; i++)				// for every output
    w(i,0) += gamma * e(i) * y(0);			// bias (weight) update
  for(int j=j1; j<=jn; j++)				// for every input
    for(int i=i1; i<=in; i++)				// for every output
      w(i,j) += gamma * e(i) * y(j); }			// the weights update

// -----------------------------------------------------------------------------------

extern mlp_t *mlp_rndinit(const float,const float,const int);
extern void loadpat(mlp_t*,const float* const,const int,const long);

# endif
