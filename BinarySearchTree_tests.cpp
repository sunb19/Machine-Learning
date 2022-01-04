// Project UID db1f506d06d84ab787baf250c265e24e

#include "BinarySearchTree.h"
#include "unit_test_framework.h"

TEST(test_begin_end){
    BinarySearchTree<int> tree;
    ASSERT_TRUE(tree.begin() == tree.end());
    tree.insert(1);
    ASSERT_TRUE(*tree.begin()==1);
    ASSERT_TRUE(tree.find(1) != tree.end());

    tree.insert(3);
    tree.insert(4);
    ASSERT_TRUE(*tree.begin()==1);
    ASSERT_TRUE(tree.find(3) != tree.end());
    ASSERT_TRUE(tree.find(4) != tree.end());


}

TEST(test_find){
    BinarySearchTree<int> tree;
    BinarySearchTree<int>::Iterator i;
    BinarySearchTree<int>::Iterator i1;

    ASSERT_TRUE(tree.find(5) == tree.end());
    tree.insert(4);
    tree.insert(10);
    i = tree.insert(-5);
    tree.insert(0);
    i1 = tree.insert(12);
    tree.insert(8);
    ASSERT_TRUE(tree.find(-5) == i);
    ASSERT_TRUE(tree.find(5) == tree.end());
    ASSERT_TRUE(tree.find(12) == i1);
}

TEST(test_insert){
    BinarySearchTree<int> tree;
    BinarySearchTree<int>::Iterator i;
    BinarySearchTree<int>::Iterator i2;

    i = tree.insert(1);
    ASSERT_EQUAL(tree.size(),1u);
    ASSERT_TRUE((*i) == 1);


    i2 = tree.insert(2);
    ASSERT_EQUAL(tree.size(),2u);
    ASSERT_TRUE(i != i2);
    ASSERT_TRUE((*i2) == 2);
}

TEST(test_empty) {
    BinarySearchTree<int> tree;
    ASSERT_EQUAL(tree.empty(),true);
    tree.insert(1);
    ASSERT_EQUAL(tree.empty(),false);
}


TEST(max_min){
    BinarySearchTree<int> tree;
    ASSERT_TRUE(tree.max_element() == tree.end());
    ASSERT_TRUE(tree.min_element() == tree.end());
    BinarySearchTree<int> tree2;
    tree.insert(5);
    ASSERT_TRUE(*tree.max_element() == 5);
    ASSERT_TRUE(*tree.min_element() == 5);
    tree.insert(6);
    tree.insert(100);
    tree2.insert(-10);
    tree2.insert(0);
    tree2.insert(-20);
    ASSERT_TRUE(*tree.max_element() == 100);
    ASSERT_TRUE(*tree2.max_element() == 0);
    ASSERT_TRUE(*tree.min_element() == 5);
    ASSERT_TRUE(*tree2.min_element() == -20);
}

TEST(test_height){
    BinarySearchTree<int> tree;
    ASSERT_EQUAL(tree.height(),0u);

    ASSERT_EQUAL(tree.height(), 0u);
    for(int i =0; i<5 ;++ i){
        tree.insert(i);
    }
    ASSERT_EQUAL(tree.height(),5u);
    tree.insert(-2);
    tree.insert(-4);
    ASSERT_EQUAL(tree.height(),5u);
}

TEST(test_size){
    BinarySearchTree<int> tree;
    ASSERT_EQUAL(tree.size(),0u);

    ASSERT_EQUAL(tree.size(),0u);
    for(int i =0; i<5 ;++ i){
        tree.insert(i);
    }
    ASSERT_EQUAL(tree.size(),5u);
    tree.insert(-2);
    tree.insert(-4);
    ASSERT_EQUAL(tree.size(),7u);
}
TEST(traverse_inorder){
    BinarySearchTree<int> tree;

    std::ostringstream oss_inorder;
    tree.traverse_inorder(oss_inorder);
    ASSERT_EQUAL(oss_inorder.str(), "");
    tree.insert(4);
    tree.insert(10);
    tree.insert(-5);
    tree.insert(0);
    tree.insert(12);
    tree.insert(8);
    tree.traverse_inorder(oss_inorder);
    ASSERT_EQUAL(oss_inorder.str(), "-5 0 4 8 10 12 ");
}

