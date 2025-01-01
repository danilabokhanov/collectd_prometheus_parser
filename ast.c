#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pr_label_t *pr_create_label(char *name, char *value) {
  pr_label_t *label = malloc(sizeof(*label));
  if (!label) {
    perror("Couldn't allocate memory for label\n");
    exit(EXIT_FAILURE);
  }
  label->name = name;
  label->value = value;
  label->next = NULL;
  return label;
}

pr_timestamp_t *pr_create_empty_timestamp() {
  pr_timestamp_t *timestamp = malloc(sizeof(*timestamp));
  if (!timestamp) {
    perror("Couldn't allocate memory for timestamp\n");
    exit(EXIT_FAILURE);
  }
  timestamp->has_value = false;
  return timestamp;
}

pr_timestamp_t *pr_create_value_timestamp(int64_t value) {
  pr_timestamp_t *timestamp = malloc(sizeof(*timestamp));
  if (!timestamp) {
    perror("Couldn't allocate memory for timestamp\n");
    exit(EXIT_FAILURE);
  }
  timestamp->has_value = true;
  timestamp->value = value;
  return timestamp;
}

pr_label_t *pr_add_label_to_list(pr_label_t *list, pr_label_t *label) {
  if (!list) {
    return label;
  }
  label->next = list;
  return label;
}

pr_entry_t *pr_create_entry_from_metric(pr_metric_entry_t *metric) {
  pr_entry_t *entry = malloc(sizeof(*entry));
  if (!entry) {
    perror("Couldn't allocate memory for metric entry\n");
    exit(EXIT_FAILURE);
  }
  entry->tp = METRIC_ENTRY;
  entry->body.metric = metric;
  return entry;
}

pr_entry_t *pr_create_entry_from_comment(pr_comment_entry_t *comment) {
  pr_entry_t *entry = malloc(sizeof(*entry));
  if (!entry) {
    perror("Couldn't allocate memory for comment entry\n");
    exit(EXIT_FAILURE);
  }
  entry->tp = COMMENT_ENTRY;
  entry->body.comment = comment;
  return entry;
}

pr_entry_t *pr_create_entry_from_type(pr_type_entry_t *type) {
  pr_entry_t *entry = malloc(sizeof(*entry));
  if (!entry) {
    perror("Couldn't allocate memory for type entry\n");
    exit(EXIT_FAILURE);
  }
  entry->tp = TYPE_ENTRY;
  entry->body.type = type;
  return entry;
}

pr_entry_t *pr_create_entry_from_help(pr_help_entry_t *help) {
  pr_entry_t *entry = malloc(sizeof(*entry));
  if (!entry) {
    perror("Couldn't allocate memory for help entry\n");
    exit(EXIT_FAILURE);
  }
  entry->tp = HELP_ENTRY;
  entry->body.help = help;
  return entry;
}

pr_metric_entry_t *pr_create_metric_entry(char *name, pr_label_t *labels,
                                          double value,
                                          pr_timestamp_t *timestamp) {
  pr_metric_entry_t *metric = malloc(sizeof(*metric));
  if (!metric) {
    perror("Couldn't allocate memory for metric\n");
    exit(EXIT_FAILURE);
  }
  metric->name = name;
  metric->labels = labels;
  metric->value = value;
  metric->timestamp = timestamp;
  return metric;
}

pr_comment_entry_t *pr_create_comment_entry(char *text) {
  pr_comment_entry_t *comment = malloc(sizeof(*comment));
  if (!comment) {
    perror("Couldn't allocate memory for comment\n");
    exit(EXIT_FAILURE);
  }
  comment->text = text;
  return comment;
}

pr_type_entry_t *pr_create_type_entry(char *name, pr_metric_type_t tp) {
  pr_type_entry_t *type = malloc(sizeof(*type));
  if (!type) {
    perror("Couldn't allocate memory for type\n");
    exit(EXIT_FAILURE);
  }
  type->name = name;
  type->tp = tp;
  return type;
}

