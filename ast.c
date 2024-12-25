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

timestamp *create_empty_timestamp() {
  timestamp *timestamp = malloc(sizeof(*timestamp));
  if (!timestamp) {
    perror("Couldn't allocate memory for timestamp\n");
    exit(EXIT_FAILURE);
  }
  timestamp->has_value = false;
  return timestamp;
}

timestamp *create_value_timestamp(int64_t value) {
  timestamp *timestamp = malloc(sizeof(*timestamp));
  if (!timestamp) {
    perror("Couldn't allocate memory for timestamp\n");
    exit(EXIT_FAILURE);
  }
  timestamp->has_value = true;
  timestamp->value = value;
  return timestamp;
}

label *add_label_to_list(label *list, label *label) {
  if (!list) {
    return label;
  }
  label->next = list;
  return label;
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

node *create_type_node(type *type) {
  node *node = malloc(sizeof(*node));
  if (!node) {
    perror("Couldn't allocate memory for type node\n");
    exit(EXIT_FAILURE);
  }
  node->tp = TYPE_NODE;
  node->body.type = type;
  node->next = NULL;
  return node;
}

node *create_help_node(help *help) {
  node *node = malloc(sizeof(*node));
  if (!node) {
    perror("Couldn't allocate memory for help node\n");
    exit(EXIT_FAILURE);
  }
  node->tp = HELP_NODE;
  node->body.help = help;
  node->next = NULL;
  return node;
}

metric *create_metric(const char *name, label *labels, double value,
                      timestamp *timestamp) {
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

comment *create_comment(const char *text) {
  comment *comment = malloc(sizeof(*comment));
  if (!comment) {
    perror("Couldn't allocate memory for comment\n");
    exit(EXIT_FAILURE);
  }
  comment->text = strdup(text);
  return comment;
}

type *create_type(const char *name, enum metric_type tp) {
  type *type = malloc(sizeof(*type));
  if (!type) {
    perror("Couldn't allocate memory for type\n");
    exit(EXIT_FAILURE);
  }
  type->name = strdup(name);
  type->tp = tp;
  return type;
}

help *create_help(const char *name, const char *hint) {
  help *help = malloc(sizeof(*help));
  if (!help) {
    perror("Couldn't allocate memory for help\n");
    exit(EXIT_FAILURE);
  }
  help->name = strdup(name);
  help->hint = strdup(hint);
  return help;
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

char *get_str_with_escaped_chars(char *text) { char *iter = text; }

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
  if (metric->timestamp->has_value) {
    printf("Metric timestamp: %ld\n", metric->timestamp->value);
  }
  printf("\n");
}

void print_comment(comment *comment) {
  printf("Comment: %s\n\n", comment->text);
}

void print_type(type *type) {
  printf("Type name: %s\n", type->name);
  if (type->tp == COUNTER) {
    printf("Type value: counter\n\n");
  } else if (type->tp == GAUGE) {
    printf("Type value: gauge\n\n");
  } else if (type->tp == HISTOGRAM) {
    printf("Type value: histogram\n\n");
  } else if (type->tp == SUMMARY) {
    printf("Type value: summary\n\n");
  } else if (type->tp == UNTYPED) {
    printf("Type value: untyped\n\n");
  }
}

void print_help(help *help) {
  printf("Help name: %s\n", help->name);
  printf("Help hint: %s\n\n", help->hint);
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
    case (TYPE_NODE): {
      print_type(cur_node->body.type);
      break;
    }
    case (HELP_NODE): {
      print_help(cur_node->body.help);
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
  free(metric->timestamp);
  free(metric);
}

void delete_comment(comment *comment) {
  free(comment->text);
  free(comment);
}

void delete_type(type *type) {
  free(type->name);
  free(type);
}

void delete_help(help *help) {
  free(help->name);
  free(help->hint);
  free(help);
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
    case (TYPE_NODE): {
      delete_type(cur_node->body.type);
      break;
    }
    case (HELP_NODE): {
      delete_help(cur_node->body.help);
    }
    }
    free(cur_node);
    cur_node = nxt_node;
  }
}
