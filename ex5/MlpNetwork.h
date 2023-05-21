//MlpNetwork.h

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Matrix.h"
#include "Digit.h"
#include "Dense.h"

#define MLP_SIZE 4

//
const matrix_dims img_dims = {28, 28};
const matrix_dims weights_dims[] = {{128, 784},
                                    {64,  128},
                                    {20,  64},
                                    {10,  20}};
const matrix_dims bias_dims[] = {{128, 1},
                                 {64,  1},
                                 {20,  1},
                                 {10,  1}};

class MlpNetwork {

 public:

  /**
   * constructor for the class
   * @param weights the weights for each layer
   * @param biases the biases for each layer
   */
  MlpNetwork (Matrix weights[], Matrix biases[]);

  /**
   * operator that activates the program on the input matrix
   * @param input the matrix to activate
   * @return a struct containing the number identified and the probability
   * of correctness
   */
  digit operator() (Matrix &input);

 private:
  Dense _first;
  Dense _second;
  Dense _third;
  Dense _forth;

};

#endif // MLPNETWORK_H
