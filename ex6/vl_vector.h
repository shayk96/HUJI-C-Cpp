#include <algorithm>
#include <stdexcept>
#include <iterator>

#ifndef _VL_VECTOR_H_

#define _VL_VECTOR_H_

#define STACK 0
#define DYNAMIC 1
#define DEFAULT_CAP 16
#define DEFAULT_INSERT_SIZE 1
#define DEFAULT_ERASE_SIZE 1
#define DEFAULT_VECTOR_SIZE 0
#define RANGE_ERROR "index out of range"

template<typename T, size_t StaticCapacity = DEFAULT_CAP>

class vl_vector {

 public:
  typedef T *iterator;
  typedef const T *const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  /**
   * default constructor for the class
   */
  vl_vector ()
      : dynamic_memory (nullptr), memory_type (STACK), stack_memory (),
        vector_size (DEFAULT_VECTOR_SIZE), vector_cap (StaticCapacity)
  {}

  /**
   * copy constructor for the class
   * @param v the vector to copy
   */
  vl_vector (const vl_vector &v)
      : dynamic_memory (nullptr), memory_type (v.memory_type), stack_memory (),
        vector_size (v.vector_size), vector_cap (v.vector_cap)
  {
    if (memory_type == DYNAMIC)
      {
        dynamic_memory = new T[vector_cap];
        elements_copy (data (), v.data ());
      }
    if (memory_type == STACK)
      {
        elements_copy (data (), v.data ());
      }
  }

  /**
   * a constructor that receives  a sequence of iterators and creates a vector
   * from them
   * @tparam ForwardIterator Iterator for the sequence
   * @param first Iterator that points to the first element
   * @param last Iterator that points to the second element
   */
  template<class ForwardIterator>
  vl_vector (ForwardIterator first, ForwardIterator last)
      : dynamic_memory (nullptr), memory_type (STACK), stack_memory (),
        vector_size (DEFAULT_VECTOR_SIZE), vector_cap (StaticCapacity)
  {
    size_t num_of_elements = std::distance (first, last);
    size_t potential_vec_size = cap_c (DEFAULT_VECTOR_SIZE, num_of_elements);
    if (potential_vec_size == 0)
      {
        for (size_t ind = 0; ind < num_of_elements; ++ind)
          {
            stack_memory[ind] = *first;
            first++;
          }
        vector_cap = StaticCapacity;
      }
    else
      {
        dynamic_memory = new T[potential_vec_size];
        for (size_t ind = 0; ind < num_of_elements; ++ind)
          {
            dynamic_memory[ind] = *first;
            first++;
          }
        memory_type = DYNAMIC;
        vector_cap = potential_vec_size;
      }
    vector_size = num_of_elements;
  }

  /**
   * a constructor that receives an element and creates a vector containing the
   * element as many times as the user wants
   * @param count number of times the element is created
   * @param v
   */
  vl_vector (const size_t count, const T v)
      : dynamic_memory (nullptr), memory_type (STACK), stack_memory (),
        vector_size (DEFAULT_VECTOR_SIZE), vector_cap (StaticCapacity)
  {
    size_t potential_vec_size = cap_c (DEFAULT_VECTOR_SIZE, count);
    if (potential_vec_size < StaticCapacity)
      {
        for (size_t ind = 0; ind < count; ++ind)
          {
            stack_memory[ind] = v;
          }
        vector_cap = StaticCapacity;
      }
    else
      {
        dynamic_memory = new T[potential_vec_size];
        for (size_t ind = 0; ind < count; ++ind)
          {
            dynamic_memory[ind] = v;
          }
        memory_type = DYNAMIC;
        vector_cap = potential_vec_size;
      }
    vector_size = count;
  }

  /**
   * destructor for the class
   */
  virtual ~vl_vector ()
  {
    if (memory_type == DYNAMIC)
      {
        delete[] dynamic_memory;
      }
  }

  /**
   * A function that returns a pointer to the variable that Currently holds
   * the data.
   * @return a pointer to the data
   */
  T *data () noexcept
  {
    if (memory_type == DYNAMIC)
      {
        return dynamic_memory;
      }
    else
      {
        return stack_memory;
      }
  }