pr_help_entry_t *pr_create_help_entry(char *name, char *hint) {
  pr_help_entry_t *help = malloc(sizeof(*help));
  if (!help) {
    perror("Couldn't allocate memory for help\n");
    exit(EXIT_FAILURE);
  }
  help->name = name;
  help->hint = hint;
  return help;
}

pr_item_list_t *pr_create_item_list() {
  pr_item_list_t *item_list = malloc(sizeof(*item_list));
  if (!item_list) {
    perror("Couldn't allocate memory for item list\n");
    exit(EXIT_FAILURE);
  }
  item_list->begin = NULL;
  return item_list;
}

pr_item_t *pr_create_metric_family_item() {
  pr_item_t *item = malloc(sizeof(*item));
  if (!item) {
    perror("Couldn't allocate memory for item\n");
    exit(EXIT_FAILURE);
  }
  pr_metric_family_t *metric_family = malloc(sizeof(*metric_family));
  if (!metric_family) {
    perror("Couldn't allocate memory for metric family\n");
    exit(EXIT_FAILURE);
  }
  metric_family->name = NULL;
  metric_family->help = NULL;
  metric_family->tp = UNTYPED;
  metric_family->metric_list = NULL;
  item->tp = METRIC_FAMILY_ITEM;
  item->body.metric_family = metric_family;
  item->next = NULL;
  return item;
}

pr_item_t *pr_create_comment_item(char *text) {
  pr_item_t *item = malloc(sizeof(*item));
  if (!item) {
    perror("Couldn't allocate memory for item\n");
    exit(EXIT_FAILURE);
  }
  pr_comment_t *comment = malloc(sizeof(*comment));
  comment->text = strdup(text);
  item->tp = COMMENT_ITEM;
  item->body.comment = comment;
  item->next = NULL;
  return item;
}

char *pr_get_cur_family_name(pr_item_list_t *item_list) {
  pr_item_t *item = item_list->begin;
  if (item && item->tp == METRIC_FAMILY_ITEM) {
    return item->body.metric_family->name;
  }
  return NULL;
}

void pr_add_item_to_item_list(pr_item_list_t *item_list, pr_item_t *item) {
  item->next = item_list->begin;
  item_list->begin = item;
}

pr_label_t *pr_copy_label_list(pr_label_t *label_list) {
  if (!label_list) {
    return NULL;
  }
  pr_label_t *label_list_copy = malloc(sizeof(*label_list_copy));
  if (!label_list_copy) {
    perror("Couldn't allocate memory for label list copy\n");
    exit(EXIT_FAILURE);
  }
  label_list_copy->name = strdup(label_list->name);
  label_list_copy->value = strdup(label_list->value);
  label_list_copy->next = pr_copy_label_list(label_list->next);
  return label_list_copy;
}

pr_timestamp_t *pr_copy_timestamp(pr_timestamp_t *timestamp) {
  pr_timestamp_t *timestamp_copy = malloc(sizeof(*timestamp_copy));
  if (!timestamp_copy) {
    perror("Couldn't allocate memory for label timestamp copy\n");
    exit(EXIT_FAILURE);
  }
  timestamp_copy->has_value = timestamp->has_value;
  timestamp_copy->value = timestamp->value;
  return timestamp_copy;
}

pr_metric_t *pr_create_metric_from_entry(pr_metric_entry_t *metric_entry) {
  pr_metric_t *metric = malloc(sizeof(*metric));
  if (!metric) {
    perror("Couldn't allocate memory for metric\n");
    exit(EXIT_FAILURE);
  }
  metric->labels = pr_copy_label_list(metric_entry->labels);
  metric->value = metric_entry->value;
  metric->timestamp = pr_copy_timestamp(metric_entry->timestamp);
  metric->next = NULL;
  return metric;
}

void pr_add_metric_to_metric_family(pr_metric_family_t *metric_family,
                                    pr_metric_t *metric) {
  metric->next = metric_family->metric_list;
  metric_family->metric_list = metric;
}

