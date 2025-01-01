#include <stdio.h>

#include "ast.h"
#include "log.h"

void pr_print_label(pr_label_t *label) {
  printf("Label name: %s, Label value: %s\n", label->name, label->value);
}

void pr_print_label_list(pr_label_t *label_list) {
  pr_label_t *cur_label = label_list;
  while (cur_label) {
    pr_print_label(cur_label);
    cur_label = cur_label->next;
  }
}

void pr_print_metric_entry(pr_metric_entry_t *metric) {
  printf("Metric name: %s\n", metric->name);
  pr_print_label_list(metric->labels);
  printf("Metric value: %f\n", metric->value);
  if (metric->timestamp->has_value) {
    printf("Metric timestamp: %ld\n", metric->timestamp->value);
  }
  printf("\n");
}

void pr_print_comment_entry(pr_comment_entry_t *comment) {
  printf("Comment: %s\n\n", comment->text);
}

void pr_print_type_entry(pr_type_entry_t *type) {
  printf("Type name: %s\n", type->name);
  switch (type->tp) {
  case (COUNTER): {
    printf("Type value: counter\n\n");
    break;
  }
  case (GAUGE): {
    printf("Type value: gauge\n\n");
    break;
  }
  case (HISTOGRAM): {
    printf("Type value: histogram\n\n");
    break;
  }
  case (SUMMARY): {
    printf("Type value: summary\n\n");
    break;
  }
  case (UNTYPED): {
    printf("Type value: untyped\n\n");
    break;
  }
  }
}

void pr_print_help_entry(pr_help_entry_t *help) {
  printf("Help name: %s\n", help->name);
  printf("Help hint: %s\n\n", help->hint);
}

void pr_print_entry(pr_entry_t *entry) {
  switch (entry->tp) {
  case (METRIC_ENTRY): {
    pr_print_metric_entry(entry->body.metric);
    break;
  }
  case (COMMENT_ENTRY): {
    pr_print_comment_entry(entry->body.comment);
    break;
  }
  case (TYPE_ENTRY): {
    pr_print_type_entry(entry->body.type);
    break;
  }
  case (HELP_ENTRY): {
    pr_print_help_entry(entry->body.help);
    break;
  }
  }
}

void pr_print_metric(pr_metric_t *metric) {
  printf("Metric:\n");
  pr_print_label_list(metric->labels);
  printf("Value: %f\n", metric->value);
  if (metric->timestamp->has_value) {
    printf("Metric timestamp: %ld\n", metric->timestamp->value);
  }
}

void pr_print_metric_list(pr_metric_t *metric_list) {
  pr_metric_t *cur_metric = metric_list;
  while (cur_metric) {
    pr_print_metric(cur_metric);
    cur_metric = cur_metric->next;
  }
}

void pr_print_metric_family(pr_metric_family_t *metric_family) {
  printf("Metric family item:\n");
  printf("Name: %s\n", metric_family->name);
  switch (metric_family->tp) {
  case (COUNTER): {
    printf("Type: counter\n");
    break;
  }
  case (GAUGE): {
    printf("Type: gauge\n");
    break;
  }
  case (HISTOGRAM): {
    printf("Type: histogram\n");
    break;
  }
  case (SUMMARY): {
    printf("Type value: summary\n");
    break;
  }
  case (UNTYPED): {
    printf("Type value: untyped\n");
    break;
  }
  }
  printf("Help: %s\n", metric_family->help);
  pr_print_metric_list(metric_family->metric_list);
  printf("\n");
}

void pr_print_comment(pr_comment_t *comment) {
  printf("Comment item:\n");
  printf("%s\n\n", comment->text);
}

void pr_print_item(pr_item_t *item) {
  switch (item->tp) {
  case (METRIC_FAMILY_ITEM): {
    pr_print_metric_family(item->body.metric_family);
    break;
  }
  case (COMMENT_ITEM): {
    pr_print_comment(item->body.comment);
    break;
  }
  }
}

void pr_print_item_list(pr_item_list_t *item_list) {
  pr_item_t *cur_item = item_list->begin;
  while (cur_item) {
    pr_print_item(cur_item);
    cur_item = cur_item->next;
  }
}
