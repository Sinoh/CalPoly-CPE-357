/* This isn't a particularly interesting program, but it was
 * designed to be broken, not to do anything useful.
 *   What it does (or should) do:
 *     * separate the input into a list of words, then traverse the
 *       list forwards printing each word forwards, then reversed
 *       about the middle of the screen, print a horizontal bar,
 *       then repeat with the list backwards.
 *
 *     * For the purposes of this exercise, a word is defined as a
 *       a sequence of non-whitespace characters delimited by
 *       whitespace or the end of the buffer, whichever comes first. :)
 *
Example:
% a.out
Able was I ere I saw Elba
                                   Able|elbA
                                    was|saw
                                      I|I
                                    ere|ere
                                      I|I
                                    saw|was
                                   Elba|ablE
---------------------------------------+---------------------------------------
                                   ablE|Elba
                                    was|saw
                                      I|I
                                    ere|ere
                                      I|I
                                    saw|was
                                   elbA|Able
%

 *
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 39

char *reverse();                /* old-style prototype for reverse() */


typedef struct word *word;
struct word {
  char *word;
  word next;
  word prev;
};

word newword(char *s, char *e) {
  /* Create a new struct word with a copy of the given string */
  word new;
  int len,i;

  new = (word) malloc(sizeof(struct word));
  len = e - s;

  i = 0;
  for (new->word=malloc(len+1); i < len ; i++ )
    new->word[i]=s[i];
  new->word[i]='\0';


  new->next = NULL;
  new->prev = NULL;
  return new;
}

int main(int argc, char *argv[]) {
  char buffer[SIZE];
  char *s,*e;
  int i;
  word w,fwd,back,wordlist;

  wordlist = (word)malloc(sizeof(struct word));

  wordlist->word=NULL;
  wordlist->next = wordlist;
  wordlist->prev = wordlist;

  while ( NULL != (s = fgets(buffer,SIZE,stdin)) ) {

    for (; *s != '\0'; s = e) {
      while ( isspace(*s) )
        s++;                      /* skip whitespace */

      /* find the end of it */
      for (e=s;*e && !isspace(*e);e++)
        /* nothing */;

      if ( *s != '\0' ) {       /* if the string isn't empty */
                                /* add it to the list */
        w = newword(s,e);
        wordlist->prev->next = w;
        w->prev = wordlist->prev;
        wordlist->prev = w;
        w->next = wordlist;
      }
    }
    if (*s == '\0') {
        break;
    }
  }

  /* now print the list */
  /* forwards */
  fwd  = wordlist->next;
  while ( fwd != wordlist ) {
    printf("%*.*s|",SIZE,SIZE,fwd->word);
    printf("%s\n",reverse(fwd->word, 4));
    fwd  = fwd->next;
  }

  /* the bar */
  for(i=0;i < SIZE; i++)
    putchar('-');
  putchar('+');
  for(i=0;i < SIZE; i++)
    putchar('-');
  putchar('\n');

  /* backwards */
  back = wordlist->prev;
  while ( back != wordlist ) {
    printf("%*.*s|",SIZE,SIZE,back->word);
    printf("%s\n",reverse(back->word, 4));
    back = back->prev;
  }


  return 0;
}

char *reverse(char *s, int len) {
  /* reverse the string in place */
  int i;
  char tmp;

  for(i=0; i < len; i++) {
    tmp      = s[i];
    s[i]     = s[len-i-1];
    s[len-i-1] = tmp;
  }
  return s;
}

