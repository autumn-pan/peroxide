#include <stdio.h>

#include "tm.h"


uint8_t read(uint64_t *tape, size_t index) {
  if(!tape) {
    return 2;
  }


  size_t first_index = index/64;
  size_t second_index = index%64;

  if(first_index > TAPE_SIZE) {
    fprintf(stderr, "Error: Tape out of memory!");
    return 3;
  }

  return (tape[first_index] >> second_index) & 1U;
}

void write(uint64_t *tape, size_t index, bool value) {
  if(!tape) {
    return;
  }

  size_t first_index = index/64;
  size_t second_index = index%64;

  if(first_index > TAPE_SIZE) {
    fprintf(stderr, "Error: Tape out of memory!");
    return;
  }

  if(!value)
    tape[first_index] &= ~(1U << second_index);
  else
    tape[first_index] |= ~(1U << second_index);
}

void shift(NaiveTM_t *tm, DIRECTION dir) {
  if(!tm) {
    return;
  }

  tm->head_index++;
  size_t first_index = tm->head_index/64;

  if(first_index > TAPE_SIZE - 1) {
    fprintf(stderr, "Error: Tape out of memory!");
  }

  return;
}