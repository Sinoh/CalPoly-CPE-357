
#include "mytar.h"
#include "printarchive.h"


void list_archives(char *file_name, struct Header *header, int verbose_flag) {
    char mode[10];
    int permissions = octal_to_string(header->mode);
    char owner_name[OWNER_NAME_LENGTH];
    char *ptr_owner = owner_name;
    time_t mtime = octal_to_string(header->mtime);
    struct tm *time = NULL;
    char mtime_buffer[17];

    /* If verbose flag is not set, just print */
    if (!(verbose_flag)){
        printf("%s\n", file_name);
    } else if (verbose_flag) {
        /* If verbose flag is set get some info */
        /* Find the file type and set the first byte of mode */
        if (header->typeflag == REGULAR_FILE) {
            mode[0] = '-';
        } else if (header->typeflag == SYM_LINK) {
            mode[0] = '1';
        } else if (header->typeflag == DIRECTORY) {
            mode[0] = 'd';
        }

        /* Find the 9 bytes of permisions for mode */
        if (permissions & S_IRUSR) {
            mode[1] = 'r';
        } else {
            mode[1] = '-';
        }
        if (permissions & S_IWUSR){
            mode[2] = 'w';
        } else {
            mode[2] = '-';
        }   
        if (permissions & S_IXUSR) {
            mode[3] = 'x';
        } else {
            mode[3] = '-';
        }
        if (permissions & S_IRGRP) {
            mode[4] = 'r';
        } else {
            mode[4] = '-';
        }
        if (permissions & S_IWGRP) {
            mode[5] = 'w';
        } else {
            mode[5] = '-';
        }
        if (permissions & S_IXGRP) {
            mode[6] = 'x';
        } else {
            mode[6] = '-';
        }
        if (permissions & S_IROTH) {
            mode[7] = 'r';
        } else {
            mode[7] = '-';
        }
        if (permissions & S_IWOTH) {
            mode[8] = 'w';
        } else {
            mode[8] = '-';
        }
        if (permissions & S_IXOTH) {
            mode[9] = 'x';
        } else {
            mode[9] = '-';
        }

        /* Concatenate the username and group name */
        if (strlen(header->uname) > OWNER_NAME_LENGTH) {
            /* if the username is longer the 17 char, just use the 17 char */
            strncpy(owner_name, header->uname, OWNER_NAME_LENGTH);
        } else {
            /* If not, check if appending group name and '/' will go over 17 */
            strncpy(owner_name, header->uname, strlen(header->uname));
            ptr_owner += strlen(header->uname);
            *ptr_owner = '/';
            ptr_owner++;

            if(strlen(header->uname) + strlen(header->gname) < OWNER_NAME_LENGTH) {
                /* If the two names concatinated with a '/' is less than 17 char, copy to buffer */
                strncpy(owner_name, header->gname, strlen(header->gname));
            } else {
                /* If greater than 17, fill up as much of group name as possible */
                strncpy(owner_name, header->gname, OWNER_NAME_LENGTH - strlen(header->gname));
            }
        }

        /* Get modifcation time for the file */
        time = localtime(&mtime);
        /* Put the times in a buffer */
        sprintf(mtime_buffer, "%04i-%02i-%02i %02i:%02i",
            1900 + time->tm_year,
            time->tm_mon + 1,
            time->tm_mday,
            time->tm_hour,
            time->tm_min);

        /* Print all the info out */
        printf("%10s %-17s %8i %16s %s\n", mode, owner_name, octal_to_string(header->size), mtime_buffer, file_name);
    }
}

int print_archive(int argc, char **argv, int verbose_flag) {
    FILE *archive;
    struct Header *header = {0};
    int i;
    int result;
    char file_name[NAME_SIZE_BYTES + PREFIX_SIZE_BYTES + 2];

    /* Open the file */
    archive = fopen(argv[2], "rb");
    /* Loop read the each header */
    while (read_header(archive, header) != 0) {
        /* Create a char of the name of the file */
        create_name(file_name, header->prefix, header->name);

        /* If no files are given, print the whole archive */
        if (argc < 4) {
            list_archives(file_name, header, verbose_flag);
        } else {
            /* If files are given, find each one in the order given */
            for (i = 3; i < argc; i++) {
                /* Check if current file is the same as the file passed */
                if (check_file(file_name, argv[i]) == 0) {
                    /* If it is, print */
                    list_archives(file_name, header, verbose_flag);
                }
            }
        }
    }
    /* Close oopened files */
    fclose(archive);
    return result;
}