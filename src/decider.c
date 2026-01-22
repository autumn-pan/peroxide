#include <stdio.h>
#include <stdlib.h>

#include "decider.h"
#include "error.h"

RET_STATUS free_signature_children(HaltingSignature *root);

RET_STATUS free_signature_subtree(HaltingSignature *root) {
  if(!root) {
    return RET_ERR_PARAMS;
  }

  free_signature_children(root);
  free(root->tape);
  free(root);
  return RET_SUCC;
}

RET_STATUS free_signature_children(HaltingSignature *root) {
  if(!root) {
    return RET_ERR_PARAMS;
  }

  for(int i = 0; i < root->num_children; i++) {
    free_signature_subtree(root->children[i]);
  }

  return RET_SUCC;
}

