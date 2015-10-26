/*
*	Create by Nicolas Constanty 21:13:21 23/10/15
*/
#include <getopt.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/wait.h>

#define BAD_ARG 1

typedef struct t_list {
  char          *path;
  long          id;
  long          old;
  struct t_list *next;
} f_list;

void putList(f_list **start, long id, long old, char *path) {

  f_list *elem;

  if (!(elem = malloc(sizeof(f_list)))) {
    printf("Fail Malloc !!\n");
    exit (1);
  }
  elem->id = id;
  elem->old = old;
  elem->path = path;
  elem->next = *start;
  *start = elem;
}

void usage(char *program_name) {

    printf ("\nUsage: %s [OPTION]... [FILE || DIRECTORY]...\n", program_name);
    fputs ("\
\n\
  -f, --file               file watch\n\
  -d, --dir                directory watch\n\n", stdout);
}

char *autoCompileOpt(int argc, char **argv) {

  int c;
  int i;

  i = 0;
  while (1) {
    int option_index = 0;
    static struct option long_options[] = {
      {"directory", required_argument, 0, 'd'},
      {"file",      required_argument, 0, 'f'},
      {"help",      no_argument,       0, 'h'},
      {0,           0,                 0,  0 }
    };

    c = getopt_long(argc, argv, "f:d:h",
    long_options, &option_index);
    if (c == -1) {
      if (i == 0) {
        if (argc > 1)
          return argv[1];
        return ".";
      }
      break;
    }
    switch (c) {
      case 0:
        printf("option %s", long_options[option_index].name);
        if (optarg)
          printf(" with arg %s", optarg);
        printf("\n");
        break;
      case 'f':
        return (optarg);
      case 'd':
        return (optarg);
      case 'h':
        usage(argv[0]);
        exit (1);
    }
    ++i;
  }
  if (optind < argc) {
    printf("non-option ARGV-elements: ");
    while (optind < argc)
    printf("%s ", argv[optind++]);
    printf("\n");
  }
  return ("");
}

void make() {

  int     pid;
  int     status;

  pid = fork();
  if (pid == 0) {
    system("make");
    system("make clean");
  }
  else
    {
      status = 0;
      wait(&status);
    }
}

int spyFile(char *file_path) {

  struct stat st;
  long old;

  if (stat(file_path, &st) != 0)
    return -1;
  old = st.st_mtime;
  while (42) {
    usleep(200000);
    if (stat(file_path, &st) != 0)
      return -1;
    if (st.st_mtime != old) {
      printf("\033[0;32mFile %s change :\e[0m\n", file_path);
      make();
      old = st.st_mtime;
    }
  }
}

int checkExist(f_list *fl, f_list *tmp, long id) {
  tmp = fl;
  while (tmp != NULL) {
    if (tmp->id == id)
      return (1);
    tmp = tmp->next;
  }
  return (0);
}

int getFiles(char *dir_path, f_list **fl, f_list *tmp) {

  DIR     *dir;
  struct  stat st;
  char    current_file[256];
  struct  dirent *dp;

  if ((dir = opendir(dir_path)) == NULL) {
    perror("Cannot open folder");
    exit (1);
  }
  while ((dp = readdir(dir)) != NULL) {

    if (dp->d_name[0] != '.' ) {
      strcpy(current_file, dir_path);
      strcat(current_file, dp->d_name);
      stat(current_file, &st);
      if (S_ISREG(st.st_mode) && !checkExist(*fl, tmp, st.st_ino)) {
        putList(fl, st.st_ino, st.st_mtime, strdup(current_file));
      }
      else if (S_ISDIR(st.st_mode)) {
        getFiles(strcat(current_file, "/"), fl, tmp);
      }
    }
  }
  return (closedir(dir));
}

int spyDir(char *dir_path) {

  f_list  *fl;
  f_list  *tmp;
  struct  stat st;

  fl = NULL;
  if (dir_path[strlen(dir_path) - 1] != '/')
    strcat(dir_path, "/");
  while (42) {
    if (getFiles(dir_path, &fl, tmp) == -1)
      return (-1);
    tmp = fl;
    while (tmp != NULL) {
      if (stat(tmp->path, &st) != 0)
        return (-1);
      if (st.st_mtime != tmp->old) {
        printf("\033[0;32mFile %s change :\e[0m\n", tmp->path);
        make();
        tmp->old = st.st_mtime;
      }
      tmp = tmp->next;
    }
    usleep(200000);
  }
}

int spy(char *opt) {

  struct stat st;
  if (stat(opt, &st) != 0)
    return (-1);
  if (S_ISREG(st.st_mode))
    return (spyFile(opt));
  else if (S_ISDIR(st.st_mode))
    return (spyDir(opt));
  printf("%s does not exist\n", opt);
  return (-1);
}

int main(int argc, char **argv)
{
  char *opt;

  opt = autoCompileOpt(argc, argv);
  printf("%s\n", opt);
  if (strcmp(opt, "") != 0)
    return (spy(opt));
  else
    printf ("Try --help, -h for more results\n");
  return (0);
}
