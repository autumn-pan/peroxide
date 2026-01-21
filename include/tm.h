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
  size_t
} NaiveTM_t;

#endif