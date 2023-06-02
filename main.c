#include "cmd.h"
#include "db.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  if (argc == 1) {
    printf("\nUsage: todo [command]\n\n");
    printf("topic: add a new topic\n");
    printf("tls: list all the topics\n");
    printf("trm: remove a topic\n");
    printf("lsbt: print every stored item by topic");
    printf("add: add a new item\n");
    printf("ls: print every stored item\n");
    printf("rm: remove an item\n\n");
    exit(EXIT_SUCCESS);
  }

  char cmd[strlen(argv[1]) + 1];
  strcpy(cmd, argv[1]);

  if (strcmp(cmd, "add") == 0) {
    add_item();
  } else if (strcmp(cmd, "ls") == 0) {
    read_items();
  } else if (strcmp(cmd, "rm") == 0) {
    delete_item();
  } else if (strcmp(cmd, "topic") == 0) {
    new_topic();
  } else if (strcmp(cmd, "tls") == 0) {
    list_topic();
  } else if (strcmp(cmd, "trm") == 0) {
    rm_topic();
  } else if (strcmp(cmd, "lsbt") == 0) {
    filter_by_topic();
  } else {
    fprintf(stderr, "Unknown command %s\n", cmd);
    exit(EXIT_FAILURE);
  }

  return 0;
}
