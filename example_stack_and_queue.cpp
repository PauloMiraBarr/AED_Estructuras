int main() {
    stack_<int> s;
    s.push(10);
    s.push(20);
    cout << s.top() << endl; // 20
    s.pop();
    cout << s.top() << endl; // 10

    queue_<int> q;
    q.push(1);
    q.push(2);
    q.push(3);
    while (!q.empty()) {
        cout << q.front() << " ";
        q.pop();
    }
    // salida: 1 2 3
}
