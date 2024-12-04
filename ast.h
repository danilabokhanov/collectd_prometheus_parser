#ifndef AST_H
#define AST_H

#include <stdint.h>

typedef struct label {
  char *name;
  char *value;
  struct label *next;
} label;

typedef struct metric {
  char *name;
  label *labels;
  double value;
  int64_t timestamp;
} metric;

typedef struct comment {
  char *text;
} comment;

typedef struct node {
  enum { METRIC_NODE, COMMENT_NODE } tp;
  union {
    metric *metric;
    comment *comment;
  } body;
  struct node *next;
} node;

typedef struct tree {
  node *begin;
} tree;

label *create_label(const char *name, const char *value);
label *add_label_to_list(label *list, label *label);
metric *create_metric(const char *name, label *labels, double value,
                      int64_t timestamp);
comment *create_comment(const char *text);
node *create_metric_node(metric *metric);
node *create_comment_node(comment *comment);

tree *create_empty_tree();
void add_node_to_tree(tree *tree, node *node);
void print_tree(tree *tree);
void delete_tree(tree *tree);

#endif