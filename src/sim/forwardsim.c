#include "sim/forwardsim.h"
#include "tm.h"

#include <string.h>
#include <stdio.h>
#include <stdint.h>

void step(NaiveTM_t *tm) {
  for(int i = 0; i < 100; i++) {
    uint8_t symbol = read(tm->tape, tm->head_index);
    write(tm->tape, tm->head_index, tm->rules[tm->state][symbol].write);
    shift(tm, tm->rules[tm->state][symbol].dir);
    tm->state = tm->rules[tm->state][symbol].state;
  }
}
