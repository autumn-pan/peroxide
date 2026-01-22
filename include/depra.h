#ifndef TURING_H
#define TURING_H

#include <gmp.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

extern const uint8_t NUM_STATES;
// This system handles 6 state TMs, so any symbol after 'F' (70) represents a
// halt
extern const uint8_t HLT_SYMBOLS;
extern const uint8_t NUM_SYMBOLS;
extern const uint16_t TAPE_LENGTH;
extern const uint16_t START_POS;

enum MODE_TYPE { SIM, RANGE, CELLS, UNSET };
enum SIM_RESULT { HALTED, OVERFLOWED, TAPE_EXCEEDED };

typedef struct {
  bool error;    // Whether or not the instruction was successfully parsed
  bool halting;  // Whether or not it is the halt state
  uint8_t write; // What value to write under the head
  bool move;     // false represents left, true represents right
  uint8_t new_state;
} Instruction_t;

typedef struct {
  Instruction_t instructions[6][2];
  uint8_t tape[UINT16_MAX];
  uint32_t head;
  uint8_t state;
} TuringMachine_t;

TuringMachine_t *init_turing(char *str);
uint8_t *init_tape(size_t size);
bool write(uint8_t *tape, size_t index, bool value);
int8_t read(uint8_t *tape, size_t index);

#endif

#ifndef UNACC_H
#define UNACC_H

#include <stdint.h>

uint64_t unaccelerated_sim(TuringMachine_t *tm, enum MODE_TYPE mode);

#endif