// mlp1.h: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// 1-layer "multilayer" perceptron

# ifndef MLP1_H
# define MLP1_H

# define IN1 (1)	// 1st input
# define INn (Nin)	// last (n-th) input

# define OU1 (Nin+1)	// 1st output
# define OUn (Nin+Nou)	// last output

// no. of neurons
# define Nx  (1+Nin+Nou)

// no. of weights
# define Nw  ((Nin+1)*Nou)

// default activation functions
# ifndef Fou
# define Fou sigm
# endif

# include "mlp.h"

extern      void  mlp1_ff(mlp_t*);
extern      void  mlp1_bp(mlp_t*, float);
extern weights_t *mlp1_weights(const mlp_t*);
extern      void  mlp1_weights_load(mlp_t*,const weights_t*);

# define fforward(mlp)		   mlp1_ff(mlp);
# define backprop(mlp,gamma)	   mlp1_bp(mlp,gamma);
# define weights(mlp)		   mlp1_weights(mlp);
# define weights_load(mlp,weights) mlp1_weights_load(mlp,weights);

# endif
