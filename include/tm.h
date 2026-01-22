#ifndef TM_H
#define TM_H

#include <stddef.h>
#include <stdint.h>

#ifndef TM_PARAMS 
  #define STATES 6
  #define SYMBOLS 2
  #define TAPE_SIZE 4
#endif


typedef enum {
  LEFT = -1,
  RIGHT = 1
} DIRECTION;

typedef struct {
  uint8_t write;
  DIRECTION dir;
  char state;
} Instruction_t;

typedef struct {
  uint64_t *tape;
  Instruction_t **rules;
  size_t step_index;
  size_t head_index;
  char state;
} NaiveTM_t;

// Reduced Configuration Tree (RCT)
typedef struct halting_signature {
  struct halting_signature **children;
  uint64_t *tape;
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

NaiveTM_t *init_naive_tm(Instruction_t **rules);

#endif