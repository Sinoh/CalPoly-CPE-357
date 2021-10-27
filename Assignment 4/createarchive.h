
#ifndef CREATEARCHIVE_H
#define CREATEARCHIVE_H

struct Header* create_header(char *file_name, struct stat *stats, int strict_flag);
int archive_file(char *file_name, FILE *out_file);
void archive_directory(char *directory_name, FILE *out_file, int verbose_flag, int strict_flag);
void operate_file(char *argv, struct stat *stats, FILE *out_file, int verbose_flag, int strict_flag);
int create_archive(int argc, char **argv, int verbose_flag, int strict_flag);

#endif