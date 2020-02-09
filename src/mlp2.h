// mlp2.h: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// 2-layer multilayer perceptron

# ifndef MLP2_H
# define MLP2_H

# define IN1 (1)	// 1st input
# define INn (Nin)	// last (n-th) input

# define H11 (Nin+1)	// 1st hidden
# define H1n (Nin+Nh1)	// last hidden

# define OU1 (H1n+1)	// 1st output
# define OUn (H1n+Nou)	// last output

// no. of neurons
# define Nx  (1+Nin+Nh1+Nou)

// no. of weights
# define Nw  ((Nin+1)*Nh1 + (Nh1+1)*Nou)

// default activation functions
# ifndef Fh1
# define Fh1 relu
# endif
# ifndef Fou
# define Fou sigm
# endif

# include "mlp.h"

extern      void  mlp2_ff(mlp_t*);
extern      void  mlp2_bp(mlp_t*, float);
extern weights_t *mlp2_weights(const mlp_t*);
extern      void  mlp2_weights_load(mlp_t*,const weights_t*);

# define fforward(mlp)		   mlp2_ff(mlp);
# define backprop(mlp,gamma)	   mlp2_bp(mlp,gamma);
# define weights(mlp)		   mlp2_weights(mlp);
# define weights_load(mlp,weights) mlp2_weights_load(mlp,weights);

# endif
