#include "Activation.h"
#include <cmath>

/**
 * constructor for the class
 * @param act_type the type of activation required
 */
Activation::Activation (ActivationType act_type) : _act_type (act_type)
{
}

/**
 * A getter for the activation type
 * @return the activation type
 */
ActivationType Activation::get_activation_type () const
{
  return _act_type;
}

/**
 * a function that uses RELU to activate the vector
 * @param mat the matrix to activate
 * @return an activated matrix as a vector
 */
Matrix re_lu (const Matrix &mat)
{
  Matrix new_mat (mat.get_rows (), mat.get_cols ());
  for (int ind = 0; ind < mat.get_cols () * mat.get_rows (); ++ind)
    {
      if (mat[ind] > MINIMUM)
        {
          new_mat[ind] = mat[ind];
        }
      else
        {
          new_mat[ind] = MINIMUM;
        }
    }
  return new_mat;
}

/**
 * a function that uses softmax to activate the vector
 * @param mat the matrix to activate
 * @return an activated matrix as a vector
 */
Matrix softmax (const Matrix &mat)
{
  Matrix new_mat (mat.get_rows (), mat.get_cols ());
  float temp = 0;
  for (int i = 0; i < mat.get_rows () * mat.get_cols (); i++)
    {
      new_mat[i] = std::exp (mat[i]);
      temp += new_mat[i];
    }
  new_mat = (1 / temp) * new_mat;
  return new_mat;
}

/**
 * an operator that activates the matrix
 * @param mat the matrix to activate
 * @return an activated vector
 */
Matrix Activation::operator() (const Matrix &mat) const
{
  if (_act_type == RELU)
    {
      return re_lu (mat);
    }
  return softmax (mat);
}
