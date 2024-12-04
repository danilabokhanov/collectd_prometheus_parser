#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

label *create_label(const char *name, const char *value) {
  label *label = malloc(sizeof(*label));
  if (!label) {
    perror("Couldn't allocate memory for label\n");
    exit(EXIT_FAILURE);
  }
  label->name = strdup(name);
  label->value = strdup(value);
  label->next = NULL;
  return label;
}

label *add_label_to_list(label *list, label *label) {
  if (!list) {
    return label;
  }
  label->next = list;
  return label;
}

metric *create_metric(const char *name, label *labels, double value,
                      int64_t timestamp) {
  metric *metric = malloc(sizeof(*metric));
  if (!metric) {
    perror("Couldn't allocate memory for metric\n");
    exit(EXIT_FAILURE);
  }
  metric->name = strdup(name);
  metric->labels = labels;
  metric->value = value;
  metric->timestamp = timestamp;
  return metric;
}

node *create_metric_node(metric *metric) {
  node *node = malloc(sizeof(*node));
  if (!node) {
    perror("Couldn't allocate memory for metric node\n");
    exit(EXIT_FAILURE);
  }
  node->tp = METRIC_NODE;
  node->body.metric = metric;
  node->next = NULL;
  return node;
}

node *create_comment_node(comment *comment) {
  node *node = malloc(sizeof(*node));
  if (!node) {
    perror("Couldn't allocate memory for comment node\n");
    exit(EXIT_FAILURE);
  }
  node->tp = COMMENT_NODE;
  node->body.comment = comment;
  node->next = NULL;
  return node;
}

comment *create_comment(const char *text) {
  comment *comment = malloc(sizeof(*comment));
  if (!comment) {
    perror("Couldn't allocate memory for comment\n");
    exit(EXIT_FAILURE);
  }
  comment->text = strdup(text);
  return comment;
}

tree *create_empty_tree() {
  tree *tree = malloc(sizeof(*tree));
  tree->begin = NULL;
  return tree;
}

void add_node_to_tree(tree *tree, node *node) {
  node->next = tree->begin;
  tree->begin = node;
}

void print_label(label *label) {
  printf("label name: %s, label value: %s\n", label->name, label->value);
}

void print_metric(metric *metric) {
  printf("Metric name: %s\n", metric->name);
  label *cur_label = metric->labels;
  while (cur_label) {
    print_label(cur_label);
    cur_label = cur_label->next;
  }
  printf("Metric value: %f\n", metric->value);
  printf("Metric timestamp: %ld\n\n", metric->timestamp);
}

void print_comment(comment *comment) {
  printf("Comment: %s\n\n", comment->text);
}

void print_tree(tree *tree) {
  node *cur_node = tree->begin;
  while (cur_node) {
    switch (cur_node->tp) {
    case (METRIC_NODE): {
      print_metric(cur_node->body.metric);
      break;
    }
    case (COMMENT_NODE): {
      print_comment(cur_node->body.comment);
      break;
    }
    }
    cur_node = cur_node->next;
  }
}

void delete_label(label *label) {
  if (!label) {
    return;
  }
  free(label->name);
  free(label->value);
  delete_label(label->next);
  free(label);
}

void delete_metric(metric *metric) {
  free(metric->name);
  delete_label(metric->labels);
  free(metric);
}

void delete_comment(comment *comment) {
  free(comment->text);
  free(comment);
}

void delete_tree(tree *tree) {
  node *cur_node = tree->begin;
  while (cur_node) {
    node *nxt_node = cur_node->next;
    switch (cur_node->tp) {
    case (METRIC_NODE): {
      delete_metric(cur_node->body.metric);
      break;
    }
    case (COMMENT_NODE): {
      delete_comment(cur_node->body.comment);
      break;
    }
    }
    free(cur_node);
    cur_node = nxt_node;
  }
}
