// mlp5.h: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// 5-layer multilayer perceptron

# ifndef MLP5_H
# define MLP5_H

# define IN1 (1)	// 1st input
# define INn (Nin)	// last (n-th) input

# define H11 (Nin+1)	// 1st layer-1
# define H1n (Nin+Nh1)	// last layer-1

# define H21 (H1n+1)	// 1st layer-2
# define H2n (H1n+Nh2)	// last layer-2

# define H31 (H2n+1)	// 1st layer-3
# define H3n (H2n+Nh3)	// last layer-3

# define H41 (H3n+1)	// 1st layer-4
# define H4n (H3n+Nh4)	// last layer-4

# define OU1 (H4n+1)	// 1st output
# define OUn (H4n+Nou)	// last output

// no. of neurons
# define Nx  (1+Nin+Nh1+Nh2+Nh3+Nh4+Nou)

// no. of weights
# define Nw  ((Nin+1)*Nh1 + (Nh1+1)*Nh2 + (Nh2+1)*Nh3 + (Nh3+1)*Nh4 + (Nh4+1)*Nou)

// default activation functions
# ifndef Fh1
# define Fh1 relu
# endif
# ifndef Fh2
# define Fh2 relu
# endif
# ifndef Fh3
# define Fh3 relu
# endif
# ifndef Fh4
# define Fh4 relu
# endif
# ifndef Fou
# define Fou sigm
# endif

# include "mlp.h"

extern      void  mlp5_ff(mlp_t*);
extern      void  mlp5_bp(mlp_t*, float);
extern weights_t *mlp5_weights(const mlp_t*);
extern      void  mlp5_weights_load(mlp_t*,const weights_t*);

# define fforward(mlp)		   mlp5_ff(mlp);
# define backprop(mlp,gamma)	   mlp5_bp(mlp,gamma);
# define weights(mlp)		   mlp5_weights(mlp);
# define weights_load(mlp,weights) mlp5_weights_load(mlp,weights);

# endif
