#include "abonent.h"

void add(struct abonent **directory, char *name, char *sec_name, char *tel) {
  struct abonent *node = malloc(sizeof(struct abonent));

  sprintf(node->name, "%s", name);
  sprintf(node->second_name, "%s", sec_name);
  sprintf(node->tel, "%s", tel);

  node->next = NULL;

  if (*directory == NULL) {
    node->prev = NULL;
    *directory = node;
  } else {
    struct abonent *end = *directory;

    while (end->next != NULL)
      end = end->next;

    node->prev = end;
    end->next = node;
  }
}

struct abonent *search(struct abonent **directory, char *name) {
  if (directory == NULL)
    return NULL;

  struct abonent *node = *directory;

  while (node != NULL) {
    if (strcmp(node->name, name) == 0)
      return node;

    node = node->next;
  }

  return NULL;
}

void delete(struct abonent **directory, char *name, char *sec_name, char *tel) {
  if (directory == NULL)
    return;

  struct abonent *node = *directory;

  while (node->next != NULL) {
    if (strcmp(node->name, name) == 0 &&
        strcmp(node->second_name, sec_name) == 0 && strcmp(node->tel, tel) == 0)
      break;

    node = node->next;
  }

  if (node != NULL) {
    if (node->next != NULL)
      node->next->prev = node->prev;

    if (node->prev != NULL)
      node->prev->next = node->next;
    else
      *directory = node->next;

    free(node);
  } else {
    return;
  }
}

void print(struct abonent *directory) {
  while (directory != NULL) {
    printf("name: %s | second name: %s | tel: %s\n", directory->name,
           directory->second_name, directory->tel);

    directory = directory->next;
  }
}