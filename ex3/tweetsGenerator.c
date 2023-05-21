#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS_IN_SENTENCE_GENERATION 20
#define MAX_WORD_LENGTH 100
#define MAX_SENTENCE_LENGTH 1000
#define SAME 0
#define TWEET_ITR "Tweet %d: "
#define USAGE_ERROR "Usage: Input should be <seed><number of tweets><path to \
tweets file><number of words to reads from file>"
#define ALOCATION_FAILURE "Allocation failure: Too much junk on the computer, \
free some space!"
#define FILE_ERROR "Error: File path is wrong!"
#define CORRECT_NUM_4 4
#define CORRECT_NUM_5 5
#define BASE 10

typedef struct WordStruct {
    char *word;
    int number_of_occurrence;
    int prob_list_size;
    int prob_list_size_with_duplicats;
    struct WordProbability *prob_list;
} WordStruct;

typedef struct WordProbability {
    struct WordStruct *word_struct_ptr;
    int num_of_occurrnces;

} WordProbability;

/************ LINKED LIST ************/
typedef struct Node {
    WordStruct *data;
    struct Node *next;
} Node;

typedef struct LinkList {
    Node *first;
    Node *last;
    int size;
} LinkList;

/**
 * Add data to new node at the end of the given link list.
 * @param link_list Link list to add data to
 * @param data pointer to dynamically allocated data
 * @return 0 on success, 1 otherwise
 */
int add (LinkList *link_list, WordStruct *data)
{
  Node *new_node = malloc (sizeof (Node));
  if (new_node == NULL)
    {
      return 1;
    }
  *new_node = (Node) {data, NULL};

  if (link_list->first == NULL)
    {
      link_list->first = new_node;
      link_list->last = new_node;
    }
  else
    {
      link_list->last->next = new_node;
      link_list->last = new_node;
    }

  link_list->size++;
  return 0;
}

/*************************************/
int dot_at_end (WordStruct *prev_word);

void
load_word (LinkList *dictionary, WordStruct *temp_word, char *word,
           int new_word);

WordStruct *check_word (LinkList *dictionary, char *word);

WordStruct *memory_allocation (WordStruct *word, int word_or_prob_list);

/**
 * Get random number between 0 and max_number [0, max_number).
 * @param max_number
 * @return Random number
 */
int get_random_number (int max_number)
{
  int number = rand () % max_number;
  return number;
}

/**
 * Choose randomly the next word from the given dictionary, drawn uniformly.
 * The function won't return a word that end's in full stop '.' (Nekuda).
 * @param dictionary Dictionary to choose a word from
 * @return WordStruct of the chosen word
 */
WordStruct *get_first_random_word (LinkList *dictionary)
{
  struct Node *temp = dictionary->first;
  int word_number = get_random_number (dictionary->size);
  while (1 != 0)
    {
      for (int i = 0; i < word_number; ++i)
        {
          temp = temp->next;
        }
      if (dot_at_end (temp->data) == 1)
        {
          break;
        }
      word_number = get_random_number (dictionary->size);
      temp = dictionary->first;
    }
  return temp->data;
}

/**
 * Choose randomly the next word. Depend on it's occurrence frequency
 * in word_struct_ptr->WordProbability.
 * @param word_struct_ptr WordStruct to choose from
 * @return WordStruct of the chosen word
 */
WordStruct *get_next_random_word (WordStruct *word_struct_ptr)
{
  int word_count = 0;
  int number = get_random_number (word_struct_ptr->
      prob_list_size_with_duplicats);
  WordProbability *temp = word_struct_ptr->prob_list;
  for (int i = 0; i < number; ++i)
    {
      word_count += 1;
      if (word_count >= temp->num_of_occurrnces)
        {
          word_count = 0;
          temp++;
        }
    }
  return temp->word_struct_ptr;
}

/**
 * Receive dictionary, generate and print to stdout random sentence out of it.
 * The sentence most have at least 2 words in it.
 * @param dictionary Dictionary to use
 * @return Amount of words in printed sentence
 */
int generate_sentence (LinkList *dictionary)
{
  WordStruct *temp = get_first_random_word (dictionary);
  int num_of_words = 1;
  while (num_of_words <= MAX_WORDS_IN_SENTENCE_GENERATION)
    {
      printf ("%s ", temp->word);
      temp = get_next_random_word (temp);
      num_of_words++;
      if (dot_at_end (temp) == 0)
        {
          printf ("%s", temp->word);
          break;
        }
    }
  printf ("\n");
  return num_of_words;
}

