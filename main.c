#include "sim/forwardsim.h"
#include "tm.h"

int main() {
  NaiveTM_t *tm = init_tm("1RB0RF_0LC---_1LD1LC_0LE0LD_1RE1RF_0RC1RA");
  if(!tm) {
    return 1;
  }
  
  step(tm);
  return 0;
}