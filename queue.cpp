template<typename T>
struct node_q {
    T data;
    node_q<T>* next = nullptr;
    node_q(T d) : data(d) {}
};

template<typename T>
struct queue_ {
    node_q<T>* front_ = nullptr;
    node_q<T>* back_  = nullptr;

    void push(const T& e) {
        auto n = new node_q<T>(e);
        if (back_ == nullptr) {
            front_ = back_ = n;
        } else {
            back_->next = n;
            back_ = n;
        }
    }

    void pop() {
        if (front_ == nullptr) return;
        auto temp = front_;
        front_ = front_->next;
        if (front_ == nullptr) back_ = nullptr;
        delete temp;
    }

    T front() {
        return front_->data;
    }

    bool empty() {
        return front_ == nullptr;
    }
};
