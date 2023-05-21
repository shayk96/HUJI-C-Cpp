#include "MlpNetwork.h"

/**
 * constructor for the class
 * @param weights the weights for each layer
 * @param biases the biases for each layer
 */
MlpNetwork::MlpNetwork (Matrix *weights, Matrix *biases)
    : _first (Dense (weights[0], biases[0], RELU)),
      _second (Dense (weights[1], biases[1], RELU)),
      _third (Dense (weights[2], biases[2], RELU)),
      _forth (Dense (weights[3], biases[3], SOFTMAX))
{}

/**
 * operator that activates the program on the input matrix
 * @param input the matrix to activate
 * @return a struct containing the number identified and the probability
 * of correctness
 */
digit MlpNetwork::operator() (Matrix &input)
{
  Matrix in, out;
  float temp = 0;
  unsigned int indx = 0;
  out = _first (input.vectorize());
  in = _second (out);
  out = _third (in);
  in = _forth (out);
  for (int ind = 0; ind < in.get_rows () * in.get_cols (); ++ind)
    {
      if (in[ind] > temp)
        {
          temp = in[ind];
          indx = ind;
        }
    }
  digit final;
  final.probability = temp;
  final.value = indx;
  return final;
}








