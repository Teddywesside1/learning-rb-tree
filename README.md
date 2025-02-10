# learning-rb-tree

## 概念

红黑树是自平衡的二叉搜索树，是对2-3树或2-3-4树概念的一种实现。

一颗二叉搜索树只要满足以下**5条性质**，即为红黑树：
  1. 节点为红色或黑色。
  2. 根节点一定是黑色。
  3. 没有连续的红色节点。
  4. 每个叶子节点（指树尾端NIL指针或NULL结点）为黑色。
  5. 对于任意节点，从该节点出发，所有到达叶子节点的路径，其经过的黑色节点数量是相同的（黑节点平衡）。

## 节点插入

插入的节点初始颜色为红色，插入操作分两步：
  1. 根据二叉搜索树的性质，找到能够插入节点的位置。
  2. 插入初始颜色为红色的新节点。
  3. 调整树的节点颜色，保证不存在连续的红色节点。
  4. 对树进行旋转，保证树的黑节点平衡。

整体叫做：红黑树的插入（1、2步骤）和插入修复（3、4步骤）。

插入分为几种情况：
  1. 父节点为黑色，或无父节点（空的树），直接插入节点，无需修复红黑树。（空树的插入需将节点染成黑色）
  2. 父节点为红色，重复以下过程：
     1. 如果叔叔节点也为红色，则将父节点、叔叔节点变成黑色，将祖父节点变成红色，重复循环。
     2. 如果叔叔节点为黑色，则将父节点变成黑色，将祖父节点变成红色。首先将当前节点与父节点旋转至一侧，再以祖父节点为中心进行旋转，即可退出循环。
  
## 节点删除

目标删除的节点分为三种：
1. 叶子节点
2. 有且仅有一个子节点的节点
3. 具有两个子节点的节点

### 删除叶子节点——无子节点

直接删除该节点，假设利用`NIL`节点代替该节点，并以此进行删除修复。

`NIL`节点是红黑树的空节点，类似于哨兵，**性质4**就是指`NIL`节点，上文中的`叶子节点`和`NIL`节点不是一个东西。

### 删除节点——有且仅有一个子节点

删除该节点，并用它的唯一子节点代替，以此进行删除修复。

### 删除节点——具有两个子节点

和普通搜索二叉树删除节点思路一致，使用该节点的前驱或后继节点代替该节点，以后继节点为例，将后继节点的值赋给当前节点，并删除后继节点。

于是，这种情况就转换成了`删除有且仅有一个子节点`的情况，因为根据二叉搜索树的性质，后继节点一定只有一个子节点（不包括`NIL`节点）。

### 删除修复

在完成`删除叶子节点——无子节点`和`删除节点——有且仅有一个子节点`后，需要进行删除修复。

`删除修复`的最大目的是，修复被破坏的**性质5**，参考[详解红黑树](https://github.com/julycoding/The-Art-Of-Programming-By-July-2nd/blob/master/ebook/zh/03.01.md)

`删除修复`的c++代码:

```bash
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

```

目的是将树转换到`情况4`的状态，此时可以通过旋转来实现黑色节点深度的平衡。

















