#include "ast.h"
#include "table.h"
#include "semantic.h"
#include "machine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

HTTable *global_table;
char **states = NULL;
int states_count = 0;
char **tape_alphabet = NULL;
int tape_alphabet_size = 0;
char **input_alphabet = NULL;
int input_alphabet_size = 0;
char *initial_state;
char **final_states = NULL;
int final_states_count = 0;
int max_depth = 100;

extern FILE *yyin;
extern int yyparse(void);
extern AST *root;

void print_usage(const char *prog_name) {
  fprintf(stderr, "Usage: %s [--verbose] <input_file> <input>\n", prog_name);
}

int main(int argc, char **argv) {
  int verbose = 0;
  int option_index = 0;
  int opt;

  static struct option long_options[] = {
    {"verbose", no_argument, 0, 'v'},
    {0, 0, 0, 0}
  };

  while ((opt = getopt_long(argc, argv, "v", long_options, &option_index)) != -1) {
    switch (opt) {
      case 'v':
        verbose = 1;
        break;
      default:
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }
  }

  if (optind + 2 > argc) {
    print_usage(argv[0]);
    exit(EXIT_FAILURE);
  }

  char *input_file_path = argv[optind];
  char *machine_input = argv[optind + 1];

  FILE *input_file = fopen(input_file_path, "r");
  if (!input_file) {
    perror("Error opening input file");
    exit(EXIT_FAILURE);
  }

  yyin = input_file;
  global_table = HT_create();

  int error = (yyparse() == 0 && root)
    ? check_semantics(root) : 1;

  if (error) {
    fclose(input_file);
    HT_free(global_table);
    exit(EXIT_FAILURE);
  }

  Machine* machine = M_create(global_table, initial_state, final_states, final_states_count);

  if (verbose) {
    printf("@states: ");
    for (int i = 0; i < states_count; ++i) {
      if (i != 0) printf(",");
      printf("%s", states[i]);
    }
    putchar('\n');

    printf("@tape_alphabet: ");
    for (int i = 0; i < tape_alphabet_size; ++i) {
      if (i != 0) printf(",");
      printf("%s", tape_alphabet[i]);
    }
    putchar('\n');

    printf("@input_alphabet: ");
    for (int i = 0; i < input_alphabet_size; ++i) {
      if (i != 0) printf(",");
      printf("%s", input_alphabet[i]);
    }
    putchar('\n');

    printf("@initial_state: %s\n", initial_state);

    printf("@final_states (%d): ", final_states_count);
    for (int i = 0; i < final_states_count; i++) {
      if (i != 0) printf(",");
      printf("%s", final_states[i]);
    }
    putchar('\n');

    printf("@max_depth: %d\n", max_depth);
    printf("\nTransitions:\n");
    HT_print(global_table);
    putchar('\n');
  }

  printf("Running the Turing Machine with the input '%s':\n", machine_input);
  M_run(machine, machine_input, max_depth);

  free(initial_state);
  AST_free(root);
  fclose(input_file);
  HT_free(global_table);

  return EXIT_SUCCESS;
}
