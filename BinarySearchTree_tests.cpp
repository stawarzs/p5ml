// Project UID db1f506d06d84ab787baf250c265e24e

#include "BinarySearchTree.h"
#include "unit_test_framework.h"

using namespace std;
TEST(test_is_empty) {
    BinarySearchTree<int> t;
    ASSERT_TRUE(t.empty())
}

TEST(test_is_not_empty) {
    BinarySearchTree<int> t;
    t.insert(7);
    ASSERT_FALSE(t.empty());
}

TEST(test_size_none) {
    BinarySearchTree<int> t;
    ASSERT_EQUAL(t.size(), 0u);
}

TEST(test_size_several) {
    BinarySearchTree<int> t;
    t.insert(7);
    t.insert(8);
    t.insert(9);
    t.insert(10);
    ASSERT_EQUAL(t.size(), 4u);
}

TEST(test_height_none) {
    BinarySearchTree<int> t;
    ASSERT_EQUAL(t.height(), 0u);
}

TEST(test_height_one) {
    BinarySearchTree<int> t;
    t.insert(3);
    ASSERT_EQUAL(t.height(), 1u);
}

TEST(test_tree_find){
    BinarySearchTree<int> t;
    t.insert(3);
    ASSERT_TRUE(t.find(3) != t.end());
    ASSERT_TRUE(t.find(5) == t.end());
}

TEST(min_greater_than_full){
    BinarySearchTree<int> t;
    


    t.insert(8);
    

    t.insert(6);
    t.insert(9);
    t.insert(10);
    t.insert(33);
    
    //cout << "cout << t.to_string()" << endl;
    //cout << t.to_string() << endl << endl;
    //t.traverse_inorder(t.begin(), std::cout);
/*
    if (less(max_element_impl(node)->datum, val)) {
      std::cout << "if 2" << std::endl;

      return nullptr;
    }
    */
    ASSERT_EQUAL(*t.min_greater_than(11), 33);
    ASSERT_EQUAL(*t.min_greater_than(1), 6);
    ASSERT_EQUAL(*t.min_greater_than(-208), 6);
    ASSERT_EQUAL(*t.min_greater_than(7), 8);
    ASSERT_EQUAL(*t.min_greater_than(9), 10);
    ASSERT_TRUE(*t.min_greater_than(8) != 8);
    ASSERT_TRUE(t.min_greater_than(200) == t.end());
}

TEST(min_greater_than_empty){
    BinarySearchTree<int> t;
    ASSERT_EQUAL(t.min_greater_than(10), t.end());
}

TEST(min_val_none){
    BinarySearchTree<int> t;
    ASSERT_EQUAL(t.min_element(), t.end());
}

TEST(min_val_one){
    BinarySearchTree<int> t;
    t.insert(1);
    ASSERT_EQUAL(*t.min_element(), 1);
}

TEST(min_val_many){
    BinarySearchTree<int> t;
    t.insert(7);
    t.insert(8);
    t.insert(9);
    t.insert(10);
    ASSERT_EQUAL(*t.min_element(), 7);
}

TEST(max_val_none){
    BinarySearchTree<int> t;
    ASSERT_EQUAL(t.max_element(), t.end());
}

TEST(max_val_one){
    BinarySearchTree<int> t;
    t.insert(1);
    ASSERT_EQUAL(*t.max_element(), 1);
}

TEST(max_val_many){
    BinarySearchTree<int> t;
    t.insert(7);
    t.insert(8);
    t.insert(9);
    t.insert(10);
    ASSERT_EQUAL(*t.max_element(), 10);
}

TEST(begin){
    BinarySearchTree<int> t;
    t.insert(7);
    t.insert(8);
    t.insert(9);
    t.insert(10);
    ASSERT_EQUAL(*t.begin(), 7);
}

TEST(begin_empty){
    BinarySearchTree<int> t;
    ASSERT_EQUAL(t.begin(), t.end());
}

TEST(end){
    BinarySearchTree<int> t;
    t.insert(7);
    t.insert(8);
    t.insert(9);
    t.insert(10);
    ASSERT_EQUAL(t.end(), t.end());
}

