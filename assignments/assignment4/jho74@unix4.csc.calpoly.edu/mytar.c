
#include "mytar.h"


int check_options(char *options) {
    int sum = 0;
    int i;
    char option_error[] = "./mytar: you may only choose one of the 'ctx' options.";

    /* Loop through every options given */
    for (i = 0; i < strlen(options); i++) {
        /* Check if the option is valid */
        if(options[i] != 'c' || options[i] != 't' || options[i] != 'x' || options[i] != 'v' || options[i] != 'f' || options[i] != 'S') {
            printf("./mytar: uncrecognized option '%c'\n", options[i]);
            return '\0';
        } else {
            /* Add value of the options up */
            sum += (int)(options[i]);
        }
    }

    /*Check the total value of the options with each case and operate on that */
    switch(sum) {
        case (int)('c' + 't'):
            fprintf(stderr, "%s", option_error);
            return '\0';
        case (int)('c' + 'x'):
            fprintf(stderr, "%s", option_error);
            return '\0';
        case (int)('t' + 'x'):
            fprintf(stderr, "%s", option_error);
            return '\0';
        case (int)('c' + 't' + 'x'):
            fprintf(stderr, "%s", option_error);
            return '\0';
        case (int)('c' + 'f'):
            return (int)('c' + 'f');
        case (int)('c' + 'v' + 'f'):
            return (int)('c' + 'v' + 'f');
        case (int)('c' + 'f' + 'S'):
            return (int)('c' + 'f' + 'S');
        case (int)('c' + 'v' + 'f' + 'S'):
            return (int)('c' + 'v' + 'f' + 'S');
        case (int)('t' + 'f'):
            return (int)('t' + 'f');
        case (int)('t' + 'v' + 'f'):
            return (int)('t' + 'v' + 'f');
        case (int)('t' + 'f' + 'S'):
            return (int)('t' + 'f' + 'S');
        case (int)('t' + 'v' + 'f' + 'S'):
            return (int)('t' + 'v' + 'f' + 'S');
        case (int)('x' + 'f'):
            return (int)('x' + 'f');
        case (int)('x' + 'v' + 'f'):
            return (int)('x' + 'v' + 'f');
        case (int)('x' + 'f' + 'S'):
            return (int)('x' + 'f' + 'S');
        case (int)('x' + 'v' + 'f' + 'S'):
            return (int)('x' + 'v' + 'f' + 'S');
        default:
            return '\0';
    }
    return sum;
}

int octal_to_string(char *octal) {
    int total = 0;
    int octal_length = strlen(octal);
    int i;

    /* Loops through the octal and returns the decimal */
    for (i = 0; i < octal_length; i++) {
        total += (int)(octal[i] - '0') * pow(8.0, (double)(octal_length - i - 1));
    }
    return total;
}

int read_header(FILE *archive, struct Header *header) {
    /*reads the archive, 1 block, and returns the checksum to check when it reaches the end */
    fread(header, BLOCK_SIZE_BYTES, 1, archive);
    return octal_to_string(header->checksum);
}

int check_file(char *file, char *compare) {
    char file_buffer[strlen(compare) + 1];
    char compare_buffer[strlen(compare) + 1];
    
    /* sets the two buffers to all '\0' */
    memset(file_buffer, '\0', strlen(compare) + 1);
    memset(compare_buffer, '\0', strlen(compare) + 1);
    /* Copies the file name into the file buffer with the same number of characters as its compared to name */
    strncpy(file_buffer, file, strlen(compare));
    /* Copies the compared to name to the compared buffer */
    strcpy(compare_buffer, compare);
    /* return the compared result */
    return strcmp(file_buffer, compare_buffer);
}

void create_name(char *buffer, char *prefix, char *name) {
    char *append;

    /* Checks which case this is */
    if (strlen(prefix) == 0){
        /* If prefix is empty, name is name */
        strncpy(buffer, name, strlen(name));
    } else {
        /* If prefix is not empty, name is prefix appended with '/' and append with name */
        strncpy(buffer, prefix, strlen(prefix));
        buffer[strlen(prefix)] = '/';
        append = buffer + strlen(prefix) + 1;
        strncpy(append, name, strlen(name));
    }
}


char *append_name(char *directory_name, char *to_append) {
    char *new_name;
    char *append;

    /* Malloc new space for the new name with the correct size */
    new_name = malloc(sizeof(*new_name) + strlen(directory_name) + strlen(to_append) + 1);
    /* Copy the old name to new_name */
    strcpy(new_name, directory_name);
    /* Set the the next space after the old name to be a pointer to the rest of the name */
    append = new_name + strlen(directory_name);
    /* copy the new portion of the name to the pointer */
    strcpy(append, to_append);
    return new_name;
}


int main(int argc, char *argv[]) {
    int options;

    /* Check the correct number of arguments is passed */
    if (argc < 2) {
        perror("usage: mytar [ctxvS]f tarfile [ path [ ... ] ]\n");
        return 1;
    } else if ((options = check_options(argv[1])) == '\0') {
        /* Determine what combination of options was passed and if it was valid */
        perror("usage: ./mytar [ctxvS]f tarfile [ path [ ... ] ]\n");
        return 1;
    } else {
        /* If valid options, choose the option that was presented */
        switch(options) {
            case (int)'c':
                perror("Bad address\nwrite: Bad file descriptor\nBad file descriptor\n");
                return 1;
            case (int)'t':
                perror("Bad address\n");
                return 1;
            case (int)'x':
                perror("Bad address\n");
                return 1;
            case (int)('c' + 'f'):
                return create_archive(argc, argv, 0 , 0);
            case (int)('c' + 'v' + 'f'):
                return create_archive(argc, argv, 1 , 0);
            case (int)('c' + 'f' + 'S'):
                return create_archive(argc, argv, 0 , 1);
            case (int)('c' + 'v' + 'f' + 'S'):
                return create_archive(argc, argv, 1 , 1);
            case (int)('t' + 'f'):
                return print_archive(argc, argv, 0);
            case (int)('t' + 'v' + 'f'):
                return print_archive(argc, argv, 1);
            case (int)('t' + 'f' + 'S'):
                return print_archive(argc, argv, 0);
            case (int)('t' + 'v' + 'f' + 'S'):
                return print_archive(argc, argv, 1);
            case (int)('x' + 'f'):
                return extract_archive(argc, argv, 0);
            case (int)('x' + 'v' + 'f'):
                return extract_archive(argc, argv, 1);
            case (int)('x' + 'f' + 'S'):
                return extract_archive(argc, argv, 0);
            case (int)('x' + 'v' + 'f' + 'S'):
                return extract_archive(argc, argv, 1);
        }
    }
    return 1;
}