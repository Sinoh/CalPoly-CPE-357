
#ifndef PRINTARCHIVE_H
#define PRINTARCHIVE_H

void list_archives(char *file_name, struct Header *header, int verbose_flag);
int print_archive(int argc, char **argv, int verbose_flag);

#endif