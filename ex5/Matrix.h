#ifndef MATRIX_H
#define MATRIX_H

#include <istream>
#define THRASH_HOLD  0.1
#define MEMORY_ERROR "Error: Memory allocation failure"
#define INITIAL_ROW 1
#define INITIAL_COL 1
#define INITIAL_VAL 0
#define SPACE " "
#define DOUBLE_SPACE "  "
#define DOUBLE_ASTERISK "**"
#define MATRIX_SIZE_ERROR "Error: Matrices are not of the same size"
#define FILE_ERROR "Error: Failed to read file"
#define ERROR_RANGE_INDEX "Error: Index out of range"
#define WRONG_SIZES_INPUT "Error: wrong sizes for creation of matrix"

/**
 * @struct matrix_dims
 * @brief Matrix dimensions container. Used in MlpNetwork.h and main.cpp
 */
typedef struct matrix_dims {
    int rows, cols;
} matrix_dims;

class Matrix {
 public:
  /**
   * default constructor for the class
   */
  Matrix ();

  /**
   * constructor for the class
   * @param rows number of rows
   * @param cols number of cols
   */
  Matrix (int rows, int cols);

  /**
   * copy constructor
   * @param matrix a matrix to copy
   */
  Matrix (const Matrix &matrix);

  /**
   * destructor for the class
   */
  ~Matrix ();

  /**
   * getter for the rows
   * @return number of rows in the matrix
   */
  int get_rows () const;

  /**
   * getter for the columns
   * @return number of columns in the matrix
   */
  int get_cols () const;

  /**
   * transposes the matrix
   */
  Matrix &transpose ();

  /**
   * vectorizes the matrix
   */
  Matrix &vectorize ();

  /**
   * prints the matrix
   */
  void plain_print () const;

  /**
   * dottes the matrix
   * @param mat the matrix
   * @return a new matrix that has been dotted
   */
  Matrix dot (const Matrix &mat) const;

  /**
   * calculates the norm of the matrix
   * @return the norm of the matrix
   */
  float norm () const;

  /**
   * reads from a binary file
   * @param file the file to read
   * @param mat the matrix to store the data inside
   */
  friend std::istream &read_binary_file (std::istream &file, Matrix &mat);

  /**
   * Matrix multiplication
   * @param mat the matrix to multiply
   * @return the product of the multiplication
   */
  Matrix operator* (const Matrix &mat) const;

  /**
   * Matrix addition
   * @param mat the matrix to add to
   * @return a new matrix
   */
  Matrix operator+ (const Matrix &mat) const;

  /**
   * Assignment
   * @param mat the matrix to copy
   * @return a changed matrix
   */
  Matrix &operator= (const Matrix &mat);

  /**
   * Scalar multiplication on the right
   * @param num the scalar
   * @return a new matrix
   */
  Matrix operator* (float num) const;

  /**
   * Matrix addition accumulation
   * @param mat the matrix to add to
   * @return the same matrix
   */
  Matrix &operator+= (const Matrix &mat);

  /**
   * Parenthesis indexing
   * @param num1 the row index
   * @param num2 the col index
   * @return a float in the right indexes
   */
  float operator() (int row, int col) const;

  /**
   * Brackets indexing
   * @param num the index
   * @return a float in the right index by reference
   */
  float &operator() (int row, int col);

  /**
   * Brackets indexing
   * @param num the index
   * @return a float in the right index
   */
  float operator[] (int num) const;

  /**
   * Brackets indexing
   * @param num the index
   * @return a float in the right index by reference
   */
  float &operator[] (int num);

  /**
   * Output stream, Pretty export of matrix
   * @param os output stream
   * @param mat the matrix to output
   * @return the output stream
   */
  friend std::ostream &operator<< (std::ostream &os, const Matrix &mat);

  /**
   * Scalar multiplication on the left
   * @param num the scalar
   * @param mat the matrix
   * @return a new matrix
   */
  friend Matrix operator* (float num, const Matrix &mat);

 private:
  float *_mat;
  matrix_dims mat_dims;
};

#endif //MATRIX_H
