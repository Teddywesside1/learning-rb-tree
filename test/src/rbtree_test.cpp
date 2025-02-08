#include "rbtree.hpp"

#include "gtest/gtest.h"
#include "glog/logging.h"
#include "glog/log_severity.h"

#include <chrono>

#include <set>

#define MESSURE_DURATION(run)                                                                \
  {                                                                                          \
    auto start = std::chrono::high_resolution_clock::now();                                  \
    (run);                                                                                   \
    auto end = std::chrono::high_resolution_clock::now();                                    \
    LOG(INFO) << #run << " cost(us): "                                                       \
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count(); \
  }


TEST(rbtree, insertion) 
{
  RBTree<int> tree;

  for (int i = 0 ; i < 1e6 ; ++ i) {
    tree.insert(i);
  }

  CHECK(tree.is_valid_rb_tree());

  LOG(INFO) << "Got A valid rb_tree, with size : " << tree.size();
}

TEST(rbtree, insertion_random) 
{
  RBTree<int> tree;

  for (int i = 0 ; i < 1e6 ; ++ i) {
    int rd = rand();
    try {
      tree.insert(rd);
    } catch (const std::exception& e) {
      LOG(INFO) << "rd : " << rd << ", got ex :" << e.what(); 
    }
  }

  CHECK(tree.is_valid_rb_tree());

  LOG(INFO) << "Got A valid rb_tree, with size : " << tree.size();
}



TEST(rbtree, lookup) 
{
  const int SIZE = 1E6;

  RBTree<int> tree;

  for (int i = 0 ; i < SIZE ; ++ i) {
    tree.insert(i);
  }
  CHECK(tree.is_valid_rb_tree());
  LOG(INFO) << "Got A valid rb_tree, with size : " << tree.size();

  auto test_func = [&]() {
    for (int i = 0 ; i < SIZE ; ++ i) {
      int rd = rand() % SIZE;
      auto node = tree.find_parent(rd);
      CHECK(node != nullptr && node->key == rd);
    }
  };

  MESSURE_DURATION(test_func());
}


TEST(stl_set, lookup) 
{
  const int SIZE = 1E6;

  std::set<int> tree;

  for (int i = 0 ; i < SIZE ; ++ i) {
    tree.insert(i);
  }

  auto test_func = [&]() {
    for (int i = 0 ; i < SIZE ; ++ i) {
      int rd = rand() % SIZE;
      auto itr = tree.find(rd);
      CHECK(itr != tree.end() && *itr == rd);
    }
  };

  MESSURE_DURATION(test_func());
}