TEST(tree_find_invariance_complicated){
    BinarySearchTree<int> t;
    t.insert(99);
    t.insert(33);
    t.insert(2);
    t.insert(100);
    t.insert(13);
    t.insert(88);    
    t.insert(1);
    t.insert(8);
    t.insert(212);
    t.insert(39);
    t.insert(999);
    t.insert(918);
    t.insert(75);    
    t.insert(57);


    ASSERT_TRUE(t.find(99) != t.end());
    ASSERT_TRUE(t.find(33) != t.end());
    ASSERT_TRUE(t.find(2) != t.end());
    ASSERT_TRUE(t.find(100) != t.end());
    ASSERT_TRUE(t.find(13) != t.end());
    ASSERT_TRUE(t.find(88) != t.end());
    ASSERT_TRUE(t.find(1) != t.end());
    ASSERT_TRUE(t.find(8) != t.end());
    ASSERT_TRUE(t.find(212) != t.end());
    ASSERT_TRUE(t.find(39) != t.end());
    ASSERT_TRUE(t.find(999) != t.end());
    ASSERT_TRUE(t.find(918) != t.end());
    ASSERT_TRUE(t.find(75) != t.end());
    ASSERT_TRUE(t.find(57) != t.end());

    *t.find(1) = 13000;

    ASSERT_FALSE(t.check_sorting_invariant());

}


TEST(check_check_sorting_invariant) {
    BinarySearchTree<int> t;
    ASSERT_TRUE(t.check_sorting_invariant());
    t.insert(50);
    t.insert(75);
    t.insert(60);
    t.insert(69);
    t.insert(68);
    t.insert(12);  
    t.insert(11);    
    t.insert(13);   
    t.insert(100);   
    t.insert(101);  
    t.insert(102);   

    //cout << "cout << t.to_string()" << endl;
    //cout << t.to_string() << endl << endl;

    ASSERT_TRUE(t.check_sorting_invariant());   
    *(t.begin()) =  1000;

    //cout << "cout << t.to_string()" << endl;
    //cout << t.to_string() << endl << endl;

    ASSERT_FALSE(t.check_sorting_invariant()); 

    // good so far

    *(t.begin()) =  -10;
    cout << t.to_string() << endl << endl;
    ASSERT_TRUE(t.check_sorting_invariant());   

    *(++++++(t.begin())) =  -11;
    //cout << "cout << t.to_string()" << endl;
    //cout << t.to_string() << endl << endl;

    ASSERT_FALSE(t.check_sorting_invariant()); 
    *(t.begin()) =  50;
    //cout << "cout << t.to_string()" << endl;
    //cout << t.to_string() << endl << endl;
    



    BinarySearchTree<int> tt;

    tt.insert(50);
    tt.insert(75);
    tt.insert(60);
    tt.insert(69);
    tt.insert(68);
    tt.insert(12);  
    tt.insert(11);    
    tt.insert(13);   
    tt.insert(100);   
    tt.insert(101);  
    tt.insert(102);   
    ASSERT_TRUE(tt.check_sorting_invariant()); 
    *(++++++(tt.begin())) = 500;
    ASSERT_FALSE(tt.check_sorting_invariant()); 

    //50, 40, 80, 55, 95
    BinarySearchTree<int> blah;

    blah.insert(50);
    blah.insert(40);
    blah.insert(80);
    blah.insert(55);
    blah.insert(95);

    *blah.find(55) = 0;

    ASSERT_FALSE(blah.check_sorting_invariant()); 


}   

TEST(traverses) {
    BinarySearchTree<int> t;

    t.insert(50);
    t.insert(75);
    t.insert(60);
    t.insert(69);
    t.insert(68);
    t.insert(12);  
    t.insert(11);    
    t.insert(13);   
    t.insert(100); 
    t.insert(101);  
    t.insert(102);  

    ostringstream osinorder;
    t.traverse_inorder(osinorder);
    cout << "inorder" << endl;
    cout << osinorder.str() << endl << endl;

    cout << "cout << t.to_string()" << endl;
    cout << t.to_string() << endl << endl;

    ASSERT_TRUE(osinorder.str() == "11 12 13 50 60 68 69 75 100 101 102 ");

    ostringstream oss_preorder;
    t.traverse_preorder(oss_preorder);
    cout << "preorder" << endl;
    cout << oss_preorder.str() << endl << endl;
    ASSERT_TRUE(oss_preorder.str() == "50 12 11 13 75 60 69 68 100 101 102 ");

    BinarySearchTree<int> e;

    ostringstream osinorderE;
    e.traverse_inorder(osinorderE);
    cout << "inorder" << endl;
    cout << osinorderE.str() << endl << endl;

    cout << "cout << e.to_string()" << endl;
    cout << e.to_string() << endl << endl;

    ASSERT_TRUE(osinorderE.str() == "");

    ostringstream oss_preorderE;
    e.traverse_preorder(oss_preorderE);
    cout << "preorder" << endl;
    cout << oss_preorderE.str() << endl << endl;
    ASSERT_TRUE(oss_preorderE.str() == "");

}

TEST_MAIN()