/**
 * This function calls the generate sentence function as many times as the
 * user wants
 * @param num_of_tweets number of sentences to generate
 * @param dictionary holds the words to create sentences from
 */
void create_tweets (int num_of_tweets, LinkList *dictionary)
{
  for (int i = 1; i <= num_of_tweets; ++i)
    {
      printf (TWEET_ITR, i);
      generate_sentence (dictionary);
    }
}

/**
 * This function checks if the next word already in the probability list of
 * the current word
 * @param curr_word the current word
 * @param next_word the next word
 * @return pointer to word if the word exists else NULL
 */
WordProbability *check_if_exists (WordStruct *curr_word, WordStruct *next_word)
{
  WordProbability *temp = curr_word->prob_list;
  int size = 0;
  while (temp != NULL && size < curr_word->prob_list_size)
    {
      if (strcmp (temp->word_struct_ptr->word, next_word->word) == 0)
        {
          return temp;
        }
      size++;
      temp++;
    }
  return NULL;
}

/**
 * Gets 2 WordStructs. If second_word in first_word's prob_list,
 * update the existing probability value.
 * Otherwise, add the second word to the prob_list of the first word.
 * @param first_word
 * @param second_word
 * @return 0 if already in list, 1 otherwise.
 */
int
add_word_to_probability_list (WordStruct *first_word, WordStruct *second_word)
{
  WordProbability *pos = check_if_exists (first_word, second_word);
  if (first_word->prob_list == NULL)
    {
      memory_allocation (first_word, 1);
      first_word->prob_list->word_struct_ptr = second_word;
      first_word->prob_list->num_of_occurrnces = 1;
      first_word->prob_list_size = 1;
      first_word->prob_list_size_with_duplicats = 1;
      return 1;
    }
  if (first_word->prob_list != NULL && pos == NULL)
    {
      memory_allocation (first_word, 2);
      first_word->prob_list[first_word->prob_list_size].word_struct_ptr =
          second_word;
      first_word->prob_list[first_word->prob_list_size].num_of_occurrnces = 1;
      first_word->prob_list_size += 1;
      first_word->prob_list_size_with_duplicats += 1;
      return 0;
    }
  pos->num_of_occurrnces += 1;
  first_word->prob_list_size_with_duplicats += 1;
  return 1;

}

/**
 * Read word from the given file. Add every unique word to the dictionary.
 * Also, at every iteration, update the prob_list of the previous word with
 * the value of the current word.
 * @param fp File pointer
 * @param words_to_read Number of words to read from file.
 *                      If value is bigger than the file's word count,
 *                      or if words_to_read == -1 than read entire file.
 * @param dictionary Empty dictionary to fill
 */
void fill_dictionary (FILE *fp, int words_to_read, LinkList *dictionary)
{
  char *word = NULL;
  char line[MAX_SENTENCE_LENGTH];
  int word_read = 0;
  WordStruct *temp_word = NULL;
  WordStruct *prev_word = NULL;
  while (fgets (line, MAX_SENTENCE_LENGTH, fp) != NULL)
    {
      //line[strcspn (line, "\n")] = 0;
      word = strtok (line, " \n");
      while (word_read != words_to_read)
        {
          temp_word = check_word (dictionary, word);
          if (temp_word == NULL)
            {
              temp_word = memory_allocation (NULL, 0);
              load_word (dictionary, temp_word, word, 1);
              word_read++;
            }
          else
            {
              load_word (dictionary, temp_word, word, 0);
              word_read++;
            }
          if (dot_at_end (prev_word) == 1)
            {
              add_word_to_probability_list (prev_word, temp_word);
            }
          prev_word = temp_word;
          word = strtok (NULL, " \n");
          if (word == NULL)
            {
              prev_word = NULL;
              temp_word = NULL;
              break;
            }
        }
    }
}

/**
 * checks if there is a dot at the end of the world
 * @param prev_word the word to check
 * @return 0 if ends with a dot else 1
 */
int dot_at_end (WordStruct *prev_word)
{
  if (prev_word == NULL)
    {
      return 0;
    }
  int dot_loc = strlen (prev_word->word) - 1;
  if (strcmp (&prev_word->word[dot_loc], ".") == SAME)
    {
      return 0;
    }
  return 1;
}

/**
 * handles the memory allocation for the program
 * @param word the WordStruct to allocate memory for
 * @param word_or_prob_list a flag determining the memory type
 * @return a pointer to the WordStruct
 */
