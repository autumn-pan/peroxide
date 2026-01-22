#include "tm.h"
#include <stdio.h>
#include <stdlib.h>

NaiveTM_t *init_naive_tm(Instruction_t **rules) {
  NaiveTM_t *tm = malloc(sizeof(tm));
  if(!tm) {
    fprintf(stderr, "Error: Failed to allocate memeory!");
    return NULL;
  }

  tm->tape = calloc(TAPE_SIZE, sizeof(uint64_t));
  tm->head_index = 32*TAPE_SIZE;

  tm->rules = rules;
  tm->state = 0;
  tm->step_index = 0;

  return tm;
}

