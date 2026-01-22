#include <stdio.h>
#include <stdlib.h>

#include "decider.h"
#include "error.h"

RET_STATUS free_signature_subtree(HaltingSignature *sig) {
  if(!sig) {
    return RET_ERR_PARAMS;
  }

  free_signature_children(sig);
  free(sig->tape);
  free(sig);
  return RET_SUCC;
}

RET_STATUS free_signature_children(HaltingSignature *sig) {
  if(!sig) {
    return RET_ERR_PARAMS;
  }

  for(int i = 0; i < sig->num_children; i++) {
    free_signature_subtree(sig->children[i]);
  }

  return RET_SUCC;
}

