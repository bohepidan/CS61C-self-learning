/*
 * Include the provided hash table library.
 */
#include "hashtable.h"

/*
 * Include the header file.
 */
#include "philspel.h"

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Character utility routines.
 */
#include <ctype.h>

/*
 * String utility routines.
 */
#include <string.h>

/*
 * This hash table stores the dictionary.
 */
#include <stdbool.h>
HashTable *dictionary;
const int MAXLEN = 60;
/*
 * The MAIN routine.  You can safely print debugging information
 * to standard error (stderr) as shown and it will be ignored in
 * the grading process.
 */
int main(int argc, char **argv)
{
  if (argc != 2)
  {
    fprintf(stderr, "Specify a dictionary\n");
    return 0;
  }
  /*
   * Allocate a hash table to store the dictionary.
   */
  fprintf(stderr, "Creating hashtable\n");
  dictionary = createHashTable(2255, &stringHash, &stringEquals);

  fprintf(stderr, "Loading dictionary %s\n", argv[1]);
  readDictionary(argv[1]);
  fprintf(stderr, "Dictionary loaded\n");

  fprintf(stderr, "Processing stdin\n");
  processInput();

  /*
   * The MAIN function in C should always return 0 as a way of telling
   * whatever program invoked this that everything went OK.
   */
  return 0;
}

/*
 * This should hash a string to a bucket index.  Void *s can be safely cast
 * to a char * (null terminated string) and is already done for you here
 * for convenience.
 */
unsigned int stringHash(void *s)
{
  char *string = (char *)s;
  // -- TODO --
  unsigned int shift = 0;
  for (char *cur = string; *cur != '\0'; cur++)
  {
    if (*cur - 'a' >= 0 && *cur - 'z' <= 0)
    {
      shift += *cur - 'a';
    }
    else if (*cur - 'A' >= 0 && *cur - 'Z' <= 0)
    {
      shift += *cur - 'A';
    }
  }
  return shift % 2255;
}

/*
 * This should return a nonzero value if the two strings are identical
 * (case sensitive comparison) and 0 otherwise.
 */
int stringEquals(void *s1, void *s2)
{
  // in mu defination, s2 is the key, s1 is in the linked list.
  char *string1 = (char *)s1;
  char *string2 = (char *)s2;
  // -- TODO --
  if (strlen(string1) != strlen(string2))
  {
    return 0;
  }

  if (strcmp(string1, string2) == 0)
  {
    return 1;
  }
  else if (*string1 - *string2 == 0 || *string1 - *string2 == 'a' - 'A')
  {
    char *cur1 = string1 + 1, *cur2 = string2 + 1;
    while (*cur1)
    {
      if (*cur2 >= 'a')
      {
        if (*cur1 != *cur2)
          return 0;
      }
      else
      {
        if (*cur2 - *cur1 != 'A' - 'a')
        {
          return 0;
        }
      }
      cur1++;
      cur2++;
    }
    return 1;
  }
  else
  {
    return 0;
  }
}

/*
 * This function should read in every word from the dictionary and
 * store it in the hash table.  You should first open the file specified,
 * then read the words one at a time and insert them into the dictionary.
 * Once the file is read in completely, return.  You will need to allocate
 * (using malloc()) space for each word.  As described in the spec, you
 * can initially assume that no word is longer than 60 characters.  However,
 * for the final 20% of your grade, you cannot assumed that words have a bounded
 * length.  You CANNOT assume that the specified file exists.  If the file does
 * NOT exist, you should print some message to standard error and call exit(1)
 * to cleanly exit the program.
 *
 * Since the format is one word at a time, with new lines in between,
 * you can safely use fscanf() to read in the strings until you want to handle
 * arbitrarily long dictionary chacaters.
 */
void readDictionary(char *dictName)
{
  // -- TODO --
  FILE *words = NULL;
  words = fopen(dictName, "r");
  if (words == NULL)
  {
    fprintf(stderr, "no such file!");
    exit(1);
  }
  char *singleword = malloc(sizeof(char) * (1 + MAXLEN));
  while (fscanf(words, "%s\n", singleword) != EOF)
  {
    char *newstr = malloc(sizeof(char) * (strlen(singleword) + 1));
    strcpy(newstr, singleword);
    insertData(dictionary, newstr, newstr);
  }
  fclose(words);
}

/*
 * This should process standard input (stdin) and copy it to standard
 * output (stdout) as specified in the spec (e.g., if a standard
 * dictionary was used and the string "this is a taest of  this-proGram"
 * was given to stdin, the output to stdout should be
 * "this is a teast [sic] of  this-proGram").  All words should be checked
 * against the dictionary as they are input, then with all but the first
 * letter converted to lowercase, and finally with all letters converted
 * to lowercase.  Only if all 3 cases are not in the dictionary should it
 * be reported as not found by appending " [sic]" after the error.
 *
 * Since we care about preserving whitespace and pass through all non alphabet
 * characters untouched, scanf() is probably insufficent (since it only considers
 * whitespace as breaking strings), meaning you will probably have
 * to get characters from stdin one at a time.
 *
 * Do note that even under the initial assumption that no word is longer than 60
 * characters, you may still encounter strings of non-alphabetic characters (e.g.,
 * numbers and punctuation) which are longer than 60 characters. Again, for the
 * final 20% of your grade, you cannot assume words have a bounded length.
 */
void processInput()
{
  // -- TODO --
  char cur;
  char *buf = malloc(sizeof(char) * 1024);
  int i = 0;
  while ((cur = getchar()) != '\n')
  {
    buf[i++] = cur;
  }
  buf[i] = '\0';
  char *input = malloc(sizeof(char) * 60);
  char *present = buf;
  while (true)
  {
    i = 0;
    while (*present != ' ' && *present != '\0')
    {
      input[i++] = *present;
      present++;
    }
    input[i] = '\0';
    bool isword = true;
    for (int j = 0; j < i; j++)
    {
      if (input[j] - 'a' <= 'z' - 'a' && input[j] - 'a' >= 0 || input[j] - 'A' >= 0 && input[j] - 'A' <= 'Z' - 'A')
        ;
      else
        isword = false;
    }
    if (isword)
    {
      void *found = findData(dictionary, input);
      if (!found)
      {
        printf("%s [sic]", input);
      }
      else
      {
        printf("%s", input);
      }
    }
    else
    {
      printf("%s", input);
    }
    if (*present == '\0')
    {
      printf("\n");
      break;
    }
    else
    {
      printf(" ");
      present++;
    }
  }
}
