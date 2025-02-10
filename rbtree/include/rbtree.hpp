#pragma once

#include <queue>
#include <string>
#include <iostream>
#include <assert.h>
#include <functional>

enum RBColor {
  RED,
  BLACK
};

template<typename Key>
struct RBNode {
  Key key;
  RBColor color;
  RBNode<Key>* parent;
  RBNode<Key>* left;
  RBNode<Key>* right;
};

template<typename Key>
class RBTree {
public:
  RBTree()  {
    __dummy_nil_node = new NodeType();
    __dummy_nil_node->color = BLACK;
  }

  using NodeType = RBNode<Key>;

  int size() const {
    return size_;
  }

  void insert(const Key& key);

  void remove(const Key& key);

  void left_rotate(NodeType* node);

  void right_rotate(NodeType* node);

  void __insert(NodeType* node);

  void __remove(NodeType* node);

  void __remove_leaf_node(NodeType* node);

  void __remove_node_with_one_child(NodeType* node);

  void fix_remove(NodeType* node, RBColor node_remove_color);

  void fix_insert(NodeType* node);

  NodeType* find_parent(const Key& key);

  // help func
  void print();
  bool is_valid_rb_tree();

  NodeType* root_ = nullptr;
  int size_ = 0;

  NodeType* __dummy_nil_node;
};

template<typename Key>
void RBTree<Key>::left_rotate(NodeType* node)
{
  if (node == nullptr || node->right == nullptr) {
    throw std::runtime_error("[left_rotate] got invalid node");
  }

  // 1. node节点左移
  NodeType* right_child = node->right;
  node->right = right_child->left;
  if (node->right != nullptr) {
    node->right->parent = node;
  }

  // 2. 右子节点做根节点
  right_child->left = node;
  right_child->parent = node->parent;
  node->parent = right_child;

  // 3. 判断是否是整个树的根节点
  if (right_child->parent == nullptr) {
    root_ = right_child;
  } else {
    if (right_child->parent->left == node) {
      right_child->parent->left = right_child;
    } else {
      right_child->parent->right = right_child;
    }
  }

}

template<typename Key>
void RBTree<Key>::right_rotate(NodeType* node)
{
  if (node == nullptr || node->left == nullptr) {
    throw std::runtime_error("[right_rotate] got invalid node");
  }

  // 1. node节点右移
  NodeType* left_child = node->left;
  node->left = left_child->right;
  if (node->left != nullptr) {
    node->left->parent = node;
  }

  // 2. 左子节点做根
  left_child->right = node;
  left_child->parent = node->parent;
  node->parent = left_child;

  // 3. 判断
  if (left_child->parent == nullptr) {
    root_ = left_child;
  } else {
    if (left_child->parent->left == node) {
      left_child->parent->left = left_child;
    } else {
      left_child->parent->right = left_child;
    }
  }
}


template<typename Key>
void RBTree<Key>::print()
{
  if (root_ == nullptr) {
    return;
  }

  std::queue<NodeType*> q;
  q.push(root_);

  while (!q.empty()) {
    int size = q.size();
    std::string level_print;
    for (int i = 0 ; i < size ; ++ i) {
      auto cur = q.front();
      q.pop();
      level_print += std::to_string(cur->key) + "." + (cur->color == RED ? "r" : "b") + ".p:" 
                     + (cur->parent == nullptr ? "null" : std::to_string(cur->parent->key)) + "\t";

      if (cur->left != nullptr) {
        q.push(cur->left);
      }
      if (cur->right != nullptr) {
        q.push(cur->right);
      }
    }

    std::cout << level_print << std::endl;
  }
}

template<typename Key>
bool RBTree<Key>::is_valid_rb_tree()
{
  // 1. 只有紅色黑色節點  （不检查）
  // 2. 根節點爲黑色    
  // 3. 没有连续红色节点
  // 4. 每个叶子节点为为黑色  （不检查）
  // 5. 对于任意节点，从该节点出发，所有到达叶子节点的路径，其经过的黑色节点数量是相同的（黑节点平衡）。
  if (root_ == nullptr) return true;

  // 2. 根节点为黑色
  if (root_->color == RED) return false;

  // 3. 没有连续红色节点
  // 5. 对于任意节点，从该节点出发，所有到达叶子节点的路径，其经过的黑色节点数量是相同的（黑节点平衡）。
  bool invalid_flag = false;
  std::function<int(NodeType*)> func = [&](NodeType* node) {
    if (node == nullptr) return 0;

    if (node->parent != nullptr && node->parent->color == RED && node->color == RED) {
      invalid_flag = true;
    }

    int left_length = func(node->left);
    int right_length = func(node->right);
    if (left_length != right_length) {
      invalid_flag = true;
    }

    return node->color == BLACK ? 1 + left_length : left_length;
  };

  func(root_);

  if (invalid_flag == true) return false;

  return true;
}


