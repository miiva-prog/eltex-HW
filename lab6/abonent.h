#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE_STRING 10

struct abonent {
  struct abonent *prev, *next;
  char name[SIZE_STRING];
  char second_name[SIZE_STRING];
  char tel[SIZE_STRING];
};

void add(struct abonent **directory, char *name, char *sec_name, char *tel);

struct abonent *search(struct abonent **directory, char *name);

void delete(struct abonent **directory, char *name, char *sec_name, char *tel);

void print(struct abonent *directory);