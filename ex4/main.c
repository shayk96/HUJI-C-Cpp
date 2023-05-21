#include <stdio.h>
#include "vector.h"

void *elem_cpy (const void *elem)
{
  int *a = malloc (sizeof (int));
  *a = *((int *) elem);
  return a;
}
int elem_cmp (const void *elem_1, const void *elem_2)
{
  return *((const int *) elem_1) == *((const int *) elem_2);
}
void elem_free (void **elem)
{
  free (*elem);
}

void print_vector(vector *vec){
  printf ("\n\n");
  printf ("vector size = %zu\n", vec->size);
  printf ("vector capacity = %zu\n", vec->capacity);
  for (int i = 0; i < vec->size; i=i+3)
    {
      if (vec->size == i)
        break;
      printf ("index -> %d",i);
      if (i < 10)
        printf (" ");
      printf ("    ");
      printf ("value --> %d", *(int *) vec->data[i]);
      if (i < 10)
        printf (" ");
      printf ("    ");
      printf ("|");
      printf ("    ");
      if (vec->size == i+1)
        break;
      printf ("index -> %d",i+1);
      if (i < 9)
        printf (" ");
      printf ("    ");
      printf ("value --> %d", *(int *) vec->data[i+1]);
      if (i < 9)
        printf (" ");
      printf ("    ");
      printf ("|");
      printf ("    ");
      if (vec->size == i+2)
        break;
      printf ("index -> %d",i+2);
      printf ("    ");
      printf ("value --> %d\n", *(int *) vec->data[i+2]);
    }
}


/////
int main ()
{
  vector *vec = vector_alloc (elem_cpy, elem_cmp, elem_free);
  printf ("insert elements\n");
  for (int i = 0; i < 32; ++i)
    {
      vector_push_back (vec, &i);
    }
  print_vector (vec);
  printf ("\n\n---------------------------------------\n");
  printf ("deletes elements\n");
  for (int i = 0; i < 11; ++i)
    {
      vector_erase(vec, 12);

    }
  size_t y = 60;
  printf ("%d", vector_find (vec,&y));
  y = 6;
  int uu = *(int *)(vector_at (vec, y));
  printf ("\n%d\n", uu);
  print_vector (vec);
  printf ("\n\n---------------------------------------\n");
  printf ("clear vector:\n");
  vector_clear (vec);
  print_vector (vec);
  printf ("vector info:\n size = %zu , cap = %zu  \n", vec->size, vec->capacity);
  vector_free (&vec);
  return 0;
}