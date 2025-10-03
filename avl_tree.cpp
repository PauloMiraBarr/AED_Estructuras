#include <iostream>


#include <algorithm>
using namespace std;
template <typename T>
class AVLTree {
private:
    struct Node {
        T data;
        Node* left;
        Node* right;
        int height;
        Node(T val) : data(val), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;

    int height(Node* node) {
        return (node ? (node -> height) : 0);
    }

    int balanceFactor(Node* node) {
        return (node ? height(node -> left) - height(node -> right) : 0);
    }

    // Rotación a la derecha
    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    // Rotación a la izquierda
    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    Node* insert(Node* node, T value) {
        if(!node)
            return new Node(value);

        if(value < node->data)
            node->left = insert(node->left, value);
        else if(value > node->data)
            node->right = insert(node->right, value);
        else
            return node;

        node->height = 1 + std::max(height(node->left), height(node->right));

        int balance = balanceFactor(node);

        // Caso de rotación a la derecha
        if (balance > 1 and value < node->left->data)
            return rotateRight(node);

        // Caso de rotación a la izquierda
        if(balance < -1 and value > node->right->data)
            return rotateLeft(node);

        // Rotación izquierda-derecha
        if(balance > 1 and value > node->left->data) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // Rotación derecha-izquierda
        if(balance < -1 and value < node->right->data) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    void inOrder(Node* node) {
        if(!node) return;
        inOrder(node -> left);
        cout << (node -> data) << " ";
        inOrder(node -> right);
    }

    Node* min_node(Node* node) {
        Node* current = node;
        while(current and current -> left)
            current = current -> left;
        return current;
    }

    Node* deleteNode(Node* root, T value) {
        if(!root) return root;

        if(value < root->data) {
            root->left = deleteNode(root->left, value);
        }
        else if(value > root->data) {
            root->right = deleteNode(root->right, value);
        }
        else {
            if(!root->left) {
                Node* temp = root->right;
                delete root;
                return temp;
            }
            if(!root->right) {
                Node* temp = root->left;
                delete root;
                return temp;
            }

            Node* temp = min_node(root->right);
            root->data = temp->data;
            root->right = deleteNode(root->right, temp->data);
        }

        root->height = 1 + std::max(height(root->left), height(root->right));

        int balance = balanceFactor(root);

        if(balance > 1 and balanceFactor(root->left) >= 0)
            return rotateRight(root);

        if(balance > 1 and balanceFactor(root->left) < 0) {
            root->left = rotateLeft(root->left);
            return rotateRight(root);
        }

        if(balance < -1 and balanceFactor(root->right) <= 0)
            return rotateLeft(root);

        if(balance < -1 and balanceFactor(root->right) > 0) {
            root->right = rotateRight(root->right);
            return rotateLeft(root);
        }

        return root;
    }

public:
    AVLTree() : root(nullptr) {}

    void insert(T value) {
        root = insert(root, value);
    }

    void printInOrder() {
        inOrder(root);
        cout << std::endl;
    }

    void erase(T val) {
        root = deleteNode(root, val); 
    }

    Node* find(T val) {
        Node* current = root;
        while(current) {
            if(val == current -> data)
                return current;
            if(val < current->data)
                current = current->left;
            else
                current = current->right;
        } return nullptr;
    }
};

int main() {
    AVLTree<int> intTree;
    intTree.insert(30);
    intTree.insert(20);
    intTree.insert(10);
    intTree.insert(5);
    intTree.insert(25);
    intTree.insert(40);
    intTree.insert(50);

    cout << "Arbol AVL de enteros en orden antes de eliminar 25:\n";
    intTree.printInOrder(); cout << "\n";

    intTree.erase(25);
    cout << "Arbol AVL de enteros en orden despues de eliminar 25:\n";
    intTree.printInOrder(); cout << "\n";

    cout << "40 se encuentra en el AVL Tree: " << (intTree.find(40)? "YES" : "NO") << "\n";
    cout << "91 se encuentra en el AVL Tree: " << (intTree.find(91)? "YES" : "NO") << "\n";

    cout << "\n";

    AVLTree<float> floatTree;
    floatTree.insert(30.5f);
    floatTree.insert(20.3f);
    floatTree.insert(10.7f);

    cout << "Arbol AVL de flotantes en orden antes de eliminar 20.3:\n";
    floatTree.printInOrder(); cout << "\n";

    floatTree.erase(20.3f);
    cout << "Arbol AVL de flotantes en orden despues de eliminar 20.3:\n";
    floatTree.printInOrder(); cout << "\n";

    AVLTree<char> charTree;
    charTree.insert('F');
    charTree.insert('D');
    charTree.insert('A');
    charTree.insert('B');

    cout << "Arbol AVL de caracteres en orden antes de eliminar 'D':\n";
    charTree.printInOrder(); cout << "\n";

    charTree.erase('D');
    cout << "Arbol AVL de caracteres en orden despues de eliminar 'D':\n";
    charTree.printInOrder(); cout << "\n";

    return 0;
}
