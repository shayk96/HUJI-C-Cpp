#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define BAD_INPUT 0
#define GOOD_INPUT 1
#define SAME 0
#define COMMANDS {"check", "encode", "decode"}
#define MAX_KEY 25
#define MIN_KEY -25
#define NUM_OF_LETTERS 26
#define ENCODE_DECODE 0
#define CHECK 1
#define ENCODE_COMM "encode"
#define DECODE_COMM "decode"
#define CHECK_COMM "check"
#define NUM_OF_ARGS_CHECK 4
#define NUM_OF_ARGS_EN_DE 5
#define MIN_CAP_LETTERS 65
#define MAX_CAP_LETTERS 90
#define MIN_LETTERS 97
#define MAX_LETTERS 122
#define MAX_SIZE 7
#define ERROR_COMMAND "The given command is invalid\n"
#define ERROR_CHECK "Usage: cipher <check> <source path file> <output path \
file>\n"
#define ERROR_EN_DE "Usage: cipher <encode|decode> <k> <source path file> \
<output path file>\n"
#define INVALID_ENC  "Invalid encrypting\n"
#define VALID_ENC "Valid encrypting with k = %i\n"
#define ERROR_FILE "The given file is invalid\n"

/**
 * This function checks if the number of arguments is right and if the command
 * is right and prints an informative massage to the user
 * @param size : The number of arguments given
 * @param inputs : The commands given
 * @return : EXIT_SUCCESS if everything OK else returns EXIT_FAILURE
 */
