
#include "mytar.h"
#include "extractarchive.h"

int extract_file(char *output_file, struct Header *header, FILE *archive) {
    FILE *output;
    char buffer_block[BLOCK_SIZE_BYTES];
    int file_size = octal_to_string(header->size);
    int this_size = 0;

    /* Open the output file */
    output = fopen(output_file, "wb");

    /* Read the archive one block at a time */
    while(fread(buffer_block, BLOCK_SIZE_BYTES, 1, archive)) {
        /* If end of file, break out */
        if (feof(archive)){
            break;
        } else if (ferror(archive)){
            /* If error, perror it */
            perror("error");
        } else {
            /* Of the size for the ouput file plus the next block is greater than the file size, break out */
            if (this_size + BLOCK_SIZE_BYTES > file_size) {
                break;
            } else {
                /* If read is successful, write a block to the ouput file */
                fwrite(buffer_block, BLOCK_SIZE_BYTES, 1, output);
                this_size += BLOCK_SIZE_BYTES;
                /* Reset the block with 0 */
                memset(buffer_block, 0, BLOCK_SIZE_BYTES);
            }
        }
    }
    /* Write the last block given and close the output*/
    fwrite(buffer_block, file_size - this_size, 1, output);
    fclose(output);

    /* Set some info for the file */
    int mode = octal_to_string(header->mode);
    if (chmod(header->name, mode) < 0) {
    }
    if (chown(header->name, octal_to_string(header->uid), octal_to_string(header->gid)) < 0) {
    }
    return 1;
}

int extract_symlink(struct Header *header, FILE *out_file) {

    /* Set some info for the file */
    symlink(header->linkname, header->name);
    chmod(header->name, octal_to_string(header->mode));
    if (chown(header->name, octal_to_string(header->uid), octal_to_string(header->gid)) < 0) {
    }
    return 1;
}

int extract_directory(struct Header *header, FILE *out_file) {
    
    /* Set some info for the file */
    mkdir(header->name, octal_to_string(header->mode));
    if (chown(header->name, octal_to_string(header->uid), octal_to_string(header->gid)) < 0) {
    }
    return 1;
}

int extraction(char *file_name, struct Header* header, FILE *archive, int verbose_flag) {
    int result = 0;

    /* If verbose flag is set, print */
    if(verbose_flag) {
        printf("%s\n", file_name);
    }

    /* Check which file type it is, and call a function based on that */
    switch(header->typeflag) {
        case REGULAR_FILE:
            if (extract_file(file_name, header, archive) == 0) {
                perror("Error extraction");
                result = 1;
            }
            break;
        case SYM_LINK:
            if (extract_symlink(header, archive) == 0) {
                perror("Error extraction");
                result = 1;
            }
            break;
        case DIRECTORY:
            if (extract_directory(header, archive) == 0) {
                perror("Error extraction");
                result = 1;
            }
            break;
    }
    return result;
}

int extract_archive(int argc, char **argv, int verbose_flag) {
    FILE *archive;
    struct Header *header = {0};
    int i;
    int result;
    char file_name[NAME_SIZE_BYTES + PREFIX_SIZE_BYTES + 2];

    archive = fopen(argv[2], "rb");
    while (read_header(archive, header) != 0) {
        create_name(file_name, header->prefix, header->name);

        if (argc < 4) {
            result = extraction(file_name, header, archive, verbose_flag);
        } else {
            for (i = 3; i < argc; i++) {
                if (check_file(file_name, argv[i]) == 0) {
                    result = extraction(file_name, header, archive, verbose_flag);
                }
            }
        }
    }
    fclose(archive);
    return result;
}