TEST(traverse_preorder){
    BinarySearchTree<int> tree;

    std::ostringstream oss_preorder;
    tree.traverse_preorder(oss_preorder);
    ASSERT_EQUAL(oss_preorder.str(), "");
    tree.insert(4);
    tree.insert(10);
    tree.insert(-5);
    tree.insert(0);
    tree.insert(12);
    tree.insert(8);
    tree.traverse_preorder(oss_preorder);
    ASSERT_EQUAL(oss_preorder.str(), "4 -5 0 10 8 12 ");
}

TEST(test_check_sorting_invariant){
    BinarySearchTree<int> tree;
    BinarySearchTree<int> tree2;

    BinarySearchTree<int>::Iterator it;
    BinarySearchTree<int>::Iterator it2;


    ASSERT_TRUE(tree.check_sorting_invariant());
    tree.insert(1);
    ASSERT_TRUE(tree.check_sorting_invariant());
    tree.insert(2);
    ASSERT_TRUE(tree.check_sorting_invariant());
    it2 = tree.insert(-2);
    ASSERT_TRUE(tree.check_sorting_invariant());
    it = tree.insert(5);
    ASSERT_TRUE(tree.check_sorting_invariant());
    *it = -10;
    ASSERT_FALSE(tree.check_sorting_invariant());

    ASSERT_TRUE(tree2.check_sorting_invariant());
    tree2.insert(0);
    ASSERT_TRUE(tree2.check_sorting_invariant());
    tree.insert(5);
    ASSERT_TRUE(tree2.check_sorting_invariant());
    it2 = tree2.insert(-10);
    ASSERT_TRUE(tree2.check_sorting_invariant());
    tree2.insert(30);
    ASSERT_TRUE(tree2.check_sorting_invariant());
    *it2 = 20;
    ASSERT_FALSE(tree2.check_sorting_invariant());


 }

TEST(test_min_greater_than){
    BinarySearchTree<int> tree;
    ASSERT_EQUAL(tree.min_greater_than(9), tree.end());
    tree.insert(5);
    ASSERT_EQUAL(tree.min_greater_than(5), tree.end());
    tree.insert(7);
    tree.insert(3);
    ASSERT_TRUE(*tree.min_greater_than(5) == 7);
    tree.insert(10);
    ASSERT_TRUE(*tree.min_greater_than(7) == 10);
    tree.insert(-20);
    ASSERT_TRUE(*tree.min_greater_than(-30) == -20);

}

TEST(test_equals_copy){
    BinarySearchTree<int> tree;
    BinarySearchTree<int> tree2;
    BinarySearchTree<int> copiedTree2(tree2);
    BinarySearchTree<int> tree3;
    BinarySearchTree<int> tree4;

    ASSERT_EQUAL(tree.size(), 0u);
    ASSERT_TRUE(tree.empty());
    ASSERT_EQUAL(tree.height(), 0u);
    ASSERT_EQUAL(copiedTree2.size(), 0u);
    ASSERT_TRUE(copiedTree2.empty());
    ASSERT_EQUAL(copiedTree2.height(), 0u);

    tree.insert(0);
    tree.insert(7);
    tree.insert(-2);
    tree.insert(5);
    tree2 = tree;
    BinarySearchTree<int> copiedTree(tree2);
    ASSERT_EQUAL(tree.size(), 4u);
    ASSERT_FALSE(tree.empty());
    ASSERT_EQUAL(tree.height(), 3u);
    ASSERT_TRUE(*tree.max_element() == 7);
    ASSERT_TRUE(*tree.min_element() == -2);
    ASSERT_TRUE(*tree.min_greater_than(6) == 7);

    ASSERT_EQUAL(tree2.size(), 4u);
    ASSERT_FALSE(tree2.empty());
    ASSERT_EQUAL(tree2.height(), 3u);
    ASSERT_TRUE(*tree2.max_element() == 7);
    ASSERT_TRUE(*tree2.min_element() == -2);
    ASSERT_TRUE(*tree2.min_greater_than(6) == 7);

    ASSERT_EQUAL(copiedTree.size(), 4u);
    ASSERT_FALSE(copiedTree.empty());
    ASSERT_EQUAL(copiedTree.height(), 3u);   
    ASSERT_TRUE(*copiedTree.max_element() == 7);
    ASSERT_TRUE(*copiedTree.min_element() == -2);
    ASSERT_TRUE(*copiedTree.min_greater_than(6) == 7);
}

TEST_MAIN()