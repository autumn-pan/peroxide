#ifndef DECIDER_H
#define DECIDER_H

#include <stdint.h>
#include <stddef.h>

#include "tm.h"
#include "error.h"

#ifndef DECIDER_PARAMS
  #define CT_DEPTH 32
  #define HPR_TOLERANCE 4
#endif

// Reduced Configuration Tree (RCT)
typedef struct halting_signature {
  struct halting_signature **children;
  size_t num_children;
  uint64_t tape[TAPE_SIZE];
  size_t head_index;
  char state;
} HaltingSignature;

// Extended Backwards Reasoning Decider
typedef struct {
  HaltingSignature *halting_signature;
  HaltingSignature *current_node;
  HaltingSignature **unexplored_leaves;

  Instruction_t rules[STATES][SYMBOLS];
  size_t num_unsolved;
} EBRDecider;

RET_STATUS free_configuration_subtree(HaltingSignature *root);

#endif