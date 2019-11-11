//
// Created by Александр on 08.11.2019.
//

#ifndef AKINATOR_TREE_H
#define AKINATOR_TREE_H

template <typename T>
struct node {

    T data_;
    node* left_  = nullptr;
    node* right_ = nullptr;

    // Constructors

    explicit node(T el) {
        data_ = el;
    }

};

template <typename T>
struct Tree {
    int MAX_TREE_SIZE = 1024;

    int size_ = 0;

    node<T>* head_ = nullptr;

    // Methods

    ~Tree() {
        cout << "Tree was destructed" << endl;
        if (head_ != nullptr)
            free_tree(head_);
        head_ = nullptr;
    }

    void free_tree(node<T>* el) {
        assert(el != nullptr);
        if (el->left_ != nullptr)
            free_tree(el->left_);
        if (el->right_ != nullptr)
            free_tree(el->right_);
        free(el);
        size_--;
    }

    int push_left(node<T>* el, T value) {
        assert(el);

        size_++;
        el->left_ = make_node(value);
        return 0;
    }

    int push_right(node<T>* el, T value) {
        assert(el);

        size_++;
        el->right_ = make_node(value);
        return 0;
    }

    int push_head(T value) {
        if (head_ != nullptr) {
            dump(DUMP_INFO, "Tree was not free to use 'push_head' function");
            return 1;
        }

        size_ = 1;
        head_ = make_node(value);
        return 0;
    }

    node<T>* make_node(T value) {
        node<T>* new_node = (node<T>*)calloc(1, sizeof(node<T>));

        if (new_node == nullptr) {
            dump(DUMP_INFO, "Memory was not allocated");
            assert(ERROR);
            return nullptr;
        }

        new_node->data_ = value;

        return new_node;
    }

};


#endif //AKINATOR_TREE_H
