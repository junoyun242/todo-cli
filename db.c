#include "db.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

sqlite3 *db_init() {
  sqlite3 *db;

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
      "CREATE TABLE IF NOT EXISTS topic (id INTEGER PRIMARY KEY NOT NULL, "
      "name TEXT NOT NULL, created_at datetime default current_timestamp)",
      0, 0, NULL);

  if (conn != SQLITE_OK) {
    fprintf(stderr, "error: %s\n", sqlite3_errmsg(db));
    exit(EXIT_FAILURE);
  }

  conn = sqlite3_exec(db,
                      "CREATE TABLE IF NOT EXISTS todo (id INTEGER PRIMARY KEY "
                      "NOT NULL, title TEXT NOT NULL, body TEXT, topic_id "
                      "INTEGER NOT NULL, created_at datetime "
                      "default current_timestamp, FOREIGN KEY "
                      "(topic_id) REFERENCES topic "
                      "(id))",
                      0, 0, NULL);

  if (conn != SQLITE_OK) {
    fprintf(stderr, "error: %s\n", sqlite3_errmsg(db));
    exit(EXIT_FAILURE);
  }

  return db;
}