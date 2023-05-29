#include "cmd.h"
#include "db.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_CONTENT 100

static int get_line(char *arr);

bool add_item(void) {
  sqlite3 *db;
  char *content, *sql, *err_msg = 0;
  content = malloc(sizeof(char) * MAX_CONTENT + 1);
  sql = malloc(sizeof(content) + 50);
  int conn;

  printf("What do you want to add? ");
  get_line(content);
  sprintf(sql, "INSERT INTO todo (title) VALUES ('%s')", content);

  db = db_init();
  conn = sqlite3_exec(db, sql, 0, 0, &err_msg);
  if (conn != SQLITE_OK) {
    fprintf(stderr, "Error: %s\n", err_msg);
    exit(EXIT_FAILURE);
  }

  printf("added");
  sqlite3_close(db);
  free(content);
  free(sql);

  return true;
}

void read_items(void) {}
void delete_item(void) {}

static int get_line(char *arr) {
  int ch, len = 0;

  while ((ch = getchar()) != '\n') {
    if (len == 100) {
      break;
    }
    *arr++ = ch;
    len++;
  }

  *arr++ = '\n';
  return len;
}