  /**
   * A function that returns a pointer to the variable that currently holds
   * the data.
   * @return a pointer to the data
   */
  const T *data () const noexcept
  {
    if (memory_type == DYNAMIC)
      {
        return dynamic_memory;
      }
    else
      {
        return stack_memory;
      }
  }

  /**
   * The function returns the amount of elements currently in vector.
   * @return number of elements in the vector
   */
  virtual size_t size () const noexcept
  {
    return vector_size;
  }

  /**
   * The function returns the current vector capacity.
   * @return the vector capacity
   */
  size_t capacity () const noexcept
  {
    return vector_cap;
  }

  /**
   * The function checks whether the vector is empty or not
   * @return True if empty else False
   */
  virtual bool empty () const noexcept
  {
    return vector_size == DEFAULT_VECTOR_SIZE;
  }

  /**
   * The function gets an index and returns the the value associated with it
   * in the vector.
   * @param index
   * @return
   */
  T at (const long int index) const
  {
    if (index < 0 || index >= (long int) vector_size)
      {
        throw std::out_of_range (RANGE_ERROR);
      }
    return data ()[index];
  }

  /**
   * The function gets an index and returns the the value associated with it
   * in the vector.
   * @param index
   * @return
   */
  T &at (const long int index)
  {
    if (index < 0 || index >= (long int) vector_size)
      {
        throw std::out_of_range (RANGE_ERROR);
      }
    return data ()[index];
  }

  /**
   * The function receives an element and adds it to the end of the vector.
   * @param element the element
   */
  virtual void push_back (const T element)
  {
    if ((memory_type == STACK
         && vector_size + DEFAULT_INSERT_SIZE > StaticCapacity)
        || (memory_type == DYNAMIC
            && cap_c (vector_size, DEFAULT_INSERT_SIZE) != 0))
      {
        extend_capacity (DEFAULT_INSERT_SIZE);
      }
    vector_size++;
    this->at (vector_size - 1) = element;
  }

  /**
   * Returns a pointer to the first element in the data as an Iterator
   * @return an Iterator pointer
   */
  iterator begin () noexcept
  {
    return data ();
  }

  /**
   * Returns a pointer to an element one over the last element in the data
   * as an Iterator
   * @return an Iterator pointer
   */
  iterator end () noexcept
  {
    return data () + vector_size;
  }

  /**
   * Returns a pointer to the first element in the data as an Iterator
   * @return an Iterator pointer
   */
  const_iterator cbegin () const noexcept
  {
    return data ();
  }

  /**
   * Returns a pointer to an element one over the last element in the data
   * as an Iterator
   * @return an Iterator pointer
   */
  const_iterator cend () const noexcept
  {
    return data () + vector_size;
  }

  /**
   * Returns a pointer to the first element in the data as an Iterator
   * @return an Iterator pointer
   */
  const_iterator begin () const noexcept
  {
    return data ();
  }

  /**
   * Returns a pointer to an element one over the last element in the data
   * as an Iterator
   * @return an Iterator pointer
   */
  const_iterator end () const noexcept
  {
    return data () + vector_size;
  }

  /**
   * Returns a pointer to the first element in the data as an Iterator
   * @return an Iterator pointer
   */
  virtual reverse_iterator rbegin () noexcept
  {
    return std::reverse_iterator<iterator> (end ());
  }

  /**
   * Returns a pointer to the last element in the data as an Iterator
   * @return an Iterator pointer
   */
  virtual reverse_iterator rend () noexcept
  {
    return std::reverse_iterator<iterator> (begin ());
  }

  /**
   * Returns a pointer to the first element in the data as an Iterator
   * @return an Iterator pointer
   */
  virtual const_reverse_iterator rbegin () const noexcept
  {
    return std::reverse_iterator<const_iterator> (end ());
  }

  /**
   * Returns a pointer to the last element in the data as an Iterator
   * @return an Iterator pointer
   */
  virtual const_reverse_iterator rend () const noexcept
  {
    return std::reverse_iterator<const_iterator> (begin ());
  }

  /**
   * Returns a pointer to the first element in the data as an Iterator
   * @return an Iterator pointer
   */
  virtual const_reverse_iterator crbegin () const noexcept
  {
    return std::reverse_iterator<const_iterator> (rbegin ());
  }

