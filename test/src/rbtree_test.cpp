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

  int SIZE = 1e6;

  auto func_test_rbtree = [=]() {
    RBTree<int> tree;
    for (int i = 0 ; i < SIZE ; ++ i) {
      int rd = rand();
      try {
        tree.insert(rd);
      } catch (const std::exception& e) {
        // LOG(INFO) << "rd : " << rd << ", got ex :" << e.what(); 
      }
    }    
    CHECK(tree.is_valid_rb_tree());
  };

  auto func_test_stl_set = [=]() {
    std::set<int> stl_set;
    for (int i = 0 ; i < SIZE ; ++ i) {
      stl_set.insert(rand());
    }
  };

  MESSURE_DURATION(func_test_rbtree());
  MESSURE_DURATION(func_test_stl_set());

}



TEST(rbtree, lookup_random) 
{
  const int SIZE = 1E6;

  RBTree<int> tree;

  for (int i = 0 ; i < SIZE ; ++ i) {
    tree.insert(i);
  }
  CHECK(tree.is_valid_rb_tree());
  LOG(INFO) << "Got A valid rb_tree, with size : " << tree.size();

  auto func_test_rbtree = [&]() {
    for (int i = 0 ; i < SIZE ; ++ i) {
      int rd = rand() % SIZE;
      auto node = tree.find_parent(rd);
      CHECK(node != nullptr && node->key == rd);
    }
  };

  MESSURE_DURATION(func_test_rbtree());

  std::set<int> stl_set;

  for (int i = 0 ; i < SIZE ; ++ i) {
    stl_set.insert(i);
  }

  auto func_test_stl_set = [&]() {
    for (int i = 0 ; i < SIZE ; ++ i) {
      int rd = rand() % SIZE;
      auto itr = stl_set.find(rd);
      CHECK(itr != stl_set.end() && *itr == rd);
    }
  };

  MESSURE_DURATION(func_test_stl_set());
}


TEST(rbtree, remove) 
{
  const int SIZE = 1e6;

  RBTree<int> tree;

  for (int i = 0 ; i < SIZE ; ++ i) {
    tree.insert(i);
  }
  CHECK(tree.is_valid_rb_tree());
  tree.print();
  LOG(INFO) << "Got A valid rb_tree, with size : " << tree.size();
  
  for (int i = 0 ; i < SIZE ; ++ i) {
    try {
      tree.remove(i);
    } catch (const std::exception& e) {
      LOG(INFO) << "Remove got invalid key : " << i;
    }
  }
  CHECK(tree.is_valid_rb_tree());
}


TEST(rbtree, remove_random) 
{
  const int SIZE = 1e6;

  RBTree<int> tree;

  for (int i = 0 ; i < SIZE ; ++ i) {
    tree.insert(i);
  }
  CHECK(tree.is_valid_rb_tree());
  LOG(INFO) << "Got A valid rb_tree, with size : " << tree.size();
  
  auto func_test_rbtree = [&]() {
    for (int i = 0 ; i < SIZE ; ++ i) {
      int rd = rand() % SIZE;
      tree.remove(rd);
    }
    CHECK(tree.is_valid_rb_tree());
  };

  MESSURE_DURATION(func_test_rbtree());

  std::set<int> stl_set;
  for (int i = 0 ; i < SIZE ; ++ i) {
    stl_set.insert(i);
  }
  auto func_test_stl_set = [&]() {
    for (int i = 0 ; i < SIZE ; ++ i) {
      int rd = rand() % SIZE;
      stl_set.erase(rd);
    }
  };

  MESSURE_DURATION(func_test_stl_set());
}