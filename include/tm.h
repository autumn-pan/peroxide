#ifndef TM_H
#define TM_H

#include <stddef.h>
#include <stdint.h>

#ifndef TM_PARAMS 
  #define STATES 6
  #define SYMBOLS 2
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
  Instruction_t rules[STATES][SYMBOLS];
  size_t step_index;
  size_t head_index;
  char state;
} NaiveTM_t;

// Reduced Configuration Tree (RCT)
typedef struct configurationNode_t {
  uint64_t *tape;
  configurationNode_t **children;
  size_t head_index;
  char state;
} ConfigurationNode_t;

// Extended Backwards Reasoning Decider
typedef struct {
  ConfigurationNode_t *halting_signature;
  ConfigurationNode_t *current_node;
  ConfigurationNode_t **unexplored_leaves;

  Instruction_t rules[STATES][SYMBOLS];
} EBRDecider;

#endif