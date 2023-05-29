#include "cmd.h"
#include "db.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  if (argc == 1) {
    printf("Usage: todo [command]\n\n");
    printf(" add\n ls\n delete\b");
    exit(EXIT_SUCCESS);
  }

  char cmd[strlen(argv[1]) + 1];
  strcpy(cmd, argv[1]);
  printf("%s\n", cmd);

  if (strcmp(cmd, "add") == 0) {
    add_item();
  } else if (strcmp(cmd, "ls") == 0) {
    read_items();
  } else if (strcmp(cmd, "delete") == 0) {
    delete_item();
  } else {
    fprintf(stderr, "Unknown command %s\n", cmd);
    exit(EXIT_FAILURE);
  }

  return 0;
}
