#define BINARYTREE_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <vector>
#include <stdexcept>
using namespace std;

namespace ariel
{
  template <typename T>
  class BinaryTree{
    public:
    // inner class
      struct Node
      {
        Node *left;
        Node *right;
        T type;
        
        Node(const T &num) : type(num), left(nullptr), right(nullptr) {}

        Node(const Node &temp): type(temp.type){
            if (temp.left!=nullptr){
                left = new Node(*temp.left);
            }
            else{
                left = nullptr;
            }
            if (temp.right!=nullptr){
                right = new Node(*temp.right);
            }
            else{
                right = nullptr;
            }
        }

        ~Node(){
          // left= nullptr;
          // right= nullptr;
        }

      };

      // Avoid copying
      BinaryTree &operator=(BinaryTree &&temp)noexcept {
        if (root!= nullptr){
          remove(root);
        }
        root = temp.root;
        temp.root = nullptr;
        return *this;
      }

      BinaryTree<T> &operator=(const BinaryTree<T> &temp){
        if (this != &temp)
        {
          remove(root);
          root = new Node (*temp.root);
        }
        return *this;
      }

      BinaryTree(const BinaryTree &&temp)noexcept : root(temp.root){}

      BinaryTree(BinaryTree &&temp) noexcept{
        root=temp.root;
        temp.root=nullptr;
      }

      BinaryTree(BinaryTree const &temp) : root(nullptr){
        Node const *temp_node = temp.root;
        this->add_root(temp_node->type);
      }
      
      // fields
      Node *root;
    
      BinaryTree() : root(nullptr) {}

      ~BinaryTree(){
        remove(root);
      }
      
      void remove(Node *root){
          if (root == NULL){
            return;
          }
          remove(root->right);
          remove(root->left);
          delete root;
      }

      Node *getRoot() { 
        return root; 
      }

      BinaryTree<T> &add_root(T value){
        if (root){
          root->type = value;
        }
        else {
          root = new Node(value);
        }
        return *this;
      }

      bool isEmpty() const { 
        return root==nullptr; 
      }

      Node *get_leaf(Node *root, T value){
        if (root == nullptr){
          return nullptr;
        }
        if (root->type == value){
            return root;
        }
        Node *foundNode = get_leaf(root->left, value);
        if (foundNode== nullptr){
            return get_leaf(root->right, value);
        }
        return foundNode;
      }

      BinaryTree<T> &add_left(T father, T left_child){
        Node *temp = get_leaf(root, father);
        if (temp == nullptr){
          throw std::runtime_error("The father value is not exsist");
        }
        if (temp->left != nullptr){
          temp-> left-> type = left_child;
        }
        else {
          temp->left = new Node (left_child);
        }
        return *this;
      }

      BinaryTree<T> &add_right(T father, T right_child){
        Node *temp = get_leaf(root, father);
        if (temp == nullptr){
          throw std::runtime_error("The father value is not exsist");
        }
        if (temp->right != nullptr){
          temp-> right-> type = right_child;
        }
        else {
          temp->right = new Node (right_child);
        }
        return *this;
      }

  
      class iterator
      {
        private:
          Node* pointer_to_current_node;
          std::vector<Node *> tree_order;
          int type; //1:preorder, 0:inorder, -1:postorder
        
          
          void preorder(){
            if (pointer_to_current_node == nullptr){
              return;
            }
            std::stack<Node *> pre_stack;
            pre_stack.push(pointer_to_current_node);
            Node *temp_node = nullptr;
            while (!pre_stack.empty()){
              temp_node= pre_stack.top();
              pre_stack.pop();
              tree_order.push_back(temp_node);
              if (temp_node->right){
                pre_stack.push(temp_node->right);
              }
              if (temp_node->left){
                pre_stack.push(temp_node->left);
              }
            }
          }
          
          void inorder(){
            if (pointer_to_current_node == nullptr){
              return;
            }
             std::stack<Node *> in_stack;
            Node *temp_node= pointer_to_current_node;
            while (!in_stack.empty()|| temp_node != nullptr){
              while(temp_node!= nullptr){
                in_stack.push(temp_node);
                temp_node= temp_node->left;
              }
              temp_node= in_stack.top();
              tree_order.push_back(temp_node);
              in_stack.pop();
              temp_node= temp_node->right;
            }
          }
          
          void postorder(){
            if (pointer_to_current_node == nullptr){
              return;
            }
             std::stack<Node *> post_stack;
            post_stack.push(pointer_to_current_node);
            Node *temp_node = nullptr;
            while (!post_stack.empty()){
              temp_node= post_stack.top();
              post_stack.pop();
              tree_order.push_back(temp_node);
              if (temp_node->left){
                post_stack.push(temp_node->left);
              }
              if (temp_node->right){
                post_stack.push(temp_node->right);
              }
            }
            reverse(tree_order.begin(),tree_order.end());
          }

        public:
          iterator(Node* ptr = nullptr, int t= 0): pointer_to_current_node(ptr),type(t){
              if (type == 0)
              {
                inorder();
              }
              else if (type == 1)
              {
                preorder();
              }
              else
              {
                postorder();
              }
          }

          T &operator*() const{
            if (pointer_to_current_node == nullptr){
              cout<<"null pointer"<<endl;
            }
            return pointer_to_current_node->type;
          }

          T *operator->() const { 
            return &(pointer_to_current_node->type); 
          }

          iterator &operator++(){
            if (!tree_order.empty()){
              pointer_to_current_node= tree_order.at(0);
              tree_order.erase(tree_order.begin());
            }
            else{
              pointer_to_current_node= nullptr;
            }
            return *this;
          }
          
          iterator operator++(int){
            iterator temp = *this;
            if (!tree_order.empty()){
              pointer_to_current_node= tree_order.at(0);
              tree_order.erase(tree_order.begin());
            }
            else{
              pointer_to_current_node= nullptr;
            }
            return temp;
          }

          bool operator==(const iterator &it) const{
            return pointer_to_current_node == it.pointer_to_current_node;
          }

          bool operator!=(const iterator &it) const{
            return pointer_to_current_node != it.pointer_to_current_node;
          }
      };
      
      iterator begin(){
        return ++iterator{root, 0};
      }
  
      iterator end(){
        return iterator{nullptr, 0};
      }

      iterator begin_preorder() { 
        return ++iterator{root, 1}; 
      }

      iterator end_preorder() { 
        return iterator{nullptr, 1}; 
      }

      iterator begin_postorder(){
        return ++iterator{root, -1};
      }

      iterator end_postorder(){
        return iterator{nullptr, -1};
      }

      iterator begin_inorder(){
        return ++iterator{root, 0};
      }

      iterator end_inorder(){
        return iterator{nullptr, 0};
      }
      
      friend ostream &operator<<(ostream &os, BinaryTree &bt){
        return os;
      }
  };
}