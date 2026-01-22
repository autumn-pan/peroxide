#include "depra.h"

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

const uint8_t NUM_STATES = 6;
// This system handles 6 state TMs, so any symbol after 'F' (70) represents a halt
const uint8_t HLT_SYMBOLS = 71;
const uint8_t NUM_SYMBOLS = 2;
const uint16_t TAPE_LENGTH = UINT16_MAX;
const uint16_t START_POS = (UINT16_MAX >> 1);

// Tests will be conducted on BB6 holdout
// 1RB1LD_1RC0LD_1LD1RE_0LA1LD_0RB0RF_---0RC

bool write(uint8_t *tape, size_t index, bool value) {
  if (index >= TAPE_LENGTH - 1) {
    fprintf(stderr, "Error: Index out of bounds!\n");
    return false;
  }

  size_t integer_index = floor(index / 8);
  size_t subindex = index % 8;

  if (value)
    tape[integer_index] |= (1 << (7 - subindex));
  else
    tape[integer_index] &= ~(1 << (7 - subindex));

  return true;
}

int8_t read(uint8_t *tape, size_t index) {
  if (index >= TAPE_LENGTH - 1) {
    fprintf(stderr, "Error: Index out of bounds!\n");
    return -1;
  }
  if(!tape)
  {
    fprintf(stderr, "Error: Null passed to read!\n");
    return -1;
  }

  size_t integer_index = (index / 8);
  size_t subindex = index % 8;

  return ((tape[integer_index] >> (7 - subindex)) & 1U);
}

// Parse a halt instruction "---"
bool parse_halt(char *str) {
  if (str[0] != '-' || str[1] != '-' || str[2] != '-')
    return false;

  str += 3;
  return true;
}

void dump_instruction(Instruction_t i) {
  if (i.error)
    return;
  else if (i.halting)
  {
    printf("Value: ---\n");
    return;
  }

  printf("Value: %i%i%c\n", i.write, i.move, i.new_state);
}

Instruction_t parse_instruction(char *str) {
  Instruction_t instruction;
  if (parse_halt(str)) {
    instruction.halting = true;
    return instruction;
  }

  // Parse what value is written to the tape
  instruction.halting = false;
  if (!isdigit(str[0])) {
    instruction.error = true;
    return instruction;
  }
  instruction.write = str[0] - '0';
  str++;

  // Parse which direction the head moves
  if (str[0] == 'R')
    instruction.move = true;
  else if (str[0] == 'L')
    instruction.move = false;
  else {
    instruction.error = true;
    return instruction;
  }
  str++;

  // Parse the next state that the machine transistions to
  instruction.new_state = str[0];
  str++;

  instruction.error = false;
  return instruction;
}

TuringMachine_t *init_turing(char *str) {
  TuringMachine_t *tm = malloc(sizeof(TuringMachine_t));
  if (!tm)
    return NULL;

  // Initialize the turing machine
  tm->head = START_POS;
  tm->state = 'A';

  char *mirror = str;
  for (int i = 0; i < NUM_STATES; i++) {
    for (int j = 0; j < NUM_SYMBOLS; j++) {
      Instruction_t tmp = parse_instruction(mirror);
      if (tmp.error)
        return NULL;

      mirror += 3;
      tm->instructions[i][j] = tmp;

      printf("Index: %i, %i ... ", i, j);
      dump_instruction(tmp);
    }

    // Account for the underscore separating states
    if (i < NUM_STATES - 1)
      mirror++;
  }

  // Zero initialize the tape
  memset(tm->tape, 0, TAPE_LENGTH/8);
  return tm;
}

#define MAX_STEPS 1000000000

// Find the next instruction
Instruction_t get_instruction(TuringMachine_t* tm)
{
  int8_t value = read(tm->tape, tm->head);
  if(value == -1)  
  {
    Instruction_t error;
    error.error = true;
    return error;
  }

  // Ensure that the instruction indices are within bounds
  if(tm->state < 'A' || tm->state - 'A' >= NUM_STATES)
  {
    fprintf(stderr, "Error: Turing machine state '%s' is out of bounds!\n", tm->state);
    exit(EXIT_FAILURE);
  }
  Instruction_t instruction = tm->instructions[tm->state - 'A'][value];
  return instruction;
}

// Overflow tracking is not necessary for now because it's not meant to track more than 2^64 steps
// Adding it is trivial
uint64_t unaccelerated_sim(TuringMachine_t *tm, enum MODE_TYPE mode) {
  if (!tm) {
    fprintf(stderr, "Error: Null passed to unaccelerated_sim!\n");
    return 0;
  }

  uint16_t leftmost_cell = (UINT16_MAX/2);
  uint16_t rightmost_cell = (UINT16_MAX/2);

  for(uint64_t s = 0; s < MAX_STEPS; s++) {
    // Track how many unique cells are visited by the machine
    if(mode == CELLS) 
    {
      if (leftmost_cell > tm->head)
        leftmost_cell = tm->head;
      else if (rightmost_cell < tm->head)
        rightmost_cell = tm->head;
    }

    // Check for any halting conditions
    if (tm->head >= TAPE_LENGTH || tm->head < 0) {
      printf("Turing Machine Out Of Bounds at step %i\n", s);
      printf("Head Position: %i\n", tm->head);
      return 0;
    }

    // Get instruction and check if it's halting
    Instruction_t instruction = get_instruction(tm);
    if (instruction.halting) {
      printf("Turing Machine Halted!\n");
      printf("Index: %i\n", s);
      return 0;
    }
    if(instruction.error)
      return 0;

    // Execute the code on the instruction
    if(!write(tm->tape, tm->head, (bool)instruction.write))
      return 0;
      
    tm->state = instruction.new_state;
    // Move the turing machine over by one step
    if(instruction.move == 1) 
      tm->head++;
    else
      tm->head--;
  }

  printf("SIMULATION LIMIT REACHED: INDETERMINATE");
  return rightmost_cell - leftmost_cell;
}