#ifndef C___PROJECT_DENSE_H
#define C___PROJECT_DENSE_H

#include "Activation.h"

class Dense {

 public:

  /**
   *
   * @param w a matrix being the weights
   * @param bias a matrix being the bias
   * @param act_type the activation type
   */
  Dense (const Matrix &w, const Matrix &bias, ActivationType act_type);

  /**
   * a getter for the weights
   * @return the weights
   */
  Matrix get_weights () const;

  /**
   * a getter fot the bias
   * @return the bias
   */
  Matrix get_bias () const;

  /**
   * a getter for the activation type
   * @return the activation type
   */
  Activation get_activation () const;

  Matrix operator() (const Matrix &input) const;

 private:
  Matrix _weights;
  Matrix _bias;
  Activation _act;
};

#endif //C___PROJECT_DENSE_H
