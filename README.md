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
  2. 父节点为红色：
     - 首先调整树的颜色：
       1. 首先，父节点一定存在它自己的父节点，且颜色为黑色（否则违反**性质2**和**性质3**）。
       2. 要做的操作是：令父节点变成黑色，祖父节点变成红色，若因祖父节点的变色，导致出现连续红色节点，则重复这一操作。
       3. 期间，若父节点的另一子节点（叔叔节点）也是红色，则将其染成黑色。
     - 对树进行旋转：
       1. 选择最后一个染成红色的祖父节点，对树进行旋转，保证红黑树的黑节点平衡。
  
