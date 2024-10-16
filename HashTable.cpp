#include <iostream>
#include <cmath>

using namespace std;

class HashTable {
public:
    HashTable(int initial_size = 7);
    ~HashTable();

    void insert(int key);
    void remove(int key);
    int search(int key);
    void printTable();
    
private:
    struct Entry {
        int key;
        bool is_empty;
        bool is_deleted;
    };

    Entry* table;
    int size;
    int num_elements;
    float load_factor = 0.8;
    int max_probing_limit = 1000;

    int hash(int key);
    int quadratic_probe(int key, int i);
    bool is_prime(int n);
    int next_prime(int n);
    void resize();
};

// Constructor for HashTable
HashTable::HashTable(int initial_size) {
    size = next_prime(initial_size);  // Get the next prime number for size
    table = new Entry[size];
    num_elements = 0;

    for (int i = 0; i < size; ++i) {
        table[i].key = -1;          // Initialize key to -1 (indicating empty slot)
        table[i].is_empty = true;   // Mark slot as empty
        table[i].is_deleted = false; // Slot is not deleted
    }
}

// Destructor for HashTable
HashTable::~HashTable() {
    delete[] table;  // Free memory
}

// Hash function (modulus operator)
int HashTable::hash(int key) {
    return key % size;  
}

// Quadratic probing function
int HashTable::quadratic_probe(int key, int i) {
    return (hash(key) + i * i) % size;  
}

// Check if a number is prime
bool HashTable::is_prime(int n) {
    if (n < 2) return false;
    for (int i = 2; i <= sqrt(n); ++i) {
        if (n % i == 0) return false;
    }
    return true;
}

// Find the next prime number
int HashTable::next_prime(int n) {
    while (!is_prime(n)) {
        ++n;
    }
    return n;
}

// Resize the hash table when load factor is exceeded
void HashTable::resize() {
    int new_size = next_prime(size * 2);  // Find the next prime after doubling size
    Entry* old_table = table;  
    int old_size = size;

    size = new_size;
    table = new Entry[size];  // Allocate new table with new size

    for (int i = 0; i < size; ++i) {
        table[i].key = -1;          // Initialize to empty
        table[i].is_empty = true;   // Mark as empty
        table[i].is_deleted = false; // Not deleted
    }

    int old_num_elements = num_elements;  
    num_elements = 0;  // Reset element count

    // Rehash all the non-deleted keys from the old table
    for (int i = 0; i < old_size; ++i) {
        if (!old_table[i].is_empty && !old_table[i].is_deleted) {
            insert(old_table[i].key);  // Reinsert old keys
        }
    }

    delete[] old_table;  // Delete old table
}

// Insert function with quadratic probing and resizing logic
void HashTable::insert(int key) {
    // Prevent duplicate key insertion
    if (search(key) != -1) {
        cout << "Duplicate key insertion is not allowed" << endl;
        return;
    }

    // Resize if load factor is exceeded
    if ((float)num_elements / size >= load_factor) {
        resize(); 
    }

    int i = 0;
    while (i < max_probing_limit) {
        int index = quadratic_probe(key, i);
        if (table[index].is_empty || table[index].is_deleted) {
            table[index].key = key;        // Insert the key
            table[index].is_empty = false; // Mark as non-empty
            table[index].is_deleted = false; // Mark as not deleted
            ++num_elements;
            return;
        }
        ++i;
    }

    cout << "Max probing limit reached!" << endl;
}

// Search function with quadratic probing
int HashTable::search(int key) {
    int i = 0;
    while (i < max_probing_limit) {
        int index = quadratic_probe(key, i);
        if (table[index].is_empty) {
            return -1;  // Not found
        }
        if (table[index].key == key && !table[index].is_deleted) {
            return index;  // Key found
        }
        ++i;
    }
    return -1;  // Not found after max probing limit
}

// Remove function for marking an entry as deleted
void HashTable::remove(int key) {
    int index = search(key);
    if (index == -1) {
        cout << "Element not found" << endl;
        return;
    }

    table[index].is_deleted = true;  // Mark as deleted
    --num_elements;
}

// Print the hash table
void HashTable::printTable() {
    for (int i = 0; i < size; ++i) {
        if (table[i].is_empty || table[i].is_deleted) {
            cout << "- ";  // Print empty or deleted slots as "-"
        } else {
            cout << table[i].key << " ";  // Print valid keys
        }
    }
    cout << endl;
}
