#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

/* Appends the directory name to an overall buffer in reverse order*/
void append_string(char *original, char *to_append) {
  int len_original = 0;
  int i;
  char *backslash = "/";

  if (original != NULL) {
    len_original = strlen(original);
  }
  for (i = strlen(to_append) - 1; i >= 0; i--) {
    original[len_original] = to_append[i];
    len_original++;
  }
  original[len_original] = *backslash;
  original[++len_original] = '\0';
}

/* Traverse the tree until root and appending each path name to a buffer*/
void traverse_diractory(char *path_name, int path_size, int inode) {
  DIR *directory;
  int prev_inode;
  struct stat stats;
  struct dirent *dirs;

  /* Check if chdir failed */
  if (chdir("..") != 0) {
    perror("mypwd");
    exit(0);
  }

  /* Check if root */
  stat(".", &stats);
  prev_inode = stats.st_ino;
  if (inode == prev_inode) {
    return;
  }

  /* Find the name of the current directory */
  if ((directory = opendir(".")) != NULL) {
    while ((dirs = readdir(directory)) != NULL) {
      if (inode == dirs->d_ino) {
        if ((strlen(dirs->d_name) + strlen(path_name)) > path_size) {
          printf("path too long\n");
          exit(2);
        }
        /* If found, append to a buffer */
        append_string(path_name, dirs->d_name);
        break;
      }
    }

    /* Print error and exit if directory not found */
    if (dirs == NULL) {
      printf("cannot get current directory\n");
      return exit(3);

    } else {
      /* Else, keep traversing */
      traverse_diractory(path_name, path_size, prev_inode);
    }
    closedir(directory);
    /* If current directory does not exist, print error and exit */
  } else {
    printf("cannot get current directory\n");
    return exit(3);
  }
}

/* print the path name, it is store in a buffer in reversed order */
void print_cwd(char *path_name, int path_size) {
  struct stat stats;
  int prev_inode;
  int i_node;
  int i;
  stat(".", &stats);
  i_node = stats.st_ino;
  stat("..", &stats);
  prev_inode = stats.st_ino;

  /* check if first directory is root */
  if (i_node == prev_inode) {
    printf("/\n");
    return;
  }

  /* If not, traverse directory */
  traverse_diractory(path_name, path_size, i_node);

  /* Print out path from buffer */
  for (i = strlen(path_name); i >= 0; i--) {
    printf("%c", path_name[i]);
  }
  printf("\n");
  return;
}

int main(int argc, char *argv[]) {
  char path_name[2048];

  if (argc > 1) {
    perror("mypwd\n");
    exit(1);
  }

  print_cwd(path_name, sizeof(path_name));
  return 0;
}