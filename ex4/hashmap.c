#include "hashmap.h"

#define INCREASE 1
#define DECREASE 2

/**
 * Allocates dynamically new hash map element.
 * @param func a function which "hashes" keys.
 * @return pointer to dynamically allocated hashmap.
 * @if_fail return NULL.
 */
hashmap *hashmap_alloc (hash_func func)
{
  if (func == NULL)
    {
      return NULL;
    }
  hashmap *map = malloc (sizeof (hashmap));
  if (map == NULL)
    {
      return NULL;
    }
  map->capacity = HASH_MAP_INITIAL_CAP;
  map->size = 0;
  map->hash_func = func;
  map->buckets = calloc (1, sizeof (vector) * map->capacity);
  if (map->buckets == NULL)
    {
      free (map);
      return NULL;
    }
  return map;
}

/**
 * Frees a hash map and the elements the hash map itself allocated.
 * @param p_hash_map pointer to dynamically allocated pointer to hash_map.
 */
void hashmap_free (hashmap **p_hash_map)
{
  for (size_t ind = 0; ind < (*p_hash_map)->capacity; ++ind)
    {
      if ((*p_hash_map)->buckets[ind] != NULL)
        {
          vector_free (&(*p_hash_map)->buckets[ind]);
          (*p_hash_map)->buckets[ind] = NULL;
        }
    }
  free ((*p_hash_map)->buckets);
  (*p_hash_map)->buckets = NULL;
  free (*p_hash_map);
  *p_hash_map = NULL;
}

/**
 * frees the hashmap buckets
 * @param buckets the buckets holding vectors
 * @param capacity number of buckets
 */
void free_buckets (vector **buckets, size_t capacity)
{
  for (size_t ind = 0; ind < capacity; ++ind)
    {
      if (buckets[ind] != NULL)
        {
          vector_free (&buckets[ind]);
          buckets[ind] = NULL;
        }
    }
  free (buckets);
}
/**
 * hashes the key to a number
 * @param hash_map the hash table
 * @param key the pair key
 * @return a size_t number
 */
size_t get_hash_index (const hashmap *hash_map, const_keyT key)
{
  return (hash_map->hash_func (key)) & (hash_map->capacity - 1);
}

/**
 * rolls back the changes made before a failure happened
 * @param new_buckets the new buckets
 * @param hash_map the hase table
 * @param flag a number indicating what changes to do
 */
void rollback_changes (vector **new_buckets, hashmap *hash_map, int flag)
{
  free_buckets (new_buckets, hash_map->capacity);
  if (flag == 1)
    {
      hash_map->capacity = hash_map->capacity / 2;
    }
  if (flag == 2)
    {
      hash_map->capacity = hash_map->capacity * 2;
    }
}

/**
 * copies the old table to the new table
 * @param hash_map the hash table
 * @param ind the bucket
 * @param vec_ind the vector
 * @param new_buckets the new buckets
 * @param flag tells the function which function called her and what to do
 * @return 0 if fail else success
 */
int rehash_helper (hashmap *hash_map, size_t ind, size_t vec_ind,
                                              vector **new_buckets, int flag)
{
  pair *temp = vector_at (hash_map->buckets[ind], vec_ind);
  size_t key = get_hash_index (hash_map, temp->key);
  if (new_buckets[key] == NULL)
    {
      new_buckets[key] = vector_alloc (pair_copy, pair_cmp, pair_free);
      if (new_buckets[key] == NULL)
        {
          rollback_changes (new_buckets, hash_map, flag);
          return 0;
        }
    }
  if (vector_push_back (new_buckets[key], temp) == 0)
    {
      rollback_changes (new_buckets, hash_map, flag);
      return 0;
    }
  return 1;
}

/**
 * rehashes the hash table upon enlargement
 * @param hash_map the hase table
 * @return a new table if successful else NULL
 */
