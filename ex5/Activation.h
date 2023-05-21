//Activation.h

#ifndef ACTIVATION_H
#define ACTIVATION_H

#include "Matrix.h"
#define MINIMUM 0

/**
 * @enum ActivationType
 * @brief Indicator of activation function.
 */
enum ActivationType {
    RELU,
    SOFTMAX
};

class Activation {

 public:

  /**
   * constructor for the class
   * @param act_type the type of activation required
   */
  explicit Activation (ActivationType act_type);

  /**
   * A getter for the activation type
   * @return the activation type
   */
  ActivationType get_activation_type () const;

  /**
   * an operator that activates the matrix
   * @param mat the matrix to activate
   * @return an activated vector
   */
  Matrix operator() (const Matrix &mat) const;

 private:
  ActivationType _act_type;

};

#endif //ACTIVATION_H
