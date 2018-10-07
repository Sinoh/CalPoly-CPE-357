#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_long_line(FILE *file) {

  // Initialize some size for malloc
  int memorySize = 64;
  int memoryIndex = 0;

  // Allocate memory for the current line
  char *currentLine = (char *)malloc(memorySize * sizeof(*currentLine));
  if (currentLine == NULL) {
    perror("Currentline Error");
  }
  // Allocate memory for the previous line of the file
  char *previousLine = (char *)calloc(1, sizeof(*previousLine));
  if (previousLine == NULL) {
    perror("Previousline Error");
  }

  // When end of file, this loop will end
  while (feof(file) != 1) {
    char s = fgetc(file);

    // Make sure theres enough memory allocated
    if (memorySize == memoryIndex) {
      memorySize = memorySize * 2;
      currentLine = (char *)realloc(currentLine - memoryIndex, memorySize);
      currentLine += memoryIndex;
    }

    // Check compare current line and previous line
    // Also reallocate space for previous line
    if (s == '\n') {
      *currentLine = '\0';
      currentLine = currentLine - memoryIndex;

      if (strcmp(currentLine, previousLine)) {
        printf("%s\n", currentLine);
      }

      previousLine = (char *)realloc(previousLine,
                                     (memoryIndex + 1) * sizeof(*previousLine));
      strcpy(previousLine, currentLine);
      memoryIndex = 0;
    }

    // Keep looping if else
    else {
      *currentLine = s;
      memoryIndex++;
      currentLine++;
    }
  }
  // Remember to close the allocated spaces
  free(currentLine);
  free(previousLine);
}

int main(int argc, char **argv) {
  // opens the file given from the command line
  // read_long_line will perform uniq
  FILE *openfile = fopen(argv[1], "r");
  read_long_line(openfile);
  fclose(openfile);
  return 0;
}
