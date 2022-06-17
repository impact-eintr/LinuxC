#include <assert.h>
#include <endian.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/algorithm.h"

// shard with BST
rbtree_node_interface default_i_rbt_node;
rb_tree_t *bs_construct_keystr(char *str);
int internal_tree_compare(uint64_t a, uint64_t b, rbtree_node_interface *i_node,
                          int is_rbt);
void bst_internal_setchild(uint64_t parent, uint64_t child, child_t direction,
                           rbtree_node_interface *i_node);
void bst_internal_replace(uint64_t victim, uint64_t node,
                          rbtree_internal_t *tree,
                          rbtree_node_interface *i_node);
void rbt_internal_verify(rbtree_internal_t *tree, rbtree_node_interface *i_node,
                         int is_rbt);

// 4 kinds of rotations
static uint64_t rbt_internal_rotate(int64_t n, uint64_t p, uint64_t g,
                                    rbtree_internal_t *tree,
                                    rbtree_node_interface *i_node) {
  assert(tree != NULL);
  assert(tree->update_root != NULL);
  rbt_validate_interface(i_node, IRBT_CHECKNULL | IRBT_COMPARE | IRBT_PARENT | IRBT_LEFT | IRBT_RIGHT);
  // must not be null
  assert(i_node->is_null_node(n) == 0);
  assert(i_node->is_null_node(p) == 0);
  assert(i_node->is_null_node(g) == 0);
  // must be parent and grandparent
  assert(i_node->compare_nodes(p, i_node->get_parent(n)) == 0);
  assert(i_node->compare_nodes(g, i_node->get_parent(p)) == 0);

  uint64_t n_left = i_node->get_leftchild(n);
  uint64_t n_right = i_node->get_rightchild(n);
  uint64_t p_left = i_node->get_leftchild(p);
  uint64_t p_right = i_node->get_rightchild(p);
  uint64_t g_left = i_node->get_leftchild(g);
  uint64_t g_right = i_node->get_rightchild(g);

  if (i_node->compare_nodes(g_left, p) == 0) {
    if (i_node->compare_nodes(p_left, n) == 0) {
      //            g                         p
      //          /   \                     /   \
      //         p     D                   n     g
      //       /   \       =中序升根=>    / \   / \
      //      n     C                    A   B C   D
      //    /   \
      //   A     B
      bst_internal_replace(g, p, tree, i_node); // NOTE this function only change root->g to root->p !!!
      bst_internal_setchild(p, g, RIGHT_CHILD, i_node);
      bst_internal_setchild(g, p_left, LEFT_CHILD, i_node);

      return p;
    } else {
      //            g                         n
      //          /   \                     /   \
      //         p     D                   p     g
      //       /   \       =中序升根=>    / \   / \
      //      A     n                    A   B C   D
      //          /   \
      //         B     C
      bst_internal_replace(g, n, tree, i_node);
      bst_internal_setchild(n, p, LEFT_CHILD, i_node);
      bst_internal_setchild(n, g, RIGHT_CHILD, i_node);
      bst_internal_setchild(p, n_left, RIGHT_CHILD, i_node);
      bst_internal_setchild(g, n_right, LEFT_CHILD, i_node);

      return n;
    }
  } else {
    if (i_node->compare_nodes(p_left, n) == 0) {
      //       g                              n
      //     /   \                          /   \
      //    A     p                        g     p
      //        /   \      =中序升根=>    / \   / \
      //       n     D                   A   B C   D
      //     /   \
      //    B     C
      bst_internal_replace(g, p, tree, i_node);
      bst_internal_setchild(n, g, LEFT_CHILD, i_node);
      bst_internal_setchild(n, p, RIGHT_CHILD, i_node);
      bst_internal_setchild(g, n_left, RIGHT_CHILD, i_node);
      bst_internal_setchild(p, n_right, LEFT_CHILD, i_node);

      return n;
    } else {
      //       g                              p
      //     /   \                          /   \
      //    A     p                        g     n
      //        /   \      =中序升根=>    / \   / \
      //       B     n                   A   B C   D
      //           /   \
      //          C     D
      bst_internal_replace(g, p, tree, i_node);
      bst_internal_setchild(p, g, LEFT_CHILD, i_node);
      bst_internal_setchild(g, p_left, RIGHT_CHILD, i_node);

      return p;
    }
  }
}