  /**
   * Returns a pointer to the last element in the data as an Iterator
   * @return an Iterator pointer
   */
  virtual const_reverse_iterator crend () const noexcept
  {
    return std::reverse_iterator<const_iterator> (rend ());
  }

  /**
   * A function that receives an iterator pointing to an element specific
   * in the vector (position,) and 2 variables representing Iterator Input to
   * the section. The action will add all Iterator values before the position.
   * @tparam ForwardIterator an Iterator
   * @param position element specific in the vector
   * @param first first element representing Iterator Input to the section
   * @param last one past the last element representing Iterator Input
   * to the section
   * @return an iterator of The vector, which points to the first new element
   */
  template<class ForwardIterator>
  iterator
  insert (const_iterator position, ForwardIterator first,
          ForwardIterator last)
  {
    size_t num_of_elem = std::distance (first, last);
    size_t pos_ind = ind_of_itr (position);
    if ((memory_type == STACK && vector_size + num_of_elem > StaticCapacity)
        || (memory_type == DYNAMIC && cap_c (vector_size, num_of_elem) != 0))
      {
        extend_capacity (num_of_elem);
        position = begin () + pos_ind;
      }
    T *temp = new T[vector_size - pos_ind];
    int count = 0;
    for (size_t ind = pos_ind; ind < vector_size; ++ind)
      {
        temp[count] = at (ind);
        count++;
      }
    std::copy (first, last, begin () + ind_of_itr (position));
    std::copy (temp,
               temp + count, begin () + ind_of_itr (position) + num_of_elem);
    vector_size += num_of_elem;
    delete[] temp;
    return data () + ind_of_itr (position);
  }

  /**
   * An function that receives an iterator pointing to an element of a specific
   * vector (position,) and a new element. the operation will add the new
   * element to the left of the position.
   * @param position iterator pointing to an element
   * @param element a new element
   * @return an iterator pointing to the new element
   */
  iterator insert (const_iterator position, const T element)
  {
    T temp[DEFAULT_INSERT_SIZE] = {element};
    return insert (position, temp, temp + DEFAULT_INSERT_SIZE);
  }

  /**
   * The operation accepts 2 representing variables Iterator of the vector_vl
   * instance. The operation will remove the the values in the section of the
   * vector.
   * @param first the first variable to remove
   * @param last the last variable to remove
   * @return an iterator to the element to the right of the elements removed
   */
  iterator
  erase (const_iterator first, const_iterator last)
  {
    auto first_ind = ind_of_itr (first);
    auto num_of_elem = std::distance (first, last);
    auto first_dist = std::distance (cbegin (), first);
    std::rotate (
        begin () + first_dist, begin () + first_dist + num_of_elem, end ());
    vector_size -= num_of_elem;
    if (memory_type == DYNAMIC && vector_size <= StaticCapacity)
      {
        elements_copy (stack_memory, dynamic_memory);
        delete[] dynamic_memory;
        dynamic_memory = nullptr;
        vector_cap = StaticCapacity;
        memory_type = STACK;
        return begin () + first_ind;
      }
    return begin () + first_ind;
  }

  /**
   * The action receives an iterator of the vector and removes the element it
   * points to.
   * @param position Iterator that points to a vector
   * @return an iterator to the element to the right of the element removed.
   */
  iterator erase (const_iterator position)
  {
    return erase (position, position + DEFAULT_ERASE_SIZE);
  }

  /**
   * The function removes the last element from the vector.
   */
  virtual void pop_back ()
  {
    if (vector_size != DEFAULT_VECTOR_SIZE)
      {
        vector_size--;
        if (memory_type == DYNAMIC && vector_size <= StaticCapacity)
          {
            elements_copy (stack_memory, dynamic_memory);
            delete[] dynamic_memory;
            memory_type = STACK;
            vector_cap = StaticCapacity;
          }
      }
  }

  /**
   * An function that removes all the vector elements.
   */
  virtual void clear ()
  {
    if (memory_type == DYNAMIC)
      {
        delete[] dynamic_memory;
        dynamic_memory = nullptr;
      }
    memory_type = STACK;
    vector_size = DEFAULT_VECTOR_SIZE;
    vector_cap = StaticCapacity;
  }

