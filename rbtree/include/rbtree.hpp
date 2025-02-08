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
  RBTree() = default;

  using NodeType = RBNode<Key>;

  int size() const {
    return size_;
  }

  void insert(const Key& key);

  void remove(const Key& key);

  void left_rotate(NodeType* node);

  void right_rotate(NodeType* node);

  void __insert(NodeType* node);

  void fix_insert(NodeType* node);

  NodeType* find_parent(const Key& key);

  // help func
  void print();
  bool is_valid_rb_tree();

  NodeType* root_ = nullptr;
  int size_ = 0;
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
    std::cout << "First Node : " << root_->key << std::endl;
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
  
}