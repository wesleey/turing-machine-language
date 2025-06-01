#include "error.h"
#include <stdio.h>
#include <stdlib.h>

void ERR_print(const char *msg) {
  fprintf(stderr, "Error: %s\n", msg);
  exit(EXIT_FAILURE);
}

void ERR_printf(const char *msg, const char *arg) {
  fprintf(stderr, "Error: ");
  fprintf(stderr, msg, arg);
  fprintf(stderr, "\n");
  exit(EXIT_FAILURE);
}