void rbt_internal_insert(rbtree_internal_t *tree, rbtree_node_interface *i_node,
                         uint64_t node_id) {
  if (tree == NULL) {
    return;
  }
  assert(tree->update_root != NULL);
  rbt_validate_interface(i_node, IRBT_CHECKNULL | IRBT_COMPARE | IRBT_PARENT |
                                     IRBT_LEFT | IRBT_RIGHT | IRBT_COLOR | IRBT_KEY);
  assert(i_node->is_null_node(node_id));

  uint64_t x = node_id;

  // set the inserted nod as red node
  i_node->set_color(x, COLOR_RED);
  i_node->set_parent(x, NULL_ID);
  i_node->set_leftchild(x, NULL_ID);
  i_node->set_rightchild(x, NULL_ID);

  // if tree is empry, x would be insert as BLACK node
  bst_internal_insert(tree, i_node, x);

  uint64_t n = x;
  // float up RBT
  while (1) {
    rb_color_t n_color = i_node->get_color(n);
    uint64_t p = i_node->get_parent(p);
    if (i_node->is_null_node(p) == 0) {
      rb_color_t p_color = i_node->get_color(p);

      assert(n_color == COLOR_RED);
      if (i_node->is_null_node(p) == 0) {
        // end of floating up
        return;
      } else {
        // p is red && n is red
        uint64_t g = i_node->get_parent(p);
        assert(i_node->is_null_node(g) == 0);
        assert(i_node->get_color(g) == COLOR_BLACK);

        // rotate
        uint64_t r_root = rbt_internal_rotate(n, p, g, tree, i_node);

        // recolor
        i_node->set_color(g, COLOR_BLACK);
        i_node->set_color(p, COLOR_BLACK);
        i_node->set_color(n, COLOR_BLACK);
        i_node->set_color(r_root,COLOR_RED);

        n = r_root;
        continue;
      }
    } else {
      // n should be the root of the tree
      i_node->set_color(n, COLOR_BLACK);
      return;
    }
  }
}

static void rbt_get_psnf(rbtree_internal_t *tree, rbtree_node_interface *i_node,
                         uint64_t db, uint64_t *p, uint64_t *s, uint64_t *n,
                         uint64_t *f) {}

void rbt_internal_delete(rbtree_internal_t *tree, rbtree_node_interface *i_node,
                         uint64_t node_id) {}

// =======================================
//         Defaut Implementation
// =======================================

// build binary tree
static int color_tree_dfs(rb_node_t *n, char *color, int index) {
  if (n == NULL) {
    assert(color[index] == '#');
    return index;
  }

  if (color[index] == 'R') {
    n->color = COLOR_RED;
  } else if (color[index] == 'B') {
    n->color = COLOR_BLACK;
  }
  index = color_tree_dfs(n->left, color, index + 1);
  index = color_tree_dfs(n->left, color, index + 1);

  return index;
}

rb_tree_t *rbt_construct_keystr(char *tree, char *color) {}

void rbt_free(rb_tree_t *tree) {}

void rbt_add(rb_tree_t *tree, uint64_t key) {}

void rbt_insert(rb_tree_t *tree, rb_node_t *node) {}

void rbt_remove(rb_tree_t *tree, uint64_t key) {}

void rbt_delete(rb_tree_t *tree, rb_node_t *node) {}

rb_node_t *rbt_find(rb_tree_t *tree, uint64_t key) {}

rb_node_t *rbt_find_succ(rb_tree_t *tree, uint64_t key) {}

int rbt_compare(rb_tree_t *a, rb_tree_t *b) {}

void rbt_rotate(rb_node_t *n, rb_node_t *p, rb_node_t *g, rb_tree_t *tree) {}

void rbt_verify(rb_tree_t *tree) {}
