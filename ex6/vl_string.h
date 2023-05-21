#ifndef _VL_STRING_H_
#define _VL_STRING_H_

#include "vl_vector.h"
#include <cstring>

template<size_t StaticCapacity = DEFAULT_CAP>
class vl_string : public vl_vector<char, StaticCapacity> {

 public:

  typedef char *iterator;
  typedef const char *const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  /**
   * default constructor for the class
   */
  vl_string () : vl_vector<char, StaticCapacity> (1, '\0')
  {}

  /**
   * copy constructor for the class
   * @param other the vector to copy
   */
  vl_string (const vl_string<StaticCapacity> &other)
      : vl_vector<char, StaticCapacity> (other)
  {}

  /**
   * a constructor that gets a string and creates a vector containing it
   * @param input the string
   */
  vl_string (const char *input)
      : vl_vector<char, StaticCapacity> (input, input + strlen (input) + 1)
  {}

  /**
   * The function checks whether the vector is empty or not
   * @return True if empty else False
   */
  bool empty () const noexcept override
  {
    return this->size () == DEFAULT_VECTOR_SIZE;
  }

  /**
   * The function returns the amount of elements currently in vector.
   * @return number of elements in the vector
   */
  size_t size () const noexcept override
  {
    return strlen (this->data ());
  }

  /**
   * The function receives an element and adds it to the end of the vector.
   * @param element the element
   */
  void push_back (const char element) override
  {
    this->insert (this->begin () + size (), element);
  }

  /**
   * The function removes the last element from the vector.
   */
  void pop_back () override
  {
    if (size () > 0)
      {
        this->erase (this->begin () + size () - 1);
      }
  }

  /**
   * An function that removes all the vector elements.
   */
  void clear () override
  {
    this->erase (this->begin (), this->end () - 1);
  }

  /**
   * The function receives a char element and returns boolean value - Is the
   * value found in the vector?
   * @param element T-variable
   * @return True if element in vector else False
   */
  bool contains (const char *element) const noexcept
  {
    size_t length = strlen (element);
    if (length > size ())
      {
        return false;
      }
    for (size_t index = 0; index <= this->size () - length; ++index)
      {
        if (this->at (index) == *element)
          {
            bool flag = true;
            for (size_t j = 0; j < length; ++j)
              {
                if (this->at (index + j) != *(element + j))
                  {
                    flag = false;
                    break;
                  }
              }
            if (flag)
              {
                return true;
              }
            flag = false;
            continue;
          }
      }
    return false;
  }

  /**
   * add the other string to the original string and returns the updated
   * string by reference
   * @param other the string to add
   * @return original updated string
   */
  vl_string<StaticCapacity> &
  operator+= (const vl_string<StaticCapacity> &other)
  {
    this->insert (this->begin () + size (), other.begin (), other.end () - 1);
    return *this;
  }

  /**
   * add the other string to the original string and returns the updated
   * string by reference
   * @param other the string to add
   * @return original updated string
   */
  vl_string<StaticCapacity> &operator+= (const char *&other)
  {
    size_t length = strlen (other);
    this->insert (this->begin () + size (), other, other + length);
    return *this;
  }

  /**
   * add the other string to the original string and returns the updated
   * string by reference
   * @param other the string to add
   * @return original updated string
   */
  vl_string<StaticCapacity> &operator+= (const char &other)
  {
    push_back (other);
    return *this;
  }

  /**
   * add the other string to the original string and returns a new vector
   * @param other the string to add
   * @return new updated vector
   */
  vl_string<StaticCapacity>
  operator+ (const vl_string<StaticCapacity> &other) const
  {
    vl_string<StaticCapacity> vl_copy (*this);
    vl_copy.insert (
        vl_copy.begin () + size (), other.begin (), other.end () - 1);
    return vl_copy;
  }

  /**
   * add the other string to the original string and returns a new vector
   * @param other the string to add
   * @return new updated vector
   */
  vl_string<StaticCapacity> operator+ (const char *&other) const
  {
    vl_string<StaticCapacity> vl_copy (*this);
    size_t length = strlen (other);
    vl_copy.insert (vl_copy.begin () + size (), other, other + length);
    return vl_copy;
  }

  /**
   * add the other string to the original string and returns a new vector
   * @param other the string to add
   * @return new updated vector
   */
  vl_string<StaticCapacity> operator+ (const char &other) const
  {
    vl_string<StaticCapacity> vl_copy (*this);
    vl_copy.push_back (other);
    return vl_copy;
  }

  /**
   * Returns a pointer to the first element in the data as an Iterator
   * @return an Iterator pointer
   */
  reverse_iterator rbegin () noexcept override
  {
    return std::reverse_iterator<iterator> (this->end () - 1);
  }

  /**
   * Returns a pointer to the last element in the data as an Iterator
   * @return an Iterator pointer
   */
  reverse_iterator rend () noexcept override
  {
    return std::reverse_iterator<iterator> (this->begin ());
  }

  /**
   * Returns a pointer to the first element in the data as an Iterator
   * @return an Iterator pointer
   */
  const_reverse_iterator rbegin () const noexcept override
  {
    return std::reverse_iterator<const_iterator> (this->end () - 1);
  }

  /**
   * Returns a pointer to the last element in the data as an Iterator
   * @return an Iterator pointer
   */
  const_reverse_iterator rend () const noexcept override
  {
    return std::reverse_iterator<const_iterator> (this->begin ());
  }

  /**
   * Returns a pointer to the first element in the data as an Iterator
   * @return an Iterator pointer
   */
  const_reverse_iterator crbegin () const noexcept override
  {
    return std::reverse_iterator<const_iterator> (rbegin ());
  }

  /**
   * Returns a pointer to the last element in the data as an Iterator
   * @return an Iterator pointer
   */
  const_reverse_iterator crend () const noexcept override
  {
    return std::reverse_iterator<const_iterator> (rend ());
  }

  /**
   * casting operator
   * @return pointer
   */
  operator const char * () const noexcept
  {
    return this->begin ();
  }

};

#endif //_VL_STRING_H_