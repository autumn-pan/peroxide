#include "depra.h"

const char* modes[] = {
    "sim",
    "range",
    "cells"
};

int main() {
  TuringMachine_t* tm = init_turing("1RB1LD_1RC0RB_1LA1LE_1LC0LA_1LF0RD_1RE---");

  unaccelerated_sim(tm, CELLS);
  return 0;
}
