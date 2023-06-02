#include "cmd.h"
#include "db.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOPIC 10
#define MAX_TITLE 50
#define MAX_BODY 100

static int get_line(char *arr, int limit);
static int read_items_cb(void *NotUsed, int argc, char **argv,
                         char **azColName);
static int list_topic_db(void *NotUsed, int argc, char **argv,
                         char **azColName);

void new_topic(void) {
  sqlite3 *db;
  int conn;
  char new_topic[MAX_TOPIC + 1], sql[50];
  printf("Type a new topic name: ");
  get_line(new_topic, MAX_TOPIC);

  sprintf(sql, "INSERT INTO topic (name) VALUES ('%s')", new_topic);
  db = db_init();

  conn = sqlite3_exec(db, sql, 0, 0, NULL);

  if (conn != SQLITE_OK) {
    fprintf(stderr, "error: %s\n", sqlite3_errmsg(db));
    exit(EXIT_FAILURE);
  }

  printf("\nTopic %s successfully created!\n\n", new_topic);
  sqlite3_close(db);
}

void list_topic(void) {
  sqlite3 *db;
  char sql[30] = "SELECT name FROM topic";
  int conn;

  db = db_init();

  printf("List of topics\n\n");
  conn = sqlite3_exec(db, sql, list_topic_db, 0, NULL);

  if (conn != SQLITE_OK) {
    fprintf(stderr, "error: %s\n", sqlite3_errmsg(db));
    exit(EXIT_FAILURE);
  }

  printf("\n");
  sqlite3_close(db);
}

void add_item(void) {
  sqlite3 *db;
  sqlite3_stmt *stmt;
  char topic_name[MAX_TOPIC + 1], title[MAX_TITLE + 1], body[MAX_BODY + 1],
      sql[sizeof(title) + sizeof(body) + 50];
  int conn, topic_id;

  printf("What is the topic? ");
  get_line(topic_name, MAX_TOPIC);

  db = db_init();
  sprintf(sql, "SELECT id FROM topic WHERE name = '%s'", topic_name);
  conn = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (conn != SQLITE_OK) {
    fprintf(stderr, "error: %s\n", sqlite3_errmsg(db));
  }
  conn = sqlite3_step(stmt);
  if (conn == SQLITE_ROW) {
    topic_id = sqlite3_column_int(stmt, 0);
    printf("topic id: %d\n", topic_id);
  } else if (conn == SQLITE_DONE) {
    fprintf(stderr, "There's no topic named %s\n", topic_name);
    exit(EXIT_FAILURE);
  } else {
    fprintf(stderr, "error: %s\n", sqlite3_errmsg(db));
    exit(EXIT_FAILURE);
  }

  printf("What do you want to add? ");
  get_line(title, MAX_TITLE);
  printf("Write down the details (Optional): ");
  get_line(body, MAX_BODY);
  sprintf(sql,
          "INSERT INTO todo (title, body, topic_id) VALUES ('%s', '%s', %d)",
          title, body, topic_id);

  conn = sqlite3_exec(db, sql, 0, 0, NULL);
  if (conn != SQLITE_OK) {
    fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
    exit(EXIT_FAILURE);
  }

  printf("\nSuccessfully added title: %s body: %s\n\n", title, body);
  sqlite3_close(db);
}

void read_items(void) {
  sqlite3 *db = db_init();
  char *sql = "SELECT todo.id, title, body, topic.name, "
              "todo.created_at FROM todo JOIN "
              "topic ON todo.topic_id == topic.id";
  int conn;

  printf("ID\tTitle\tBody\tTopic\tCreated At\n");
  conn = sqlite3_exec(db, sql, read_items_cb, 0, NULL);
  if (conn != SQLITE_OK) {
    fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
    exit(EXIT_FAILURE);
  }
  printf("\n");

  sqlite3_close(db);
}

void delete_item(void) {
  sqlite3 *db;
  int delete_id, conn;
  char sql[50];

  printf("Write the id of the item you want to delete: ");
  if (scanf("%d", &delete_id) < 1) {
    fprintf(stderr, "Error: Invalid input. Please enter an integer.\n");
    exit(EXIT_FAILURE);
  }

  sprintf(sql, "DELETE FROM todo WHERE id = %d", delete_id);

  db = db_init();
  conn = sqlite3_exec(db, sql, 0, 0, NULL);
  if (conn != SQLITE_OK) {
    fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
    sqlite3_free(db);
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

static int read_items_cb(void *NotUsed, int argc, char **argv,
                         char **azColName) {
  (void)NotUsed, (void)azColName;
  for (int i = 0; i < argc; i++) {
    printf("%s\t", argv[i] ? argv[i] : "");
  }
  printf("\n");
  return 0;
}

static int list_topic_db(void *NotUsed, int argc, char **argv,
                         char **azColName) {
  (void)NotUsed, (void)azColName;

  for (int i = 0; i < argc; i++) {
    printf("%s\t", argv[i] ? argv[i] : "");
  }
  printf("\n");
  return 0;
}