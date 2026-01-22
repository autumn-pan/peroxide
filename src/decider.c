#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "decider.h"
#include "error.h"

///////////////////////////////////////////////////////////
// Halting Signature Utils
///////////////////////////////////////////////////////////

// Run free_signature_subtree over all children of a halting signature
RET_STATUS free_signature_children(HaltingSignature *root);

// Takes root and frees all descendents of it as well as itself
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

HaltingSignature *init_halting_signature(char state, size_t head_index, uint64_t *tape) {
  if(!tape) {
    return NULL;
  }

  HaltingSignature *sig = malloc(sizeof(HaltingSignature));
  if(!sig) {
    return NULL;
  }

  memcpy(sig->tape, tape, sizeof(tape));
  sig->state = state;
  sig->head_index = head_index;
  sig->children = NULL;
  sig->num_children = 0;
  sig->terminated = false;

  return sig;
}

EBRDecider *init_decider(Instruction_t rules[STATES][SYMBOLS]) {
  EBRDecider *decider = malloc(sizeof(EBRDecider));
  if(!decider) {
    return NULL;
  }

  decider->current_node = NULL;
  decider->root = NULL;
  decider->num_unsolved = 0;
  decider->unexplored_leaves = NULL;
  memcpy(decider->rules, rules, sizeof(rules));

  return decider;
}