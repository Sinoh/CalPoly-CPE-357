#include <stdio.h>

int main() {
  int c;
  int column_count = 0;
  int tab_spacing = 8;

  while ((c = getchar()) != EOF) {

    switch (c) {
    case '\n':
    case '\r':
      column_count = 0;
      putchar(c);
      break;

    case '\t':
      if ((column_count % tab_spacing) == 0) {
        putchar(' ');
        column_count++;
      }

      while ((column_count % tab_spacing) != 0) {
        putchar(' ');
        column_count++;
      }
      break;

    case '\b':
      column_count--;
      if (column_count < 0) {
        column_count = 0;
      }
      break;

    default:
      putchar(c);
      column_count++;
      break;
    }
  }
  return 0;
}