template<typename Key>
void RBTree<Key>::insert(const Key& key)
{
  auto new_node = new NodeType();
  new_node->key = key;

  __insert(new_node);
  ++ size_;
}

template<typename Key>
void RBTree<Key>::__insert(NodeType* node)
{
  auto parent = find_parent(node->key);
  
  // 目前是空的树
  if (parent == nullptr) {
    root_ = node;
    root_->color = BLACK;
    return;
  }

  if (parent->key == node->key) {
    throw std::runtime_error("[__insert] got equal key !");
  }

  node->parent = parent;
  if (parent->key > node->key) {
    parent->left = node;
  } else {
    parent->right = node;
  }

  node->color = RED;
  fix_insert(node);
}


template<typename Key>
void RBTree<Key>::fix_insert(NodeType* node)
{
  auto parent = node->parent;

  while (node != root_ && parent != nullptr && parent->color == RED) {
    auto gparent = parent->parent;
    // 这里gparent不可能是nullptr，因为在插入前，树是合法的红黑树，根节点一定是黑色节点
    // 且gparent一定是黑色节点
    assert(gparent != nullptr);
    assert(gparent->color == BLACK);

    // 在gparent的左侧
    if (gparent->left == parent) {
      auto uncle = gparent->right;
      // 有叔叔节点且为红色，即临时4节点，转换为2节点和3节点，向上传递一个元素
      if (uncle != nullptr && uncle->color == RED) {
        uncle->color = BLACK;
        parent->color = BLACK;
        gparent->color = RED;
      } else {
        // 假如node、parent、gparent不在同一侧，旋转
        if (parent->right == node) {
          left_rotate(parent);
          std::swap(node, parent);
        }
        // 旋转一次
        parent->color = BLACK;
        gparent->color = RED;
        right_rotate(gparent);
        break;
      }
    } 
    else {
      auto uncle = gparent->left;

      if (uncle != nullptr && uncle->color == RED) {
        uncle->color = BLACK;
        parent->color = BLACK;
        gparent->color = RED;
      } else {
        if (parent->left == node) {
          right_rotate(parent);
          std::swap(node, parent);
        }
        parent->color = BLACK;
        gparent->color = RED;
        left_rotate(gparent);
        break;
      }
    }

    node = gparent;
    parent = node->parent;
  }

  // [?] 可能根節點正好是紅色，強行改成黑色
  root_->color = BLACK;
}


template<typename Key>
typename RBTree<Key>::NodeType* RBTree<Key>::find_parent(const Key& key)
{
  if (root_ == nullptr) return nullptr;

  NodeType* x = root_, *y = root_;
  while (x != nullptr) {
    y = x;
    if (x->key > key) {
      x = x->left;
    } else if (x->key < key) {
      x = x->right;
    } else {
      return x;
    }
  }

  return y;
}

template<typename Key>
void RBTree<Key>::remove(const Key& key)
{
  auto node = find_parent(key);
  if (node == nullptr || node->key != key) {
    return;
  }

  __remove(node);
  -- size_;
}


template<typename Key>
void RBTree<Key>::__remove(NodeType* node)
{
  // 1. 删除叶子节点
  if (node->left == nullptr && node->right == nullptr) {
    __remove_leaf_node(node);
  }
  // 2. 删除只有一个子节点的
  else if ((node->left == nullptr) ^ (node->right == nullptr)) {
    __remove_node_with_one_child(node);
  }
  // 3. 删除具有两个叶子节点的
  else if (node->left != nullptr && node->right != nullptr) {
    auto right_min_node = node->right;
    while (right_min_node != nullptr && right_min_node->left != nullptr) {
      right_min_node = right_min_node->left;
    }
    node->key = right_min_node->key;
    __remove(right_min_node);
  } else {
    assert(0);
  }


}

