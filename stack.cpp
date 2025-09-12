#include <iostream>
using namespace std;

template<typename T>
struct node_s {
    T data;
    node_s<T>* next = nullptr;
    node_s(T d, node_s<T>* n=nullptr) : data(d), next(n) {}
};

template<typename T>
struct stack_ {
    node_s<T>* top_ = nullptr;

    void push(const T& e) {
        top_ = new node_s<T>(e, top_);
    }

    void pop() {
        if (top_ == nullptr) return;
        auto temp = top_;
        top_ = top_->next;
        delete temp;
    }

    T top() {
        return top_->data;
    }

    bool empty() {
        return top_ == nullptr;
    }
};
