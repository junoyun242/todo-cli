#include "db.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

sqlite3 *db_init() {
  sqlite3 *db;
  sqlite3_stmt *res = NULL;
  char *err_msg;

  char *home_dir = getenv("HOME");
  if (home_dir == NULL) {
    fprintf(stderr, "Unable to access the home dir\n");
    exit(EXIT_FAILURE);
  }

  char file_loc[256];
  snprintf(file_loc, sizeof(file_loc), "%s/todo.db", home_dir);
  int conn = sqlite3_open(file_loc, &db);

  if (conn != SQLITE_OK) {
    fprintf(stderr, "Can't connect to db: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    exit(EXIT_FAILURE);
  }

  conn = sqlite3_exec(
      db,
      "CREATE TABLE IF NOT EXISTS todo (id INTEGER PRIMARY KEY "
      "NOT NULL, title TEXT NOT NULL, body TEXT, createdAt datetime "
      "default current_timestamp)",
      0, 0, &err_msg);

  if (conn != SQLITE_OK) {
    fprintf(stderr, "error: %s\n", err_msg);
    exit(EXIT_FAILURE);
  }

  return db;
}