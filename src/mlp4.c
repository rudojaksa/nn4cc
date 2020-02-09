// mlp4.c: part of nn4cc, (c) R.Jaksa 2019, GPLv3 
// 4-layer multilayer perceptron

# include <stdlib.h>
# include "CONFIG.h"
# ifdef MLP4_H

// ------------------------------------------------------------------------------- FORWARD AND BACKPROP

// forward propagation
void mlp4_ff(mlp_t *mlp) {
  blk_ff(mlp,H1,IN,Fh1);		// in -> h1
  blk_ff(mlp,H2,H1,Fh2);		// h1 -> h2
  blk_ff(mlp,H3,H2,Fh3);		// h2 -> h3
  blk_ff(mlp,OU,H3,Fou); }		// h3 -> ou

// basic backpropagation learning
void mlp4_bp(mlp_t *mlp, float gamma) {
  mlp4_ff(mlp);				// forward propagation
  blk_lms(mlp,OU,Fou);			// ou
  blk_bp(mlp,H3,OU,Fh3);		// h3 <- ou
  blk_bp(mlp,H2,H3,Fh2);		// h2 <- h3
  blk_bp(mlp,H1,H2,Fh1);		// h1 <- h2
  blk_update(mlp,blk(OU,H3),gamma);	// h3 -> ou
  blk_update(mlp,blk(H3,H2),gamma);	// h2 -> h3
  blk_update(mlp,blk(H2,H1),gamma);	// h1 -> h2
  blk_update(mlp,blk(H1,IN),gamma); }	// in -> h1

// ---------------------------------------------------------------------------------- WEIGHTS LOAD/SAVE
// these routines provide dense weights packing and they define the order of weights on the tape

// return weights vector
weights_t *mlp4_weights(const mlp_t *mlp) {
  weights_t *weights = malloc(sizeof(weights_t));
  weights->w = malloc(Nw * sizeof(float));		// actully we don't use full matrix!
  int k=0;
  forblk(H1,i) weights->w[k++]=w(i,0);			// biases to H1 neurons
  forblk(H2,i) weights->w[k++]=w(i,0);			// biases to H2 neurons
  forblk(H3,i) weights->w[k++]=w(i,0);			// biases to H3 neurons
  forblk(OU,i) weights->w[k++]=w(i,0);			// biases to OU neurons
  weights->in1 = k;
  forblk(IN,j) forblk(H1,i) weights->w[k++]=w(i,j);	// weights IN->H1
  weights->inn = k;
  forblk(H1,j) forblk(H2,i) weights->w[k++]=w(i,j);	// weights H1->H2
  forblk(H2,j) forblk(H3,i) weights->w[k++]=w(i,j);	// weights H2->H3
  forblk(H3,j) forblk(OU,i) weights->w[k++]=w(i,j);	// weights H3->OU
  weights->nw = k;
  return(weights); }

// load weights vector into network
void mlp4_weights_load(mlp_t *mlp, const weights_t *weights) {
  int k=0;
  forblk(H1,i) w(i,0)=weights->w[k++];			// biases to H1 neurons
  forblk(H2,i) w(i,0)=weights->w[k++];			// biases to H2 neurons
  forblk(H3,i) w(i,0)=weights->w[k++];			// biases to H3 neurons
  forblk(OU,i) w(i,0)=weights->w[k++];			// biases to OU neurons
  forblk(IN,j) forblk(H1,i) w(i,j)=weights->w[k++];	// weights IN->H1
  forblk(H1,j) forblk(H2,i) w(i,j)=weights->w[k++];	// weights H1->H2
  forblk(H2,j) forblk(H3,i) w(i,j)=weights->w[k++];	// weights H2->H3
  forblk(H3,j) forblk(OU,i) w(i,j)=weights->w[k++]; }	// weights H3->OU

# endif // --------------------------------------------------------------------------------------------