void pr_add_entry_to_item_list(pr_item_list_t *item_list, pr_entry_t *entry) {
  if (entry->tp != COMMENT_ENTRY) {
    char *metric_family_name = pr_get_cur_family_name(item_list);
    if (!metric_family_name || strcmp(metric_family_name, entry->body.metric->name)) {
      pr_item_t *new_metric_family = pr_create_metric_family_item();
      pr_add_item_to_item_list(item_list, new_metric_family);
    }
    pr_metric_family_t *metric_family = item_list->begin->body.metric_family;
    switch (entry->tp) {
    case (METRIC_ENTRY): {
      pr_metric_entry_t *metric_entry = entry->body.metric;
      if (!metric_family->name) {
        metric_family->name = strdup(metric_entry->name);
      }
      pr_metric_t *new_metric = pr_create_metric_from_entry(metric_entry);
      pr_add_metric_to_metric_family(metric_family, new_metric);
      break;
    }
    case (TYPE_ENTRY): {
      pr_type_entry_t *type_entry = entry->body.type;
      if (!metric_family->name) {
        metric_family->name = strdup(type_entry->name);
      }
      metric_family->tp = type_entry->tp;
      break;
    }
    case (HELP_ENTRY): {
      pr_help_entry_t *help_entry = entry->body.help;
      if (!metric_family->name) {
        metric_family->name = strdup(help_entry->name);
      }
      if (!metric_family->help) {
        metric_family->help = strdup(help_entry->hint);
      }
      break;
    }
    case (COMMENT_ENTRY): {
      break;
    }
    }
  } else {
    pr_item_t *new_comment = pr_create_comment_item(entry->body.comment->text);
    pr_add_item_to_item_list(item_list, new_comment);
  }
}

void pr_delete_label_list(pr_label_t *label_list) {
  if (!label_list) {
    return;
  }
  free(label_list->name);
  free(label_list->value);
  pr_delete_label_list(label_list->next);
  free(label_list);
}

void pr_delete_metric(pr_metric_t *metric) {
  pr_delete_label_list(metric->labels);
  free(metric->timestamp);
  free(metric);
}

void pr_delete_metric_list(pr_metric_t *metric_list) {
  if (!metric_list) {
    return;
  }
  pr_delete_metric_list(metric_list->next);
  pr_delete_metric(metric_list);
}

void pr_delete_metric_family(pr_metric_family_t *metric_family) {
  free(metric_family->name);
  free(metric_family->help);
  pr_delete_metric_list(metric_family->metric_list);
}

void pr_delete_comment_entry(pr_comment_entry_t *comment) {
  free(comment->text);
  free(comment);
}

void pr_delete_comment(pr_comment_t *comment) {
  pr_delete_comment_entry(comment);
}

void pr_delete_item(pr_item_t *item) {
  switch (item->tp) {
  case (METRIC_FAMILY_ITEM): {
    pr_delete_metric_family(item->body.metric_family);
    break;
  }
  case (COMMENT_ITEM): {
    pr_delete_comment(item->body.comment);
    break;
  }
  }
}

void pr_delete_item_list(pr_item_list_t *item_list) {
  pr_item_t *cur_item = item_list->begin;
  while (cur_item) {
    pr_item_t *next_item = cur_item->next;
    pr_delete_item(cur_item);
    cur_item = next_item;
  }
}

void pr_delete_metric_entry(pr_metric_entry_t *metric) {
  free(metric->name);
  pr_delete_label_list(metric->labels);
  free(metric->timestamp);
  free(metric);
}

void pr_delete_type_entry(pr_type_entry_t *type) {
  free(type->name);
  free(type);
}

void pr_delete_help_entry(pr_help_entry_t *help) {
  free(help->name);
  free(help->hint);
  free(help);
}

void pr_delete_entry(pr_entry_t *entry) {
  switch (entry->tp) {
  case (METRIC_ENTRY): {
    pr_delete_metric_entry(entry->body.metric);
    break;
  }
  case (COMMENT_ENTRY): {
    pr_delete_comment_entry(entry->body.comment);
    break;
  }
  case (TYPE_ENTRY): {
    pr_delete_type_entry(entry->body.type);
    break;
  }
  case (HELP_ENTRY): {
    pr_delete_help_entry(entry->body.help);
    break;
  }
  }
}