vector **rehash_increase (hashmap *hash_map)
{
  hash_map->capacity = hash_map->capacity * 2;
  vector **new_buckets = calloc (1, sizeof (vector) * hash_map->capacity);
  if (new_buckets == NULL)
    {
      hash_map->capacity = hash_map->capacity / 2;
      return NULL;
    }
  for (size_t ind = 0; ind < hash_map->capacity; ind++)
    {
      if (hash_map->buckets[ind] != NULL)
        {
          for (size_t vec_ind = 0;
               vec_ind < hash_map->buckets[ind]->size; vec_ind++)
            {
              if (rehash_helper (hash_map, ind, vec_ind, new_buckets,
                                                            INCREASE) == 0)
                {
                  return NULL;
                }
            }
        }
    }
  free_buckets (hash_map->buckets, hash_map->capacity / 2);
  return new_buckets;
}

/**
 * rehashes the hash table upon reduction
 * @param hash_map the hase table
 * @return a new table if successful else NULL
 */
vector **rehash_decrease (hashmap *hash_map)
{
  hash_map->capacity = hash_map->capacity / 2;
  vector **new_buckets = calloc (1, sizeof (vector) * hash_map->capacity);
  if (new_buckets == NULL)
    {
      hash_map->capacity = hash_map->capacity * 2;
      return NULL;
    }
  for (size_t ind = 0; ind < hash_map->capacity * 2; ind++)
    {
      if (hash_map->buckets[ind] != NULL)
        {
          for (size_t vec_ind = 0;
               vec_ind < hash_map->buckets[ind]->size; vec_ind++)
            {
              if (rehash_helper (hash_map, ind, vec_ind, new_buckets,
                                                                DECREASE) == 0)
                {
                  return NULL;
                }
            }
        }
    }
  free_buckets (hash_map->buckets, hash_map->capacity * 2);
  return new_buckets;
}

/**
 * Inserts a new in_pair to the hash map.
 * The function inserts *new*, *copied*, *dynamically allocated* in_pair,
 * NOT the in_pair it receives as a parameter.
 * @param hash_map the hash map to be inserted with new element.
 * @param in_pair a in_pair the hash map would contain.
 * @return returns 1 for successful insertion, 0 otherwise.
 */
int hashmap_insert (hashmap *hash_map, const pair *in_pair)
{
  if (hash_map == NULL || in_pair == NULL)
    {
      return 0;
    }
  if (hashmap_at (hash_map, in_pair->key) != NULL)
    {
      return 0;
    }
  hash_map->size++;
  if (hashmap_get_load_factor (hash_map) > HASH_MAP_MAX_LOAD_FACTOR)
    {
      vector **new_buckets = rehash_increase (hash_map);
      if (new_buckets == NULL)
        {
          hash_map->size--;
          return 0;
        }
      hash_map->buckets = new_buckets;
    }
  size_t key = get_hash_index (hash_map, in_pair->key);
  if (hash_map->buckets[key] == NULL)
    {
      hash_map->buckets[key] = vector_alloc (pair_copy, pair_cmp, pair_free);
      if (hash_map->buckets[key] == NULL)
        {
          hash_map->size--;
          return 0;
        }
    }
  if (vector_push_back (hash_map->buckets[key], in_pair) == 0)
    {
      hash_map->size--;
      return 0;
    }
  return 1;
}

/**
 * The function returns the value associated with the given key.
 * @param hash_map a hash map.
 * @param key the key to be checked.('C',2)
 * @return the value associated with key if exists, NULL otherwise
 * (the value itself,
 * not a copy of it).
 */
valueT hashmap_at (const hashmap *hash_map, const_keyT key)
{
  if (hash_map == NULL || key == NULL)
    {
      return NULL;
    }
  vector *temp = hash_map->buckets[get_hash_index (hash_map, key)];
  if (temp == NULL)
    {
      return NULL;
    }
  for (size_t ind = 0; ind < temp->size; ++ind)
    {
      pair *temp2 = vector_at (temp, ind);
      if (temp2->key_cmp (temp2->key, key) == 1)
        {
          return temp2->value;
        }
    }
  return NULL;
}