template<typename Key>
void RBTree<Key>::__remove_leaf_node(NodeType* node)
{
  if (node->parent == nullptr) {
    root_ = nullptr;
    return;
  }

  __dummy_nil_node->parent = node->parent;
  if (node->parent->left == node) {
    node->parent->left = __dummy_nil_node;
    fix_remove(__dummy_nil_node, node->color);
    node->parent->left = nullptr;
  }
  if (node->parent->right == node) {
    node->parent->right = __dummy_nil_node;
    fix_remove(__dummy_nil_node, node->color);
    node->parent->right = nullptr;
  }

}

template<typename Key>
void RBTree<Key>::__remove_node_with_one_child(NodeType* node)
{
  auto child = node->left == nullptr ? node->right : node->left;
  // 父节点修改
  if (node->parent != nullptr) {
    if (node->parent->left == node) {
      node->parent->left = child;
    }
    if (node->parent->right == node) {
      node->parent->right = child;
    }
  }
  // 子节点修改
  if (child != nullptr)
    child->parent = node->parent;

  if (node == root_) {
    root_ = child;
  }

  fix_remove(child, node->color);
}


template<typename Key>
void RBTree<Key>::fix_remove(NodeType* node, RBColor node_remove_color)
{
  // 被删除节点为红色
  if (node_remove_color == RED) return;

  assert(node != nullptr);
  // 当前节点为红色
  if (node->color == RED) {
    node->color = BLACK; 
    return;
  }

  // 当前节点是根节点
  if (node->parent == nullptr) {
    node->color = BLACK;
    return;
  }

  auto brother = node->parent->left == node ? node->parent->right : node->parent->left;
  assert(brother != nullptr);

  // 1. 兄弟节点是红色:
  if (brother->color == RED) {
    node->parent->color = RED;
    brother->color = BLACK;
    if (node->parent->left == node) {
      left_rotate(node->parent);
    } else {
      right_rotate(node->parent);
    }
    fix_remove(node, node_remove_color);
    return;
  }
  // 2 兄弟节点是黑色，兄弟节点左右节点都是黑色
  else if ((brother->left == nullptr || brother->left->color == BLACK)
      && (brother->right == nullptr || brother->right->color == BLACK)) {
    // 直接把兄弟节点改成红色，这样就平衡了node和brother的黑平衡，但上层仍可能存在黑节点失衡
    // 所以递归，相当于问题变成了：删除了node-parent原本的节点，导致该子树的黑节点深度-1
    brother->color = RED;
    fix_remove(node->parent, BLACK);
    return;
  } 

  // ***IMPORTANT*** 
  // 下面情况3、4存在左右镜像问题，应分开讨论
  // 最终目的就是将兄弟节点的红色子节点，放到外侧! 然后按照父节点旋转，从而平衡两侧的黑色节点深度

  if (node->parent->left == node) {
    // 3 兄弟节点是黑色，兄弟节点左子节点是红色，右子节点是黑色
    // 这种情况下，要将它转换到情况4，
    if ((brother->left != nullptr && brother->left->color == RED)
        && (brother->right == nullptr || brother->right->color == BLACK)){
      brother->color = RED;
      brother->left->color = BLACK;
      right_rotate(brother);
      fix_remove(node, node_remove_color);
    } 
    // 4 兄弟节点是黑色， 兄弟节点右子节点是红色，左子节点任意
    // 最终形态，可以通过旋转实现黑色节点深度平衡
    else if (brother->right != nullptr && brother->right->color == RED) {
      brother->color = node->parent->color;
      node->parent->color = BLACK;
      brother->right->color = BLACK;
      left_rotate(node->parent);
    } else {
      assert(0);
    }
  } 
  else {
    // 3 兄弟节点是黑色，兄弟节点右子节点是红色，左子节点是黑色
    // 这种情况下，要将它转换到情况4
    if ((brother->left == nullptr || brother->left->color == BLACK)
        && (brother->right != nullptr && brother->right->color == RED)){
      brother->color = RED;
      brother->right->color = BLACK;
      left_rotate(brother);
      fix_remove(node, node_remove_color);
    } 
    // 4 兄弟节点是黑色， 兄弟节点左子节点是红色，右子节点任意
    // 最终形态，可以通过旋转实现黑色节点深度平衡
    else if (brother->left != nullptr && brother->left->color == RED) {
      brother->color = node->parent->color;
      node->parent->color = BLACK;
      brother->left->color = BLACK;
      right_rotate(node->parent);
    } else {
      assert(0);
    }
  }
}