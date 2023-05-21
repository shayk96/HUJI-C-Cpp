#include "vector.h"

#define DECREASE 1
#define INCREASE 2

/**
 * reallocates memory in accordance to the specifications given
 * @param vector pointer to a vector
 * @param flag a number indicating which operation to do
 * @return 1 if successful else 1
 */
int memory_realocation (vector *vector, int flag)
{
  if (flag == DECREASE
      && vector_get_load_factor (vector) < VECTOR_MIN_LOAD_FACTOR)
    {
      vector->capacity = vector->capacity / 2;
      vector->data = realloc (vector->data,
                              sizeof (vector->data) * vector->capacity);
      if (vector->data == NULL)
        {
          vector->size++;
          vector->capacity = vector->capacity * 2;
          return 0;
        }
      return 1;
    }
  if (flag == INCREASE
      && vector_get_load_factor (vector) > VECTOR_MAX_LOAD_FACTOR)
    {
      vector->capacity = vector->capacity * 2;
      vector->data = realloc (vector->data,
                              sizeof (vector->data) * vector->capacity);
      if (vector->data == NULL)
        {
          vector->size--;
          vector->capacity = vector->capacity / 2;
          return 0;
        }
    }
  return 1;
}

/**
 * Dynamically allocates a new vector.
 * @param elem_copy_func func which copies the element stored in the vector
 * (returns dynamically allocated copy).
 * @param elem_cmp_func func which is used to compare elements stored in
 * the vector.
 * @param elem_free_func func which frees elements stored in the vector.
 * @return pointer to dynamically allocated vector.
 * @if_fail return NULL.
 */
vector *vector_alloc (vector_elem_cpy elem_copy_func, vector_elem_cmp
                                elem_cmp_func, vector_elem_free elem_free_func)
{
  vector *vec = malloc (sizeof *vec);
  if (vec == NULL)
    {
      return NULL;
    }
  if ((elem_copy_func == NULL) || (elem_cmp_func == NULL)
      || (elem_free_func == NULL))
    {
      return NULL;
    }
  vec->capacity = VECTOR_INITIAL_CAP;
  vec->size = 0;
  vec->data = malloc (sizeof (*vec->data) * vec->capacity);
  if (vec->data == NULL)
    {
      free (vec);
      return NULL;
    }
  vec->elem_copy_func = elem_copy_func;
  vec->elem_cmp_func = elem_cmp_func;
  vec->elem_free_func = elem_free_func;
  return vec;
}

/**
 * Frees a vector and the elements the vector itself allocated.
 * @param p_vector pointer to dynamically allocated pointer to vector.
 */
void vector_free (vector **p_vector)
{
  if (p_vector != NULL)
    {
      if ((*p_vector)->size != 0)
        {
          vector_clear (*p_vector);
        }
      free ((*p_vector)->data);
      (*p_vector)->data = NULL;
      free (*p_vector);
      *p_vector = NULL;
    }
}

/**
 * Returns the element at the given index.
 * @param vector pointer to a vector.
 * @param ind the index of the element we want to get.
 * @return the element at the given index if exists (the element itself,
 * not a copy of it),
 * NULL otherwise.
 */
void *vector_at (const vector *vector, size_t ind)
{
  if (ind >= vector->size || vector == NULL)
    {
      return NULL;
    }
  return vector->data[ind];
}

/**
 * Gets a value and checks if the value is in the vector.
 * @param vector a pointer to vector.
 * @param value the value to look for.
 * @return the index of the given value if it is in the
 * vector ([0, vector_size - 1]).
 * Returns -1 if no such value in the vector.
 */
int vector_find (const vector *vector, const void *value)
{
  if (vector == NULL || value == NULL)
    {
      return -1;
    }
  for (size_t ind = 0; ind < vector->size; ind++)
    {
      if (vector->elem_cmp_func (vector->data[ind], value))
        {
          return ind;
        }
    }
  return -1;
}

/**
 * Adds a new value to the back (index vector_size) of the vector.
 * @param vector a pointer to vector.
 * @param value the value to be added to the vector.
 * @return 1 if the adding has been done successfully, 0 otherwise.
 */
int vector_push_back (vector *vector, const void *value)
{
  if (vector == NULL || value == NULL)
    {
      return 0;
    }
  vector->size++;
  if (memory_realocation (vector, INCREASE) == 1)
    {
      vector->data[vector->size - 1] = vector->elem_copy_func (value);
      return 1;
    }
  return 0;
}

/**
 * This function returns the load factor of the vector.
 * @param vector a vector.
 * @return the vector's load factor, -1 if the function failed.
 */
double vector_get_load_factor (const vector *vector)
{
  return (double) vector->size / (double) vector->capacity;
}

/**
 * Removes the element at the given index from the vector. alters the indices
 * of the remaining
 * elements so that there are no empty indices in the
 * range [0, size-1] (inclusive).
 * @param vector a pointer to vector.
 * @param ind the index of the element to be removed.
 * @return 1 if the removing has been done successfully, 0 otherwise.
 */
int vector_erase (vector *vector, size_t ind)
{
  if (ind >= vector->size || vector == NULL)
    {
      return 0;
    }
  vector->size--;
  if (memory_realocation (vector, DECREASE) == 1)
    {
      vector->elem_free_func (&vector->data[ind]);
      for (; ind < vector->size; ind++)
        {
          if (vector->data[ind + 1] != NULL)
            {
              vector->data[ind] = vector->data[ind + 1];
            }
        }
      vector->data[vector->size] = NULL;
      return 1;
    }
  return 0;
}

/**
 * Deletes all the elements in the vector.
 * @param vector vector a pointer to vector.
 */
void vector_clear (vector *vector)
{
  int ind = (int) vector->size-1;
  while (ind >= 0)
    {
      vector_erase (vector,ind);
      ind -= 1;
    }
}