int check_input (int size, char *inputs[])
{
  int right_input = BAD_INPUT;
  char commands[3][MAX_SIZE] = COMMANDS;
  if (strcmp (inputs[1], commands[0]) == SAME
      || (strcmp (inputs[1], commands[1]) == SAME)
      || (strcmp (inputs[1], commands[2])) == SAME)
    {
      right_input = GOOD_INPUT;
    }
  if (right_input == BAD_INPUT || size == 1 || size == 0)
    {
      fprintf (stderr, ERROR_COMMAND);
      return EXIT_FAILURE; // bad input
    }
  if (strcmp (inputs[1], commands[0]) == SAME && size != NUM_OF_ARGS_CHECK)
    {
      fprintf (stderr, ERROR_CHECK);
      return EXIT_FAILURE;
    }
  if ((strcmp (inputs[1], commands[1]) == SAME
       || strcmp (inputs[1], commands[2]) == SAME)
      && size != NUM_OF_ARGS_EN_DE)
    {
      fprintf (stderr, ERROR_EN_DE);
      return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}

/**
 * This function helps the encode function
 * @param chr : the letter to encrypt or decrypt
 * @param key : the encryption key
 * @return : The encrypted or decrypted letter
 */
int encode_helper (int chr, int key)
{
  int new_chr;
  if (key >= MAX_KEY || key <= MIN_KEY)
    {
      key %= NUM_OF_LETTERS;
    }
  if ((MIN_LETTERS <= chr && chr <= MAX_LETTERS))
    {
      new_chr = chr + key;
      if (MAX_LETTERS < new_chr)
        {
          new_chr -= NUM_OF_LETTERS;
        }
      if (new_chr < MIN_LETTERS)
        {
          new_chr += NUM_OF_LETTERS;
        }
      return new_chr;
    }
  if ((MIN_CAP_LETTERS <= chr && chr <= MAX_CAP_LETTERS))
    {
      new_chr = chr + key;
      if (MAX_CAP_LETTERS < new_chr)
        {
          new_chr -= NUM_OF_LETTERS;
        }
      if (new_chr < MIN_CAP_LETTERS)
        {
          new_chr += NUM_OF_LETTERS;
        }
      return new_chr;
    }
  else
    {
      return chr;
    }
}

/**
 * This function handles the encoding and decoding phase
 * @param file_in : the file to encode or decode
 * @param file_out : the file to write the result to
 * @param key : the encryption key
 */
void encode (FILE *file_in, FILE *file_out, int key)
{
  int chr, new_chr;
  while ((chr = fgetc (file_in)) != EOF)
    {
      new_chr = encode_helper (chr, key);
      fputc (new_chr, file_out);
    }
}

/**
 * The function checks if the characters given in the Check function are legal
 * @param chr_1 : The first character
 * @param chr_2 : The second character
 * @return EXIT_FAILURE if the characters are illegal and not the same, if
 * the same EXIT_SUCCESS
 */
int check_signs (int chr_1, int chr_2)
{
  if ((MIN_LETTERS <= chr_1 && chr_1 <= MAX_LETTERS)
      && (MIN_CAP_LETTERS <= chr_2 && chr_2 <= MAX_CAP_LETTERS))
    {
      return EXIT_FAILURE;
    }
  if ((MIN_LETTERS <= chr_2 && chr_2 <= MAX_LETTERS)
      && (MIN_CAP_LETTERS <= chr_1 && chr_1 <= MAX_CAP_LETTERS))
    {
      return EXIT_FAILURE;
    }
  if (((MAX_LETTERS < chr_1 || chr_1 < MIN_CAP_LETTERS)
       || (MAX_CAP_LETTERS < chr_1 && chr_1 < MIN_LETTERS))
      && ((MAX_LETTERS < chr_2 || chr_2 < MIN_CAP_LETTERS)
          || (MAX_CAP_LETTERS < chr_2 && chr_2 < MIN_LETTERS)))
    {
      if (chr_1 != chr_2)
        {
          return EXIT_FAILURE;
        }
      else
        {
          return 2;
        }
    }
  if ((MIN_LETTERS <= chr_1 && chr_1 <= MAX_LETTERS)
      && ((MAX_LETTERS < chr_2 || chr_2 < MIN_CAP_LETTERS)
          || (MAX_CAP_LETTERS < chr_2 && chr_2 < MIN_LETTERS)))
    {
      return EXIT_FAILURE;
    }
  if ((MIN_LETTERS <= chr_2 && chr_2 <= MAX_LETTERS)
      && ((MAX_LETTERS < chr_1 || chr_1 < MIN_CAP_LETTERS)
          || (MAX_CAP_LETTERS < chr_1 && chr_1 < MIN_LETTERS)))
    {
      return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}

/**
 * This function compares between files and check if there exists an encryption
 * key
 * @param file_1 : the first file
 * @param file_2 : the second file
 * @return encryption key if there exists one else EXIT_FAILURE
 */
int check_code (FILE *file_1, FILE *file_2)
{
  int pos_key, chr_1, chr_2, chr_3, flag;
  flag = 0;
  while (chr_1 != EOF || chr_2 != EOF)
    {
      chr_1 = fgetc (file_1);
      chr_2 = fgetc (file_2);
      if (check_signs (chr_1, chr_2) == EXIT_SUCCESS && flag == 0)
        {
          pos_key = chr_2 - chr_1;
          flag = 1;
          continue;
        }
      if (check_signs (chr_1, chr_2) == 2)
        {
          continue;
        }
      if (check_signs (chr_1, chr_2) == EXIT_FAILURE)
        {
          fprintf (stdout, INVALID_ENC);
          return EXIT_SUCCESS;
        }
      chr_3 = encode_helper (chr_1, pos_key);
      if (chr_2 != chr_3)
        {
          fprintf (stdout, INVALID_ENC);
          return EXIT_SUCCESS;
        }
    }
  if (pos_key < 0)
    {
      pos_key += NUM_OF_LETTERS;
    }
  fprintf (stdout, VALID_ENC, pos_key);
  return EXIT_SUCCESS;
}

/**
 * This function checks if the files path is right and prints
 * an informative massage to the user, and calls the right function to
 * encode or decode or check
 * @param file_path_in : The file to read from or to check
 * @param file_path_out : The file to write to or to check
 * @param key : The encryption key
 * @param command : A number symbolizing the command given
 * @return : EXIT_SUCCESS if everything OK else returns EXIT_FAILURE
 */
int file_handler (const char *file_path_in, const char *file_path_out, int key,
                  int command)
{
  FILE *work_file_in;
  FILE *work_file_out;
  if (command == CHECK)
    {
      work_file_out = fopen (file_path_out, "r");
      if (work_file_out == NULL)
        {
          fprintf (stderr, ERROR_FILE);
          return EXIT_FAILURE;
        }
    }
  if (command == ENCODE_DECODE)
    {
      work_file_out = fopen (file_path_out, "w");
    }
  work_file_in = fopen (file_path_in, "r");
  if (work_file_in == NULL)
    {
      fprintf (stderr, ERROR_FILE);
      return EXIT_FAILURE;
    }
  if (command == ENCODE_DECODE)
    {
      encode (work_file_in, work_file_out, key);
    }
  if (command == CHECK)
    {
      check_code (work_file_in, work_file_out);
    }
  fclose (work_file_out);
  fclose (work_file_in);
  return EXIT_SUCCESS;
}

/**
 * The main function that runs the cipher program
 * @param argc : number of arguments
 * @param argv : the arguments
 * @return : EXIT_SUCCESS on success else EXIT_FAILURE
*/
int main (int argc, char *argv[])
{
  if (argc == 1)
    {
      fprintf (stderr, ERROR_COMMAND);
      return EXIT_FAILURE;
    }
  if (check_input (argc, argv) == EXIT_FAILURE)
    {
      return EXIT_FAILURE;
    }
  int command = ENCODE_DECODE;
  if (strcmp (argv[1], ENCODE_COMM) == SAME)
    {
      return file_handler (argv[3], argv[4],
                           atoi (argv[2]), command);
    }
  if (strcmp (argv[1], DECODE_COMM) == SAME)
    {
      return file_handler (argv[3], argv[4],
                           0 - atoi (argv[2]), command);
    }
  if (strcmp (argv[1], CHECK_COMM) == SAME)
    {
      command = CHECK;
      return file_handler (argv[2], argv[3],
                           0, command);
    }
}