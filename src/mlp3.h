// mlp3.h: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// 3-layer multilayer perceptron

# ifndef MLP3_H
# define MLP3_H

# define IN1 (1)	// 1st input
# define INn (Nin)	// last (n-th) input

# define H11 (Nin+1)	// 1st layer-1
# define H1n (Nin+Nh1)	// last layer-1

# define H21 (H1n+1)	// 1st layer-2
# define H2n (H1n+Nh2)	// last layer-2

# define OU1 (H2n+1)	// 1st output
# define OUn (H2n+Nou)	// last output

// no. of neurons
# define Nx  (1+Nin+Nh1+Nh2+Nou)

// no. of weights
# define Nw  ((Nin+1)*Nh1 + (Nh1+1)*Nh2 + (Nh2+1)*Nou)

// default activation functions
# ifndef Fh1
# define Fh1 relu
# endif
# ifndef Fh2
# define Fh2 relu
# endif
# ifndef Fou
# define Fou sigm
# endif

# include "mlp.h"

extern      void  mlp3_ff(mlp_t*);
extern      void  mlp3_bp(mlp_t*, float);
extern weights_t *mlp3_weights(const mlp_t*);
extern      void  mlp3_weights_load(mlp_t*,const weights_t*);

# define fforward(mlp)		   mlp3_ff(mlp);
# define backprop(mlp,gamma)	   mlp3_bp(mlp,gamma);
# define weights(mlp)		   mlp3_weights(mlp);
# define weights_load(mlp,weights) mlp3_weights_load(mlp,weights);

# endif
