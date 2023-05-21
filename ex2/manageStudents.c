#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define USER_INPUT_BUFFER 60
#define INFO_BUFFER 20
#define ID 2
#define GRADE 3
#define AGE 4
#define SAME 0
#define NUM_STUDENTS_INPUT "Enter number of students. Then enter\n"
#define ERR_NUM_STUDENTS_INPUT "ERROR: Number of students should be a \
positive integer\n"
#define ERR_ID "ERROR: Id should be an integer 10 digits long\n"
#define ERR_GRADE "ERROR: grade should be an integer between 0 and 100 \
(includes)\n"
#define ERR_AGE "ERROR: Age should be an integer between 18 and 120 \
(includes)\n"
#define BEST_STUDENT "best student info is: %ld,%d,%d\n"
#define STUDENT_INFO "Enter student info. Then enter\n"
#define USAGE_COMMAND "USAGE: Wrong command. please choose between <best,\
quick, bubble>\n"
#define USAGE_SIZE "USAGE: Wrong number of arguments, correct form is \
<program name><command>\n"
#define ERR_ID_ZERO "ERROR: Id should not start with 0\n"
#define COMMAND_BEST "best"
#define COMMAND_BUBBLE "bubble"
#define COMMAND_QUICK "quick"
#define BASE 10
#define MAX_ID 1000000000
#define MAX_GRADE 100
#define MIN_GRADE 0
#define MAX_AGE 120
#define MIN_AGE 18
#define ID_LEN 10


/**
 * This function check if the commands the user game are right
 * @param size : number of commands
 * @param inputs : the user input
 * @return EXIT_SUCCESS if the inputs OK else EXIT_FAILURE
 */
int check_usage (int size, char **inputs)
{
  if (size != 2)
    {
      printf (USAGE_SIZE);
      return EXIT_FAILURE;
    }
  if (strcmp (*(inputs + 1), COMMAND_BEST) == SAME)
    {
      return EXIT_SUCCESS;
    }
  if (strcmp (*(inputs + 1), COMMAND_QUICK) == SAME)
    {
      return EXIT_SUCCESS;
    }
  if (strcmp (*(inputs + 1), COMMAND_BUBBLE) == SAME)
    {
      return EXIT_SUCCESS;
    }
  printf (USAGE_COMMAND);
  return EXIT_FAILURE;

}

/**
 * A struct that holds the info about the students
 */
typedef struct Student {
    int age;
    int grade;
    long int id;
} Student;


/**
 * The function asks the user to input the number of students and check its
 * validity
 * @param num_of_students : the number of students
 * @return The number of students
 */
long int number_of_students (long int num_of_students)
{
  char user_input[USER_INPUT_BUFFER];
  char *ptr;
  printf (NUM_STUDENTS_INPUT);
  fgets (user_input, USER_INPUT_BUFFER, stdin);
  num_of_students = strtol (user_input, &ptr, BASE);
  while ((strcmp (ptr, "\n") != 0) || num_of_students <= 0)
    {
      printf (ERR_NUM_STUDENTS_INPUT);
      printf (NUM_STUDENTS_INPUT);
      fgets (user_input, USER_INPUT_BUFFER, stdin);
      num_of_students = strtol (user_input, &ptr, BASE);
    }
  return num_of_students;
}


/**
 * The function checks if the input is comprised only of digits
 * @param user_input : the input from the user
 * @param flag : tells the function what kind of check to do
 * @return EXIT_SUCCESS if the everything OK else EXIT_FAILURE
 */
int check_digits (char *user_input, int flag)
{
  long int temp = strlen (user_input);
  if (flag == AGE)
    {
      temp -= 1;
    }
  if (flag == ID && user_input == 0)
    {
      return EXIT_FAILURE;
    }
  for (int idx = 0; idx < temp; ++idx)
    {
      if (isdigit(user_input[idx]) == 0)
        {
          return EXIT_FAILURE;
        }
    }
  return EXIT_SUCCESS;
}


/**
 * Checks if the input from the user is OK
 * @param user_input : the input from the user
 * @return EXIT_SUCCESS if the inputs OK else the number of error
 */
int check_input (char *user_input)
{
  char grade[INFO_BUFFER], age[INFO_BUFFER], id[INFO_BUFFER];
  long int temp;
  char *ptr;
  if (sscanf (user_input, "%[^,], %[^,], %[^,]", id, grade, age) == EOF)
    {
      return EXIT_FAILURE;
    }
  if (strlen (id) != ID_LEN || check_digits (id, ID) == EXIT_FAILURE)
    {
      printf (ERR_ID);
      return ID;
    }
  temp = strtol (grade, &ptr, BASE);
  if ((temp < MIN_GRADE || MAX_GRADE < temp)
      || check_digits (grade, GRADE) == EXIT_FAILURE)
    {
      printf (ERR_GRADE);
      return GRADE;
    }
  temp = strtol (age, &ptr, BASE);
  if ((temp < MIN_AGE || MAX_AGE < temp)
      || check_digits (age, AGE) == EXIT_FAILURE)
    {
      printf (ERR_AGE);
      return AGE;
    }
  return EXIT_SUCCESS;
}


