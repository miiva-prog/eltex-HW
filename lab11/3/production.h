#include <stdbool.h>
#define PTH_PUCHASER 3
#define SIZE 5

struct market {
  int room;
  bool mode;
};

extern struct market showcase[SIZE];
extern int demand[PTH_PUCHASER];

int getrand(int min, int max);

void *buy(void *argument);

void *loading();

int sum();