
#include "mytar.h"
#include "createarchive.h"

struct Header* create_header(char *file_name, struct stat *stats, int strict_flag) {
    int i;
    int checksum = 0;
    char name[NAME_SIZE_BYTES];
    struct Header *header;
    struct passwd *user;
    struct group *group;
    unsigned char *ptr_header = (unsigned char *)&header;

    /* Check the length of the file name */
    if (strlen(file_name) < NAME_SIZE_BYTES) { 
        /* If the file name is less than 100, copy file name to header->file */
        strcpy(header->name, file_name);
    }
    /* If the file name is greater than 100, put overflow into prefix */
    if (strlen(file_name) > NAME_SIZE_BYTES) {
        strncpy(header->prefix, file_name, PREFIX_SIZE_BYTES);
        for(i = 155; i < strlen(file_name); i++) {
            name[i - PREFIX_SIZE_BYTES] = header->name[i];
        strncpy(header->name, name, strlen(name));
        }
    }

    /* Print the permission mode into header->mode in an octal format of 7 bytes. The eighth byte is '\0' */
    sprintf(header->mode, "%07o", stats->st_mode);
    /* Needs to change the leading bytes to '0' */
    header->mode[0] = '0';
    header->mode[1] = '0';
    header->mode[2] = '0';
    /* Print the user id into header->uid in an octal format of 7 bytes. The eighth byte is '\0' */
    sprintf(header->uid, "%07o", stats->st_uid);
    /* Print the group id into header->gid in an octal format of 7 bytes. The eighth byte is '\0' */
    sprintf(header->gid, "%07o", stats->st_gid);
    /* Print the modification time into header->mtime in an octal format of 11 bytes. The twelfth byte is '\0' */
    sprintf(header->mtime, "%011lo", stats->st_mtime);

    /* Check what type of file this is */
    if (S_ISREG(stats->st_mode)) {
        /* If a regular file, set typeflag to '0' */
        header->typeflag = REGULAR_FILE;
        sprintf(header->size, "%011lo", stats->st_size);
    } else if (S_ISLNK(stats->st_mode)) {
        /* If a symbolic link, set typeflag to '2' */
        header->typeflag = SYM_LINK;
        strcpy(header->size, "00000000000");
        readlink(file_name, header->linkname, LINK_NAME_SIZE_BYTES);
    } else if (S_ISDIR(stats->st_mode)) {
        /* If a directory, set typeflag to '5' */
        header->typeflag = DIRECTORY;
        strcpy(header->size, "00000000000");
    }

    /* Set magic to "ustar\0" */
    strcpy(header->magic, MAGIC_VALUE);
    /* Set version to "00" */
    strcpy(header->version, VERSION_VALUE);

    /* Create a sturct passwd */
    user = getpwuid(stats->st_uid);
    /* Create a struct group */
    group = getgrgid(stats->st_gid);
    /* Copy the file's owner name into uname */
    strcpy(header->uname, user->pw_name);
    /* Copy the file's group name into gname */
    strcpy(header->gname, group->gr_name);

    /* Adds up every byte in the header */
    for (i = 0; i < BLOCK_SIZE_BYTES; i++) {
        checksum += *ptr_header;
        ptr_header++;
    }

    /* Also adds eight spaces */
    checksum += ' ' * CHECKSUM_SIZE_BYTES;
    /* Copy checksum to header in an octal format of 7 bytes. The eighth byte is '\0' */
    sprintf(header->checksum, "%07o", checksum);

    return header;
}

