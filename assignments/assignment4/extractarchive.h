
#ifndef EXTRACTARCHIVE_H
#define EXTRACTARCHIVE_H

int extract_file(char *output_file, struct Header *header, FILE *archive);
int extract_symlink(struct Header *header, FILE *out_file);
int extract_directory(struct Header *header, FILE *out_file);
int extraction(char *file_name, struct Header* header, FILE *archive, int verbose_flag);
int extract_archive(int argc, char **argv, int verbose_flag);

#endif