  /**
   * The function receives a T-variable and returns boolean value - Is the
   * value found in the vector?
   * @param element T-variable
   * @return True if element in vector else False
   */
  bool contains (const T &element) const
  {
    for (size_t index = 0; index < vector_size; ++index)
      {
        if (at (index) == element)
          {
            return true;
          }
      }
    return false;
  }

  /**
   * The operator will receive an index and will return the value associated
   * with it
   * @param index a number
   * @return value associated with the number
   */
  T operator[] (const size_t index) const noexcept
  {
    return data ()[index];
  }

  /**
   * The operator will receive an index and will return the value associated
   * with it
   * @param index a number
   * @return value associated with the number
   */
  T &operator[] (const size_t index) noexcept
  {
    return data ()[index];
  }

  /**
   * copies the other vector into the original vector
   * @param other the vector to copy from
   * @return the original and changed vector
   */
  vl_vector &operator= (const vl_vector &other)
  {
    if (this == &other)
      {
        return *this;
      }
    if (memory_type == DYNAMIC)
      {
        delete[] dynamic_memory;
      }
    this->vector_size = other.vector_size;
    this->vector_cap = other.vector_cap;
    this->memory_type = other.memory_type;
    if (memory_type == STACK)
      {
        elements_copy (data (), other.data ());
      }
    else
      {
        dynamic_memory = new T[vector_cap];
        elements_copy (data (), other.data ());
      }
    return *this;
  }

  /**
   * checks if the two vectors are equal
   * @param v the original vector
   * @return True if equal else False
   */
  bool operator== (const vl_vector<T, StaticCapacity> &v) const noexcept
  {
    if (vector_size != v.vector_size)
      {
        return false;
      }
    for (size_t index = 0; index < vector_size; ++index)
      {
        if (this->data ()[index] != v.data ()[index])
          {
            return false;
          }
      }
    return true;
  }

  /**
   * checks if the two vectors are unequal
   * @param v the original vector
   * @return False if equal else True
   */
  bool operator!= (const vl_vector<T, StaticCapacity> &v) const noexcept
  {
    return !(*this == v);
  }

 private:

  T *dynamic_memory;
  int memory_type;
  T stack_memory[StaticCapacity];
  size_t vector_size;
  size_t vector_cap;

  /**
   * copies the data from one memory to another
   * @param destination the memory to copy to
   * @param output thr memory to copy from
   */
  void elements_copy (T *destination, const T *output) noexcept
  {
    for (size_t index = 0; index < vector_size; ++index)
      {
        destination[index] = output[index];
      }
  }

  /**
   * checks if the the capacity needs to ce enlarged and calculates the new
   * capacity needed
   * @param cur_elements current number of elements in the vector
   * @param new_elements number of elements needed to be inserted
   * @return 0 if no resize needed, else the new capacity needed
   */
  size_t cap_c (size_t cur_elements, size_t new_elements) noexcept
  {
    if (cur_elements + new_elements <= StaticCapacity
        || cur_elements + new_elements <= vector_cap)
      {
        return 0;
      }
    return (size_t) ((3 * (cur_elements + new_elements)) / 2);
  }

  /**
   * enlarges the capacity
   * @param num_of_elements num of  new elements
   */
  void extend_capacity (size_t num_of_elements)
  {
    vector_cap = cap_c (vector_size, num_of_elements);
    T *temp = new T[vector_cap];
    for (size_t ind = 0; ind < vector_size; ++ind)
      {
        temp[ind] = at (ind);
      }
    delete[] dynamic_memory;
    dynamic_memory = temp;
    memory_type = DYNAMIC;
  }

  /**
   * returns the index of the Iterator in the vector
   * @param position an Iterator to be converted
   * @return the index of the Iterator
   */
  size_t ind_of_itr (const_iterator position) noexcept
  {
    for (size_t index = 0; index < vector_size; ++index)
      {
        if (&(at (index)) == position)
          {
            return index;
          }
      }
    return vector_size;
  }

};

#endif //_VL_VECTOR_H_
