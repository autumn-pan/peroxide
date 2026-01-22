#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "parser.h"
#include "tm.h"

// Code from APBeaver repository, by Autumn Pan 2025
static bool validate_and_advance(char **str, char *out, size_t min_length) {
  if (strlen(*str) < min_length) 
    return false;

  *out = **str;
  (*str)++;
  return true;
}

// Parse a halt instruction "---"
bool parse_halt(char *str) {
  if (str[0] != '-' || str[1] != '-' || str[2] != '-')
    return false;

  str += 3;
  return true;
}

Instruction_t parse_instruction(char *src) {
  Instruction_t instruction;
  instruction.halting = false;
  instruction.error = false;
  
  if (parse_halt(src)) {
    instruction.halting = true;
    return instruction;
  }

    // Parse which direction the head moves
  char move_char;
  if (!validate_and_advance(&src, &move_char, 1)) {
    instruction.error = true;
    return instruction;
  }

    // Parse what value is written to the tape
  char write_char;
  if (!validate_and_advance(&src, &write_char, 1) || !isdigit(write_char)) {
    instruction.error = true;
    return instruction;
  }
  instruction.write = write_char - '0';

  if (move_char == 'R') {
    instruction.dir = RIGHT;
  } else if (move_char == 'L') {
    instruction.dir = LEFT;
  } else {
    instruction.error = true;
    return instruction;
  }

  // Parse the next state that the machine transitions to
  char state_char;
  if (!validate_and_advance(&src, &state_char, 1)) {
    instruction.error = true;
    return instruction;
  }
  instruction.state = state_char - 'A';

  instruction.error = false;
  return instruction;
}


NaiveTM_t* init_tm(char *src)
{
  NaiveTM_t *tm = malloc(sizeof(NaiveTM_t));
  if(!tm)
  {
    fprintf(stderr, "Error: Failed to allocate memory!");
    return NULL;
  }

  // Parse the instructions from the source code
  char *mirror = src;
  for (int i = 0; i < STATES; i++) {
    for (int j = 0; j < SYMBOLS; j++) {
      Instruction_t tmp = parse_instruction(mirror);
      if (tmp.error)
        return NULL;

      mirror += 3;
      tm->rules[i][j] = tmp;
    }

    // Account for the underscore separating states
    if (i < STATES - 1)
      mirror++;
  }
  tm->tape = calloc(TAPE_SIZE, sizeof(uint64_t));
  tm->head_index = 32*TAPE_SIZE;
  tm->state = 0;
  tm->step_index = 0;

  return tm;
}