#include "cmd.h"
#include "db.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE 50
#define MAX_BODY 100

static int get_line(char *arr, int limit);
static int read_cb(void *NotUsed, int argc, char **argv, char **azColName);

void add_item(void) {
  sqlite3 *db;
  char title[MAX_TITLE + 1], body[MAX_BODY + 1],
      sql[sizeof(title) + sizeof(body) + 50], *err_msg = NULL;
  int conn;

  printf("What do you want to add? ");
  get_line(title, MAX_TITLE);
  printf("Write down the details (Optional): ");
  get_line(body, MAX_BODY);
  sprintf(sql, "INSERT INTO todo (title, body) VALUES ('%s', '%s')", title,
          body);

  db = db_init();
  conn = sqlite3_exec(db, sql, 0, 0, &err_msg);
  if (conn != SQLITE_OK) {
    fprintf(stderr, "Error: %s\n", err_msg);
    sqlite3_free(err_msg);
    exit(EXIT_FAILURE);
  }

  printf("\nSuccessfully added title: %s body: %s\n\n", title, body);
  sqlite3_close(db);
}

void read_items(void) {
  sqlite3 *db = db_init();
  char *err_msg = NULL, *sql = "SELECT * FROM todo";
  int conn;

  printf("ID\tTitle\tBody\tCreated At\n");
  conn = sqlite3_exec(db, sql, read_cb, 0, &err_msg);
  if (conn != SQLITE_OK) {
    fprintf(stderr, "Error: %s\n", err_msg);
    sqlite3_free(err_msg);
    exit(EXIT_FAILURE);
  }
  printf("\n");

  sqlite3_close(db);
}

void delete_item(void) {
  sqlite3 *db;
  int delete_id, conn;
  char sql[50], *err_msg = NULL;

  printf("Write the id of the item you want to delete: ");
  if (scanf("%d", &delete_id) < 1) {
    fprintf(stderr, "Error: Invalid input. Please enter an integer.\n");
    exit(EXIT_FAILURE);
  }

  sprintf(sql, "DELETE FROM todo WHERE id = %d", delete_id);

  db = db_init();
  conn = sqlite3_exec(db, sql, 0, 0, &err_msg);
  if (conn != SQLITE_OK) {
    fprintf(stderr, "Error: %s\n", err_msg);
    sqlite3_free(err_msg);
    exit(EXIT_FAILURE);
  }

  if (sqlite3_changes(db) == 0) {
    fprintf(stderr, "There's no item with the id %d\n", delete_id);
    exit(EXIT_FAILURE);
  }

  printf("Successfully deleted id: %d\n", delete_id);
  sqlite3_close(db);
}

static int get_line(char *arr, int limit) {
  int ch, len = 0;

  while ((ch = getchar()) != '\n') {
    if (len == limit) {
      break;
    }
    *arr++ = ch;
    len++;
  }

  *arr++ = '\0';
  return len;
}

static int read_cb(void *NotUsed, int argc, char **argv, char **azColName) {
  (void)NotUsed, (void)azColName;
  for (int i = 0; i < argc; i++) {
    printf("%s\t", argv[i] ? argv[i] : "");
  }
  printf("\n");
  return 0;
}