void free_vector (hashmap *hash_map, size_t key_ind)
{
  vector_free (&hash_map->buckets[key_ind]);
  hash_map->buckets[key_ind] = NULL;
  hash_map->size--;
}

/**
 * checks if the table needs rehashing and if yes does it
 * @param hash_map the hash table
 * @return 1 if success else 0
 */
int pos_rehash(hashmap *hash_map)
{
  if (hashmap_get_load_factor (hash_map) < HASH_MAP_MIN_LOAD_FACTOR)
    {
      vector **new_buckets = rehash_decrease (hash_map);
      if (new_buckets == NULL)
        {
          return 0;
        }
      hash_map->buckets = new_buckets;
    }
  return 1;
}

/**
 * The function erases the pair associated with key.
 * @param hash_map a hash map.
 * @param key a key of the pair to be erased.
 * @return 1 if the erasing was done successfully, 0 otherwise. (if key not
 * in map, considered fail).
 */
int hashmap_erase (hashmap *hash_map, const_keyT key)
{
  if (hash_map == NULL || key == NULL)
    {
      return 0;
    }
  size_t key_ind = get_hash_index (hash_map, key);
  vector *temp = hash_map->buckets[key_ind];
  if (temp == NULL)
    {
      return 0;
    }
  for (size_t ind = 0; ind < temp->size; ++ind)
    {
      pair *temp2 = vector_at (temp, ind);
      if (temp2->key_cmp (temp2->key, key) == 1)
        {
          if (vector_erase (temp, ind) == 1 && temp->size == 0)
            {
              free_vector (hash_map, key_ind);
              return pos_rehash (hash_map);
            }
        }
    }
  return 0;
}

/**
 * This function returns the load factor of the hash map.
 * @param hash_map a hash map.
 * @return the hash map's load factor, -1 if the function failed.
 */
double hashmap_get_load_factor (const hashmap *hash_map)
{
  if (hash_map == NULL || hash_map->capacity == 0)
    {
      return -1;
    }
  return (double) hash_map->size / (double) hash_map->capacity;
}

/**
 * This function receives a hashmap and 2 functions, the first checks a
 * condition on the keys,
 * and the seconds apply some modification on the values. The function should
 * apply the modification
 * only on the values that are associated with keys that meet the condition.
 * Example: if the hashmap maps char->int, keyT_func checks if the char is a
 * capital letter (A-Z),
 * and val_t_func multiples the number by 2, hashmap_apply_if will change the
 * map: {('C',2),('#',3),('X',5)}, to: {('C',4),('#',3),('X',10)}, and the
 * return value will be 2.
 * @param hash_map a hashmap
 * @param keyT_func a function that checks a condition on keyT and
 * return 1 if true, 0 else
 * @param valT_func a function that modifies valueT, in-place
 * @return number of changed values
 */
int hashmap_apply_if (const hashmap *hash_map, keyT_func keyT_func,
                      valueT_func valT_func)
{
  if (hash_map == NULL || keyT_func == NULL || valT_func == NULL)
    {
      return -1;
    }
  int operated_on = 0;
  for (size_t ind = 0; ind < hash_map->capacity; ind++)
    {
      if (hash_map->buckets[ind] != NULL)
        {
          for (size_t vec_ind = 0;
               vec_ind < hash_map->buckets[ind]->size; ++vec_ind)
            {
              if (keyT_func (((pair *) hash_map->buckets[ind]->data[vec_ind])
                                 ->key) == 1)
                {
                  valT_func (((pair *) hash_map->buckets[ind]->data[vec_ind])
                                 ->value);
                  operated_on++;
                }
            }
        }
    }
  return operated_on;
}