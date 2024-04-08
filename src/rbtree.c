#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *root = (node_t *) malloc(sizeof(node_t));
  root->color = RBTREE_BLACK;
  root->key = -1;
  root->left = NULL;
  root->right = NULL;
  root->parent = NULL;
  p->root = root;
  p->nil = root;
  return p;
}
void delete_node(node_t *curr){
  node_t *parent = curr->parent;
  if (parent == NULL){
    free(curr);
    curr = NULL;
    return;
  }
  node_t *tmp;
  if (parent->left != NULL && parent->left == curr) {
    parent->left = NULL;
    free(curr);
    curr = NULL;
  } else if (parent->right != NULL && parent->right == curr) {
    parent->right = NULL;
    free(curr);
    curr = NULL;
  }

}
void delete_rbtree_node(node_t *curr){
  if (curr == NULL){
    return;
  }
  delete_rbtree_node(curr->left);
  delete_rbtree_node(curr->right);
  delete_node(curr);
}
void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  delete_rbtree_node(t->root);
  t->root = NULL;
  t->nil = NULL;
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *curr = t->root;
  node_t *nil = t->nil;

  while (curr != nil){
    if (curr->key == key){
      break;
    }
    if (curr->key > key){
      curr = curr->left;
      continue;
    }
    if (curr->key < key){
      curr = curr->right;
    }
  }
  return curr;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
