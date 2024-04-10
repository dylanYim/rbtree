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
  while (curr != t->nil){
    if (curr->key == key){
      return curr;
    }
    else if (curr->key < key){
      curr = curr->right;
    }
    else {
      curr = curr->left;
    }
  }
  return NULL;
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
  while(curr_node->right != t->nil){
    curr_node = curr_node->right;
  }
  return curr_node;
}

void erase_fix(rbtree *t, node_t *substitute){

  node_t *sibling;

  while(substitute != t->root && substitute->color == RBTREE_BLACK) {
    if (substitute == substitute->parent->left) {
      sibling = substitute->parent->right;
      if (sibling->color == RBTREE_RED) {
        sibling->color = RBTREE_BLACK;
        substitute->parent->color = RBTREE_RED;
        rotate_left(t, substitute->parent);
        sibling = substitute->parent->right;
      }
      if (sibling->left->color == RBTREE_BLACK && sibling->right->color == RBTREE_BLACK){
        sibling->color = RBTREE_RED;
        substitute = substitute->parent;
      } else {
        if(sibling->right->color == RBTREE_BLACK) {
          sibling->left->color = RBTREE_BLACK;
          sibling->color = RBTREE_RED;
          rotate_right(t, sibling);
          sibling = substitute->parent->right;
        }
        sibling->color = substitute->parent->color;
        substitute->parent->color = RBTREE_BLACK;
        sibling->right->color = RBTREE_BLACK;
        rotate_left(t, substitute->parent);
        substitute = t->root;
      }
    } else {
      sibling = substitute->parent->left;
      if (sibling->color == RBTREE_RED) {
        sibling->color = RBTREE_BLACK;
        substitute->parent->color = RBTREE_RED;
        rotate_right(t, substitute->parent);
        sibling = substitute->parent->left;
      }
      if (sibling->right->color == RBTREE_BLACK && sibling->left->color == RBTREE_BLACK) {
        sibling->color = RBTREE_RED;
        substitute = substitute->parent;
      } else {
        if (sibling->left->color == RBTREE_BLACK) {
          sibling->right->color = RBTREE_BLACK;
          sibling->color = RBTREE_RED;
          rotate_left(t, sibling);
          sibling = substitute->parent->left;
        }
        sibling->color = substitute->parent->color;
        substitute->parent->color = RBTREE_BLACK;
        sibling->left->color = RBTREE_BLACK;
        rotate_right(t, substitute->parent);
        substitute = t->root;
      }
    }
  }
  substitute->color = RBTREE_BLACK;
}

node_t *find_successor(rbtree *t, node_t *target){
  node_t *curr = target;
  if (curr == t->nil) {
    return curr;
  }
  while (curr->left != t->nil) {
    curr = curr->left;
  }
  return curr;
}

void transplant(rbtree *t, node_t *u, node_t *v){
  if (u->parent == t->nil) {
    t->root = v;
  } else if (u == u->parent->left) {
    u->parent->left = v;
  } else {
    u->parent->right = v;
  }
  v->parent = u->parent;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase

  node_t *target = p;
  node_t *child;
  color_t target_origin_color = target->color;
  if (p->left == t->nil) {
    child = p->right;
    transplant(t, p, p->right);
  } else if (p->right == t->nil) {
    child = p->left;
    transplant(t, p, p->left);
  }else {
    target = find_successor(t, p->right);
    target_origin_color = target->color;
    child = target->right;
    if (target->parent == p) {
      child->parent = target;
    }
    else{
      transplant(t, target, target->right);
      target->right = p->right;
      target->right->parent = target;
    }
    transplant(t, p, target);
    target->left = p->left;
    target->left->parent = target;
    target->color = p->color;
  }
  if (target_origin_color == RBTREE_BLACK) {
    erase_fix(t, child);
  }
  free(p);
  return 0;
}
int inorder_traversal(node_t *root, key_t *res, const rbtree *t, int i){
  if (root == t->nil){
    return i;
  }
  i = inorder_traversal(root->left, res, t, i);
  res[i] = root->key;
  i++;
  i = inorder_traversal(root->right, res, t, i);
  return i;
}
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  if (t->root == t->nil) {
    return -1;
  }
  inorder_traversal(t->root, arr, t, 0);
  return 0;
}
