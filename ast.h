#ifndef AST_H
#define AST_H

#include <stdbool.h>
#include <stdint.h>

typedef struct label {
  char *name;
  char *value;
  struct label *next;
} label;

enum metric_type { COUNTER, GAUGE, HISTOGRAM, SUMMARY, UNTYPED };

typedef struct timestamp {
  bool has_value;
  int64_t value;
} timestamp;

typedef struct metric {
  char *name;
  label *labels;
  double value;
  timestamp *timestamp;
} metric;

typedef struct comment {
  char *text;
} comment;

typedef struct type {
  char *name;
  enum metric_type tp;
} type;

typedef struct help {
  char *name;
  char *hint;
} help;

typedef struct node {
  enum { METRIC_NODE, COMMENT_NODE, TYPE_NODE, HELP_NODE } tp;
  union {
    metric *metric;
    comment *comment;
    type *type;
    help *help;
  } body;
  struct node *next;
} node;

typedef struct tree {
  node *begin;
} tree;

label *create_label(const char *name, const char *value);
timestamp *create_empty_timestamp();
timestamp *create_value_timestamp(int64_t value);
label *add_label_to_list(label *list, label *label);
metric *create_metric(const char *name, label *labels, double value,
                      timestamp *timestamp);
comment *create_comment(const char *text);
type *create_type(const char *name, enum metric_type tp);
help *create_help(const char *name, const char *hint);
node *create_metric_node(metric *metric);
node *create_comment_node(comment *comment);
node *create_type_node(type *node);
node *create_help_node(help *help);
tree *create_empty_tree();
void add_node_to_tree(tree *tree, node *node);
void print_tree(tree *tree);
void delete_tree(tree *tree);

#endif
