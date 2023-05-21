#include "Matrix.h"
#include <iostream>
#include <cmath>

/**
 * default constructor for the class
 */
Matrix::Matrix () : mat_dims ()
{
  mat_dims.cols = INITIAL_COL;
  mat_dims.rows = INITIAL_ROW;
  _mat = new (std::nothrow) float[mat_dims.rows * mat_dims.cols];
  if (_mat == nullptr)
    {
      std::cerr << MEMORY_ERROR << std::endl;
      exit (EXIT_FAILURE);
    }
  _mat[0] = INITIAL_VAL;
}

/**
 * constructor for the class
 * @param rows number of rows
 * @param cols number of cols
 */
Matrix::Matrix (const int rows, const int cols) : mat_dims ()
{
  if (rows <= 0 || cols <= 0)
    {
      std::cerr << WRONG_SIZES_INPUT << std::endl;
      exit (EXIT_FAILURE);
    }
  mat_dims.cols = cols;
  mat_dims.rows = rows;
  _mat = new (std::nothrow) float[mat_dims.rows * mat_dims.cols];
  if (_mat == nullptr)
    {
      std::cerr << MEMORY_ERROR << std::endl;
      exit (EXIT_FAILURE);
    }
  for (int ind = 0; ind < mat_dims.rows * mat_dims.cols; ++ind)
    {
      _mat[ind] = INITIAL_VAL;
    }
}

/**
 * copy constructor
 * @param matrix a matrix to copy
 */
Matrix::Matrix (const Matrix &matrix) : mat_dims ()
{
  this->mat_dims.rows = matrix.mat_dims.rows;
  this->mat_dims.cols = matrix.mat_dims.cols;
  this->_mat = new (std::nothrow) float[mat_dims.rows * mat_dims.cols];
  if (_mat == nullptr)
    {
      std::cerr << MEMORY_ERROR << std::endl;
      exit (EXIT_FAILURE);
    }
  for (int ind = 0; ind < mat_dims.cols * mat_dims.rows; ++ind)
    {
      this->_mat[ind] = matrix._mat[ind];
    }
}

/**
 * destructor for the class
 */
Matrix::~Matrix ()
{
  delete[] _mat;
}

/**
 * getter for the rows
 * @return number of rows in the matrix
 */
int Matrix::get_rows () const
{
  return mat_dims.rows;
}

/**
 * getter for the columns
 * @return number of columns in the matrix
 */
int Matrix::get_cols () const
{
  return mat_dims.cols;
}

/**
 * transposes the matrix
 */
Matrix &Matrix::transpose ()
{
  Matrix new_mat = *this;
  int temp = mat_dims.rows;
  mat_dims.rows = mat_dims.cols;
  mat_dims.cols = temp;
  for (int row = 0; row < mat_dims.rows; ++row)
    {
      for (int col = 0; col < mat_dims.cols; ++col)
        {
          (*this) (row, col) = new_mat (col, row);
        }
    }
  return *this;
}

/**
 * vectorizes the matrix
 */
Matrix &Matrix::vectorize ()
{
  mat_dims.rows = mat_dims.rows * mat_dims.cols;
  mat_dims.cols = INITIAL_COL;
  return *this;
}

/**
 * prints the matrix
 */
void Matrix::plain_print () const
{
  int row_size = mat_dims.cols;
  for (int row = 0; row < mat_dims.rows; ++row)
    {
      for (int col = 0; col < mat_dims.cols; ++col)
        {
          std::cout << _mat[row * row_size + col] << SPACE;
        }
      std::cout << std::endl;
    }
}

/**
 * dottes the matrix
 * @param mat the matrix
 * @return a new matrix that has been dotted
 */
Matrix Matrix::dot (const Matrix &mat) const
{
  if (this->mat_dims.cols != mat.mat_dims.cols
      || this->mat_dims.rows != mat.mat_dims.rows)
    {
      std::cerr << MATRIX_SIZE_ERROR << std::endl;
      exit (EXIT_FAILURE);
    }
  Matrix new_mat (mat_dims.rows, mat_dims.cols);
  for (int ind = 0; ind < this->mat_dims.rows * this->mat_dims.cols; ++ind)
    {
      new_mat._mat[ind] = this->_mat[ind] * mat._mat[ind];
    }
  return new_mat;
}

/**
 * calculates the norm of the matrix
 * @return the norm of the matrix
 */
float Matrix::norm () const
{
  const Matrix &temp = (*this).dot (*this);
  float sum = 0;
  for (int ind = 0; ind < temp.mat_dims.cols * temp.mat_dims.rows; ++ind)
    {
      sum += temp._mat[ind];
    }
  return sqrtf (sum);
}

/**
  * reads from a binary file
  * @param file the file to read
  * @param mat the matrix to store the data inside
  */
std::istream &read_binary_file (std::istream &file, Matrix &mat)
{
  unsigned long word_to_read =
      mat.mat_dims.rows * mat.mat_dims.cols * sizeof (float);
  file.seekg (0,std::istream::end);
  unsigned long num_of_bytes = file.tellg();
  if (num_of_bytes < word_to_read)
    {
      std::cerr << FILE_ERROR << std::endl;
      exit (EXIT_FAILURE);
    }
  file.seekg (0, std::istream::beg);
  file.read ((char *) mat._mat, (long) word_to_read);
  return file;
}

/**
 * Matrix multiplication
 * @param mat the matrix to multiply
 * @return the product of the multiplication
 */