int archive_file(char *file_name, FILE *out_file) {
    FILE *in_file;
    char buffer_block[BLOCK_SIZE_BYTES] = {0};

    /* Open the input file with read permissions and check if NULL */
    in_file = fopen(file_name, "rb");
    if(in_file == NULL) {
        perror("Error");
        return 0;
    }
    /* fread the input file to buffer_block */
    while(fread(buffer_block, BLOCK_SIZE_BYTES, 1, in_file)) {
        /* If end of file, break out */
        if (feof(in_file)){
            break;
        } else if (ferror(in_file)){
            /* If error, perror it */
            perror("error");
        } else {
            /* If read is successful, write a block to the ouput file */
            fwrite(buffer_block, BLOCK_SIZE_BYTES, 1, out_file);
            /* Reset the block with 0 */
            memset(buffer_block, 0, BLOCK_SIZE_BYTES);
        }
    }
    /* Write the las block to output file and close the input file */
    fwrite(buffer_block, BLOCK_SIZE_BYTES, 1, out_file);
    fclose(in_file);
    return 1;

}

void archive_directory(char *directory_name, FILE *out_file, int verbose_flag, int strict_flag) {
    DIR *current_directory;
    char *file_name;
    struct stat file;
    struct dirent *directory;
    struct stat directory_stats;

    /* Append a "/" to the current directory's name */
    directory_name = append_name(directory_name, "/");

    /* Open directory and check for NULL */
    if ((current_directory = opendir(directory_name)) != NULL) {
        /* Stat the current directory for checking */
        stat(directory_name, &directory_stats);
        /* Loop through every file in current directory */
        while ((directory = readdir(current_directory)) != NULL) {
            /* Check if the file is '.' by check for the first character and inode number */
            if (directory->d_name[0] != '.' && directory->d_ino != directory_stats.st_ino) {
                /* If the file is not the parent directory, append the file's name to the path name */
                file_name = append_name(directory_name, directory->d_name);
                /* If the verbose_flag is set, print */
                if (verbose_flag) {
                    printf("%s\n", file_name);
                }
                /* lstat the file and recusivley operate again */
                if (lstat(file_name, &file) == 0){
                    operate_file(file_name, &file, out_file, verbose_flag, strict_flag);
                }
                /* free this file */
                free(file_name);
            }
        }
    }
    /* Close and free this current directory */
    closedir(current_directory);
    free(directory_name);
}
void operate_file(char *argv, struct stat *stats, FILE *out_file, int verbose_flag, int strict_flag) {
    struct Header *header = create_header(argv, stats, strict_flag);
    
    /* Check what type of file it is */
    if (S_ISREG(stats->st_mode)){
        /* If it is a regular file, write the header to ouput and archive the file */
        fwrite(&header, sizeof(struct Header*), 1, out_file);
        archive_file(argv, out_file);
    } else if (S_ISLNK(stats->st_mode)){
        /* If it is a symbolic link, write the header to output */
        fwrite(&header, sizeof(struct Header*), 1, out_file);
    } else if (S_ISDIR(stats->st_mode)){
        /* If it is a directory, call this function to enter the directory to arhive it and deal with its files */
        archive_directory(argv, out_file, verbose_flag, strict_flag);
    }
}

int createArchive(int argc, char **argv, int verbose_flag, int strict_flag) {
    FILE *out_file;
    int i;
    char extra_block[BLOCK_SIZE_BYTES] = {0};
    struct stat stats;

    /* Open the output file with permissions to write and truncate if existing, and create if non-existing */
    out_file = fopen(argv[2], "wb");

    /* Loop through all the file/path arguments passed in the command line */
    for (i = 3; i < argc; i++) {
        /* Stat the current file */
        if (lstat(argv[i], &stats) == 0) {
            /* If the Verbose flag is set, print */
            if (verbose_flag) {
                printf("%s\n", argv[i]);
            }
            /* Call this function to deal with the file */
            operate_file(argv[i], &stats, out_file, verbose_flag, strict_flag);
        }
    }

    /* Write two empyt blocks of zeros at the end of the output file */
    fwrite(extra_block, BLOCK_SIZE_BYTES, 1, out_file);
    fwrite(extra_block, BLOCK_SIZE_BYTES, 1, out_file);
    /* Remember to close the output file */
    fclose(out_file);
    return 0;

}