#include <functional>
#include <stdexcept>

using namespace std;

const int maxColision = 3;
const float maxFillFactor = 0.8;

template<typename TK, typename TV>
struct ChainHashNode {
    TK key;
    TV value;
    size_t hashcode; // evita calcular el hash varias veces
    ChainHashNode* next;
    ChainHashNode(const TK& k, const TV& v, size_t h)
        : key(k), value(v), hashcode(h), next(nullptr) {}
};

template<typename TK, typename TV>
class ChainHashListIterator {
private:
    using Node = ChainHashNode<TK, TV>;
    Node* ptr;

public:

    ChainHashListIterator(Node* p = nullptr) : ptr(p) {}

	// para compatibilidad con STL
    using value_type = Node;
    using reference = Node&;
    using pointer = Node*;

    reference operator*() const { return *ptr; }
    pointer   operator->() const { return ptr; }

    ChainHashListIterator& operator++() {
        if (ptr) ptr = ptr->next;
        return *this;
    }

    bool operator!=(const ChainHashListIterator& other) const {
        return ptr != other.ptr;
    }
    bool operator==(const ChainHashListIterator& other) const {
        return ptr == other.ptr;
    }

};

template<typename TK, typename TV>
class ChainHash
{
private:
    typedef ChainHashNode<TK, TV> Node;
    typedef ChainHashListIterator<TK, TV> Iterator;

	Node** array;  // array de punteros a Node
    int nsize; // total de elementos <key:value> insertados
	int capacity; // tamanio del array
	int *bucket_sizes; // guarda la cantidad de elementos en cada bucket
	int usedBuckets; // cantidad de buckets ocupados (con al menos un elemento)

public:
    ChainHash(int initialCapacity = 10){
		this->capacity = initialCapacity;
		this->array = new Node*[capacity]();
		this->bucket_sizes = new int[capacity]();
		this->nsize = 0;
		this->usedBuckets = 0;
	}

	TV get(TK key){
		size_t hashcode = getHashCode(key);
		size_t index = hashcode % capacity;

		Node* current = this->array[index];
		while(current != nullptr){
			if(current->key == key) return current->value;
			current = current->next;
		}
		throw std::out_of_range("Key no encontrado");
	}

	int size(){ return this->nsize; }

	int bucket_count(){ return this->capacity; }

	int bucket_size(int index) {
		if(index < 0 || index >= this->capacity) throw std::out_of_range("Indice de bucket invalido");
		return this->bucket_sizes[index];
	}

	// TODO: implementar los siguientes métodos
	void set(TK key, TV value){
		size_t h = getHashCode(key);
		size_t index = h%capacity;

        // actualizar si existe
        Node* cur = array[index];
        while (cur) {
            if (cur->key == key) {
                cur->value = value;
                return;
            }
            cur = cur->next;
        }

        // Insertar al frente
        Node* node = new Node(key, value, h);
        if (array[index] == nullptr) usedBuckets++;
        node->next = array[index];
        array[index] = node;

        nsize++;
        bucket_sizes[index]++;

        // condiciones de rehash
        if (bucket_sizes[index] > maxColision || fillFactor() > maxFillFactor) {
            rehashing();
        }
	}

	bool remove(TK key) {
        size_t h = getHashCode(key);
        size_t index = h % capacity;

        Node* cur = array[index];
        Node* prev = nullptr;

        while (cur) {
            if (cur->key == key) {
                if (prev) prev->next = cur->next;
                else array[index] = cur->next;

                delete cur;
                nsize--;
                bucket_sizes[index]--;
                if (array[index] == nullptr) usedBuckets--;
                return true;
            }
            prev = cur;
            cur = cur->next;
        }
        return false;
    }

    // la llave existe en el arreglo
    bool contains(TK key) {
        size_t h = getHashCode(key);
        size_t index = h % capacity;
        Node* cur = array[index];
        while (cur) {
            if (cur->key == key) return true;
            cur = cur->next;
        }
        return false;
    }

    // Iteradores por bucket
    Iterator begin(int index) {
        if (index < 0 || index >= capacity) throw std::out_of_range("Indice de bucket invalido");
        return Iterator(array[index]);
    }

    Iterator end(int index) {
        if (index < 0 || index >= capacity) throw std::out_of_range("Indice de bucket invalido");
        return Iterator(nullptr);
    }

private:
	double fillFactor(){
		return (double)this->usedBuckets / (double)this->capacity;
	}

	size_t getHashCode(TK key){
		std::hash<TK> ptr_hash;
		return ptr_hash(key);
	}

	// mover nodo a un nuevo arreglo
    void moveNodeTo(Node* node, Node** newArray, int* newBucketSizes, int newCap, int& newUsedBuckets) {
        node->next = nullptr;
        size_t idx = node->hashcode % newCap;
        if (newArray[idx] == nullptr) newUsedBuckets++;
        node->next = newArray[idx];
        newArray[idx] = node;
        newBucketSizes[idx]++;
    }

    void rehashing() {
        int newCap = capacity * 2;
        Node** newArray = new Node*[newCap]();
        int* newBucketSizes = new int[newCap]();
        int newUsed = 0;

        // transferir todos los nodos
        for (int i = 0; i < capacity; ++i) {
            Node* cur = array[i];
            while (cur) {
                Node* next = cur->next;
                // hashcode esta en los nodos
                moveNodeTo(cur, newArray, newBucketSizes, newCap, newUsed);
                cur = next;
            }
            array[i] = nullptr;
            bucket_sizes[i] = 0;
        }

        // liberar estructuras antiguas y reemplazar
        delete[] array;
        delete[] bucket_sizes;

        array = newArray;
        bucket_sizes = newBucketSizes;
        capacity = newCap;
        usedBuckets = newUsed;
    }

public:
    ~ChainHash() {
        // liberar todos los nodos
        for (int i = 0; i < capacity; ++i) {
            Node* cur = array[i];
            while (cur) {
                Node* nxt = cur->next;
                delete cur;
                cur = nxt;
            }
            array[i] = nullptr;
        }
        delete[] array;
        delete[] bucket_sizes;
        array = nullptr;
        bucket_sizes = nullptr;
        nsize = 0;
        capacity = 0;
        usedBuckets = 0;
    }
};
