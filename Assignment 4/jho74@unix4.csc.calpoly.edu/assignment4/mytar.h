#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <math.h>


#ifndef CREATEARCHIVE_H
#include "createarchive.h"
#endif

#ifndef PRINTARCHIVE_H
#include "printarchive.h"
#endif

#ifndef EXTRACTARCHIVE_H
#include "extractarchive.h"
#endif

#ifndef MYTAR_H
#define MYTAR_H

#define BLOCK_SIZE_BYTES 512
#define NAME_SIZE_BYTES 100
#define CHECKSUM_SIZE_BYTES 8
#define LINK_NAME_SIZE_BYTES 100
#define MAGIC_SIZE_BYTES 6
#define VERSION_SIZE_BYTES 2
#define PREFIX_SIZE_BYTES 155

#define MAGIC_VALUE "ustar\0"
#define VERSION_VALUE "00"

#define REGULAR_FILE '0'
#define REGULAR_FILE_ALT '\0'
#define SYM_LINK '2'
#define DIRECTORY '5'

#define OWNER_NAME_LENGTH 17
struct Header {
  char name[100];               /* 0-99 */
  char mode[8];                 /* 100-107 */
  char uid[8];                  /* 108-115 */
  char gid[8];                  /* 116-123 */
  char size[12];                /* 124-135 */
  char mtime[12];               /* 136-147 */
  char checksum[8];             /* 148-155 */
  char typeflag;                /* 156 */
  char linkname[100];           /* 157-256 */
  char magic[6];                /* 257-262 */
  char version[2];              /* 263-264 */
  char uname[32];               /* 265-296 */
  char gname[32];               /* 297-328 */
  char devmajor[8];             /* 329-336 */
  char devminor[8];             /* 337-344 */
  char prefix[155];             /* 345-499 */
  char padding[12];             /* 500-512 */
};

int check_options(char *options);
char *append_name(char *directory_name, char *to_append);
int octal_to_string(char *octal);
int read_header(FILE *archive, struct Header *header);
int check_file(char *file, char *compare);
void create_name(char *buffer, char *prefix, char *name);

#endif