Matrix Matrix::operator* (const Matrix &mat) const
{
  if (this->mat_dims.cols != mat.mat_dims.rows)
    {
      std::cerr << MATRIX_SIZE_ERROR << std::endl;
      exit (EXIT_FAILURE);
    }
  Matrix new_mat (mat_dims.rows, mat.mat_dims.cols);
  for (int i = 0; i < new_mat.mat_dims.rows; ++i)
    {
      for (int j = 0; j < new_mat.mat_dims.cols; ++j)
        {
          for (int k = 0; k < this->mat_dims.cols; ++k)
            {
              new_mat._mat[i * new_mat.mat_dims.cols + j] +=
                  this->_mat[i * mat_dims.cols + k]
                  * mat._mat[j + mat.mat_dims.cols * k];
            }
        }
    }
  return new_mat;
}

/**
 * Matrix addition
 * @param mat the matrix to add to
 * @return a new matrix
 */
Matrix Matrix::operator+ (const Matrix &mat) const
{
  if (this->mat_dims.cols != mat.mat_dims.cols
      || this->mat_dims.rows != mat.mat_dims.rows)
    {
      std::cerr << MATRIX_SIZE_ERROR << std::endl;
      exit (EXIT_FAILURE);
    }
  Matrix new_mat (mat_dims.rows, mat_dims.cols);
  for (int ind = 0; ind < this->mat_dims.cols * this->mat_dims.rows; ++ind)
    {
      new_mat._mat[ind] = this->_mat[ind] + mat._mat[ind];
    }
  return new_mat;
}

/**
 * Scalar multiplication on the right
 * @param num the scalar
 * @return a new matrix
 */
Matrix Matrix::operator* (float num) const
{
  Matrix new_mat (mat_dims.rows, mat_dims.cols);
  for (int ind = 0; ind < this->mat_dims.cols * this->mat_dims.rows; ++ind)
    {
      new_mat._mat[ind] = this->_mat[ind] * num;
    }
  return new_mat;
}

/**
 * Matrix addition accumulation
 * @param mat the matrix to add to
 * @return the same matrix
 */
Matrix &Matrix::operator+= (const Matrix &mat)
{
  if (this->mat_dims.cols != mat.mat_dims.cols
      || this->mat_dims.rows != mat.mat_dims.rows)
    {
      std::cerr << MATRIX_SIZE_ERROR << std::endl;
      exit (EXIT_FAILURE);
    }
  for (int ind = 0; ind < this->mat_dims.cols * this->mat_dims.rows; ++ind)
    {
      this->_mat[ind] = this->_mat[ind] + mat._mat[ind];
    }
  return *this;
}

/**
 * Parenthesis indexing
 * @param num1 the row index
 * @param num2 the col index
 * @return a float in the right indexes
 */
float Matrix::operator() (const int row, const int col) const
{
  if (row < 0 || row >= mat_dims.rows || col < 0 || col >= mat_dims.cols)
    {
      std::cerr << ERROR_RANGE_INDEX << std::endl;
      exit (EXIT_FAILURE);
    }
  return this->_mat[row * mat_dims.cols + col];
}

/**
 * Brackets indexing
 * @param num the index
 * @return a float in the right index
 */
float Matrix::operator[] (const int num) const
{
  if (num >= mat_dims.cols * mat_dims.rows || num < 0)
    {
      std::cerr << ERROR_RANGE_INDEX << std::endl;
      exit (EXIT_FAILURE);
    }
  return this->_mat[num];
}

float &Matrix::operator[] (const int num)
{
  if (num >= mat_dims.cols * mat_dims.rows || num < 0)
    {
      std::cerr << ERROR_RANGE_INDEX << std::endl;
      exit (EXIT_FAILURE);
    }
  return this->_mat[num];
}

/**
 * Output stream, Pretty export of matrix
 * @param os output stream
 * @param mat the matrix to output
 * @return the output stream
 */
std::ostream &operator<< (std::ostream &os, const Matrix &mat)
{
  for (int row = 0; row < mat.get_rows (); ++row)
    {
      for (int col = 0; col < mat.get_cols (); ++col)
        {
          if (mat (row, col) > THRASH_HOLD)
            {
              os << DOUBLE_SPACE;
              continue;
            }
          os << DOUBLE_ASTERISK;
        }
      os << std::endl;
    }
  return os;
}

/**
 * Assignment
 * @param mat the matrix to copy
 * @return a changed matrix
 */
Matrix &Matrix::operator= (const Matrix &mat)
{
  if (this == &mat)
    {
      return *this;
    }
  this->mat_dims.rows = mat.mat_dims.rows;
  this->mat_dims.cols = mat.mat_dims.cols;
  delete[] _mat;
  this->_mat = new float[mat_dims.cols * mat_dims.rows];
  for (int ind = 0; ind < mat.mat_dims.cols * mat.mat_dims.rows; ++ind)
    {
      this->_mat[ind] = mat._mat[ind];
    }
  return *this;
}

/**
 * Scalar multiplication on the left
 * @param num the scalar
 * @param mat the matrix
 * @return a new matrix
 */
Matrix operator* (float num, const Matrix &mat)
{
  Matrix new_mat (mat.mat_dims.rows, mat.mat_dims.cols);
  for (int ind = 0; ind < mat.mat_dims.cols * mat.mat_dims.rows; ++ind)
    {
      new_mat._mat[ind] = mat._mat[ind] * num;
    }
  return new_mat;
}

/**
   * Brackets indexing
   * @param num the index
   * @return a float in the right index by reference
   */
float &Matrix::operator() (int row, int col)
{
  if (row < 0 || row >= mat_dims.rows || col < 0 || col >= mat_dims.cols)
    {
      std::cerr << ERROR_RANGE_INDEX << std::endl;
      exit (EXIT_FAILURE);
    }
  return this->_mat[row * mat_dims.cols + col];
}