WordStruct *memory_allocation (WordStruct *word, int word_or_prob_list)
{
  if (word_or_prob_list == 0)
    {
      WordStruct *temp_word = (WordStruct *) calloc (1,
                                                     sizeof (WordStruct));
      if (temp_word == NULL)
        {
          printf(ALOCATION_FAILURE);
          exit (EXIT_FAILURE);
        }
      temp_word->word = (char *) calloc (1, MAX_WORD_LENGTH);
      if (temp_word->word == NULL)
        {
          printf(ALOCATION_FAILURE);
          exit (EXIT_FAILURE);
        }
      return temp_word;
    }
  if (word_or_prob_list == 1)
    {
      word->prob_list = (WordProbability *) calloc (1,
                                                    sizeof (WordProbability));
      if (word->prob_list == NULL)
        {
          printf(ALOCATION_FAILURE);
          exit (EXIT_FAILURE);
        }
    }
  if (word_or_prob_list == 2)
    {
      word->prob_list = (WordProbability *) realloc (word->prob_list,
                                                     (word->prob_list_size + 1)
                                                     * sizeof (WordProbability));
      if (word->prob_list == NULL)
        {
          printf(ALOCATION_FAILURE);
          exit (EXIT_FAILURE);
        }
    }
  return 0;
}

/**
 * loads the word to the dictionary and copies the word to the WordStruct
 * @param dictionary the dictionary
 * @param temp_word the WordStruct
 * @param word the word
 * @param new_word a flag telling the function if to add to the dictionary or
 * not
 */
void
load_word (LinkList *dictionary, WordStruct *temp_word, char *word,
           int new_word)
{
  strcpy (temp_word->word, word);
  temp_word->number_of_occurrence += 1;
  if (new_word == 1)
    {
      add (dictionary, temp_word);
    }
}

/**
 * checks if the word already exists in the dictionary
 * @param dictionary the dictionary
 * @param word the word being checks
 * @return a pointer to the relevant WordStruct else NULL
 */
WordStruct *check_word (LinkList *dictionary, char *word)
{
  Node *temp = dictionary->first;
  while (temp != NULL)
    {
      if (strcmp (temp->data->word, word) == SAME)
        {
          return temp->data;
        }
      temp = temp->next;
    }
  return NULL;
}

/**
 * Free the given dictionary and all of it's content from memory.
 * @param dictionary Dictionary to free
 */
void free_dictionary (LinkList *dictionary)
{
  Node *temp = dictionary->first;
  Node *temp2 = NULL;
  while (temp != NULL)
    {
      free (temp->data->prob_list);
      free (temp->data->word);
      free (temp->data);
      temp2 = temp;
      temp = temp->next;
      free (temp2);
    }
  free (dictionary);
}

int check_input (int size)
{
  if (size != CORRECT_NUM_4 && size != CORRECT_NUM_5)
    {
      printf (USAGE_ERROR);
      return EXIT_FAILURE;
    }
  if (size == CORRECT_NUM_5)
    {
      return CORRECT_NUM_5;
    }
  else
    {
      return CORRECT_NUM_4;
    }
}

/**
 * @param argc
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 *             3) Path to file
 *             4) Optional - Number of words to read
 */
int main (int argc, char *argv[])
{
  int check_inputs = check_input(argc);
  if (check_inputs != CORRECT_NUM_4 && check_inputs != CORRECT_NUM_5)
    {
      return EXIT_FAILURE;

    }
  char *ptr = NULL;
  int seed = strtol (argv[1], &ptr, BASE);
  int num_of_tweets = strtol (argv[2], &ptr, BASE);
  FILE *fp = fopen (argv[3], "r");
  if (fp == NULL)
    {
      printf (FILE_ERROR);
      return EXIT_FAILURE;
    }
  if (check_inputs == CORRECT_NUM_4)
    {
      LinkList *dictionary = (LinkList *) calloc (1,sizeof (LinkList));
      if (dictionary == NULL)
        {
          return EXIT_FAILURE;
        }
      fill_dictionary (fp, -1, dictionary);
      fclose(fp);
      srand (seed);
      create_tweets (num_of_tweets, dictionary);
      free_dictionary (dictionary);
      return 0;
    }
  if (check_inputs == CORRECT_NUM_5)
    {
      LinkList *dictionary = (LinkList *) calloc (1, sizeof (LinkList));
      if (dictionary == NULL)
        {
          return EXIT_FAILURE;
        }
      int words_to_read = strtol (argv[4], &ptr, BASE);
      fill_dictionary (fp, words_to_read, dictionary);
      fclose(fp);
      srand (seed);
      create_tweets (num_of_tweets, dictionary);
      free_dictionary (dictionary);
      return 0;
    }
  return EXIT_FAILURE;
}