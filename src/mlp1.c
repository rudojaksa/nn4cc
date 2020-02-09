// mlp1.c: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// 1-layer "multilayer" perceptron

# include <stdlib.h>
# include "CONFIG.h"
# ifdef MLP1_H

// ------------------------------------------------------------------------------- FORWARD AND BACKPROP

// forward propagation
void mlp1_ff(mlp_t *mlp) {
  blk_ff(mlp,OU,IN,Fou); }		// in -> ou

// basic backpropagation learning (LMS only for no-hidden-layer)
void mlp1_bp(mlp_t *mlp, float gamma) {
  mlp1_ff(mlp);				// forward propagation
  blk_lms(mlp,OU,Fou);			// ou
  blk_update(mlp,blk(OU,IN),gamma); }	// in -> ou

// ---------------------------------------------------------------------------------- WEIGHTS LOAD/SAVE
// these routines provide dense weights packing and they define the order of weights on the tape

// return weights vector
weights_t *mlp1_weights(const mlp_t *mlp) {
  weights_t *weights = malloc(sizeof(weights_t));
  weights->w = malloc(Nw * sizeof(float)); 		// actully we don't use full matrix!
  int k=0;
  forblk(OU,i) weights->w[k++]=w(i,0);			// biases to OU neurons
  weights->in1 = k;
  forblk(IN,j) forblk(OU,i) weights->w[k++]=w(i,j);	// weights IN->OU
  weights->inn = k;
  weights->nw = k;
  return(weights); }

// load weights vector into network
void mlp1_weights_load(mlp_t *mlp, const weights_t *weights) {
  int k=0;
  forblk(OU,i) w(i,0)=weights->w[k++];	// biases to OU neurons
  forblk(IN,j) forblk(OU,i) w(i,j)=weights->w[k++]; }	// weights IN->OU

# endif // --------------------------------------------------------------------------------------------
