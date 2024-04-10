#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nil = (node_t *) calloc(1,sizeof(node_t));
  nil->color = RBTREE_BLACK;
  p->nil = nil;
  p->root = p->nil;

  return p;
}

void delete_rbtree_node(node_t *curr, node_t *nil){
  if (curr == nil){
    return;
  }
  delete_rbtree_node(curr->left, nil);
  delete_rbtree_node(curr->right, nil);
  free(curr);
}
void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  delete_rbtree_node(t->root, t->nil);
  free(t->nil);
  free(t);
}
node_t *init_node(node_t *nil, const key_t key){
  node_t *added = malloc(sizeof (node_t ));
  added->parent = nil;
  added->left = nil;
  added->right = nil;
  added->key = key;
  added->color = RBTREE_RED;

  return added;
}
void insert_node(rbtree *t, node_t *curr, node_t *added){

  if (curr == t->nil) {
    t->root = added;
    return;
  }

  while (curr != t->nil) {
    if (curr->key <= added->key) {
      if (curr->right == t->nil) {
        curr->right = added;
        added->parent = curr;
        return;
      }else{
        curr = curr->right;
        continue;
      }
    }
    else{
      if (curr->left == t->nil) {
        curr->left = added;
        added->parent = curr;
        return;
      } else{
        curr = curr->left;
        continue;
      }
    }
  }
}

void rotate_right(rbtree *t, node_t *key_node){
  node_t *child = key_node->left;

  key_node->left = child->right;
  if (child->right != t->nil) {
    child->right->parent = key_node;
  }
  child->parent = key_node->parent;

  if (key_node->parent == t->nil) {
    t->root = child;
  } else if (key_node == key_node->parent->right) {
    key_node->parent->right = child;
  } else{
    key_node->parent->left = child;
  }
  child->right = key_node;
  key_node->parent = child;
}

void rotate_left(rbtree *t, node_t *key_node) {
  node_t *child = key_node->right;

  key_node->right = child->left;
  if (child->left != t->nil) {
    child->left->parent = key_node;
  }
  child->parent = key_node->parent;

  if (key_node->parent == t->nil) {
    t->root = child;
  } else if (key_node == key_node->parent->left) {
    key_node->parent->left = child;
  } else{
    key_node->parent->right = child;
  }
  child->left = key_node;
  key_node->parent = child;
}

void insert_fix(rbtree *t, node_t *added){
  node_t *parent = added->parent;
  node_t *grand_parent = parent->parent;
  node_t *uncle;

  int is_left;
  int is_parent_left;

  if (added == t->root) { /*삽입된 노드가 루트*/
    added->color = RBTREE_BLACK;
    return;
  }

  if (parent->color == RBTREE_BLACK) { /*부모가 블랙이면 위반되는 속성 없음*/
    return;
  }
  is_left = (added == parent->left) ? 1 : 0;
  is_parent_left = (parent == grand_parent->left) ? 1 : 0;
  uncle = is_parent_left ? grand_parent->right : grand_parent->left;

  if (uncle->color == RBTREE_RED) {
    parent->color = uncle->color = RBTREE_BLACK;
    grand_parent->color = RBTREE_RED;

    insert_fix(t, grand_parent);
    return;
  }

  if(is_left){
    if(is_parent_left){
      rotate_right(t, grand_parent);
      parent->color = RBTREE_BLACK;
      grand_parent->color = RBTREE_RED;
      return;
    } else{
      rotate_right(t, parent);
      rotate_left(t, grand_parent);
      added->color = RBTREE_BLACK;
      grand_parent->color = RBTREE_RED;
    }
  }else{
    if (is_parent_left) {
      rotate_left(t, parent);
      rotate_right(t, grand_parent);
      added->color = RBTREE_BLACK;
      grand_parent->color = RBTREE_RED;
    } else{
      rotate_left(t, grand_parent);
      parent->color = RBTREE_BLACK;
      grand_parent->color = RBTREE_RED;
    }
  }
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  if (t == NULL) {
    return NULL;
  }
  node_t *nil = t->nil;
  node_t *root = t->root;
  node_t *added = init_node(nil, key);

  if (root == nil) {
    added->color = RBTREE_BLACK;
    t->root = added;
    return added;
  }

  insert_node(t, root, added);
  insert_fix(t, added);

  return added;

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
  if (curr == nil) {
    return NULL;
  }
  return curr;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *curr_node = t->root;
  while(curr_node->left != t->nil){
    curr_node = curr_node->left;
  }
  return curr_node;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *curr_node = t->root;
  while(curr_node->right != NULL){
    curr_node = curr_node->right;
  }
  return curr_node;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
