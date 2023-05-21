#include "Dense.h"

/**
 *
 * @param w a matrix being the weights
 * @param bias a matrix being the bias
 * @param act_type the activation type
 */
Dense::Dense (const Matrix &w, const Matrix &bias, ActivationType act_type)
    : _weights (w), _bias (bias), _act (act_type)
{
}

/**
 * a getter for the weights
 * @return the weights
 */
Matrix Dense::get_weights () const
{
  return _weights;
}

/**
 * a getter fot the bias
 * @return the bias
 */
Matrix Dense::get_bias () const
{
  return _bias;
}

/**
 * a getter for the activation type
 * @return the activation type
 */
Activation Dense::get_activation () const
{
  return _act;
}

Matrix Dense::operator() (const Matrix &input) const
{
  return _act (_weights * input + _bias);
}