/**
 * The function gets the students info from the user and loads it into
 * the structs
 * @param num_of_students : number of students
 * @param students : the struct holding the students
 * @return a pointer the the end of the last student struct
 */
Student *load_students (long int num_of_students, Student *students)
{
  char user_input[USER_INPUT_BUFFER];
  long int id;
  int grade, age;
  int success;
  char *success_input;
  for (int i = 0; i < num_of_students; ++i)
    {
      printf (STUDENT_INFO);
      success_input = fgets (user_input, USER_INPUT_BUFFER, stdin);
      if (success_input == NULL)
        {
          return NULL;
        }
      success = check_input (user_input);
      if (success == EXIT_SUCCESS)
        {
          sscanf (user_input, "%ld,%d,%d", &id, &grade, &age);
          if(id < MAX_ID)
            {
              printf (ERR_ID_ZERO);
              i -= 1;
              continue;
            }
          (students + i)->id = id;
          (students + i)->grade = grade;
          (students + i)->age = age;
        }
      else if (success == EXIT_FAILURE)
        {
          return NULL;
        }
      else
        {
          i -= 1;
        }
    }
  return students + num_of_students;
}


/**
 * The function searches for the most accomplished students and prints his
 * info out
 * @param start : a pointer to the first students
 * @param end : a pointer to the last struct
 */
void best_student (Student *start, Student *end)
{
  float so_far = (float) (start->grade) / (float) (start->age);
  int best = 0;
  int student_number = 0;
  float temp;
  Student *contender = start;
  while (contender != end)
    {
      temp = (float) (contender->grade) / (float) (contender->age);
      if (temp > so_far)
        {
          so_far = (float) (contender->grade) / (float) (contender->age);
          best = student_number;
          student_number += 1;
          contender += 1;
        }
      else
        {
          student_number += 1;
          contender += 1;
        }
    }
  printf (BEST_STUDENT, (start + best)->id, (start + best)->grade, (
      start + best)->age);
}


/**
 * The function swaps between to students
 * @param first : one of the students to swap with
 * @param second : one of the students to swap with
 */
void swap (Student *first, Student *second)
{
  Student temp = *first;
  *first = *second;
  *second = temp;
}


/**
 * This function sorts the students by grade
 * @param start : the first student
 * @param end : the second student
 */
void bubble_sort (Student *start, Student *end)
{
  long int len = end - start;
  for (int i = 0; i < len - 1; i++)
    {
      for (int j = 0; j < len - i - 1; j++)
        {
          if ((start + j)->grade > (start + j + 1)->grade)
            {
              swap (start + j, start + j + 1);
            }
        }
    }
}

/**
 * This function helps the quick sort function by pivoting and swapping
 * students
 * @param start : a pointer the first student
 * @param end : a pointer to the end of the last student
 * @return a pointer to the first student
 */
Student *partition (Student *start, Student *end)
{
  Student *pivot = end - 1;
  Student *i = start;

  for (Student *j = start; j != end - 1; j++)
    {
      if (j->age <= pivot->age)
        {
          swap (i, j);
          i++;
        }
    }
  swap (i, pivot);
  return i;
}

/**
 * This function sorts the students by age
 * @param start : a pointer the first student
 * @param end : a pointer to the end of the last student
 */
void quick_sort (Student *start, Student *end)
{
  if (start < end)
    {
      Student *pivot = partition (start, end);
      quick_sort (start, pivot);
      quick_sort (pivot + 1, end);
    }
}

/**
 * This function prints the students list
 * @param start : a pointer the first student
 * @param end : a pointer to the end of the last student
 */
void print_list (Student *start, Student *end)
{
  long int len = end - start;
  for (int i = 0; i <= len - 1; i++)
    {
      printf ("%ld,%d,%d\n", (start + i)->id, (start + i)->grade, (
          start + i)->age);
    }
}

/**
 * The main function
 * @param argc : number of arguments
 * @param argv : the arguments
 * @return EXIT_SUCCESS if the everything OK else EXIT_FAILURE
 */
int main (int argc, char **argv)
{
  if (check_usage (argc, argv) == EXIT_FAILURE)
    {
      return EXIT_FAILURE;
    }
  long int num_of_students = 0;
  num_of_students = number_of_students (num_of_students);
  Student *students = (Student *) malloc (sizeof (Student) * num_of_students);
  if (students == NULL)
    {
      return EXIT_FAILURE;
    }
  Student *end = load_students (num_of_students, students);
  if (end == NULL)
    {
      return EXIT_FAILURE;
    }
  if (strcmp (argv[1], COMMAND_BEST) == SAME)
    {
      best_student (students, end);
    }
  if (strcmp (argv[1], COMMAND_BUBBLE) == SAME)
    {
      bubble_sort (students, end);
      print_list (students, end);

    }
  if (strcmp (argv[1], COMMAND_QUICK) == SAME)
    {
      quick_sort (students, end);
      print_list (students, end);
    }
  free (students);
  return EXIT_SUCCESS;
}