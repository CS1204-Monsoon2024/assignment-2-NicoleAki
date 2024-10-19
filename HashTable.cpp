#include <iostream>
#include <cmath>

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

    Entry* table;      // Array for storing hash entries
    int size;          // Current size of the table
    int num_elements;  // Current number of elements
    float load_factor = 0.8;

    int hash(int key);
    int quadratic_probe(int key, int i);
    bool is_prime(int n);
    int next_prime(int n);
    void resize();   // Function to resize and rehash the table
    int getMaxProbingLimit();  // Function to calculate the probing limit
};

HashTable::HashTable(int initial_size) {
    size = next_prime(initial_size);  
    table = new Entry[size];
    num_elements = 0;

    // Initialize all table entries
    for (int i = 0; i < size; ++i) {
        table[i].key = -1;  // -1 indicates empty
        table[i].is_empty = true;
        table[i].is_deleted = false;
    }
}

HashTable::~HashTable() {
    delete[] table;
}

int HashTable::hash(int key) {
    return key % size;  
}

int HashTable::quadratic_probe(int key, int i) {
    return (hash(key) + i * i) % size;  
}

bool HashTable::is_prime(int n) {
    if (n < 2) return false;
    for (int i = 2; i <= sqrt(n); ++i) {
        if (n % i == 0) return false;
    }
    return true;
}

int HashTable::next_prime(int n) {
    while (!is_prime(n)) {
        ++n;
    }
    return n;
}

int HashTable::getMaxProbingLimit() {
    // Calculate the max probing limit: (size + 1) / 2
    return (size + 1) / 2;
}

void HashTable::resize() {
    int new_size = next_prime(size * 2);  // Resize to the next prime number
    Entry* old_table = table;  // Store the old table
    int old_size = size;

    // Create a new table with the new size
    size = new_size;
    table = new Entry[size];

    // Initialize new table entries
    for (int i = 0; i < size; ++i) {
        table[i].key = -1;
        table[i].is_empty = true;
        table[i].is_deleted = false;
    }

    // Reset element count and rehash old elements into the new table
    int old_num_elements = num_elements;  
    num_elements = 0;

    for (int i = 0; i < old_size; ++i) {
        if (!old_table[i].is_empty && !old_table[i].is_deleted) {
            insert(old_table[i].key);  // Rehash old keys
        }
    }

    delete[] old_table;  // Free the memory of the old table
}

void HashTable::insert(int key) {
    // Check for duplicate keys
    if (search(key) != -1) {
        std::cout << "Duplicate key insertion is not allowed" << std::endl;
        return;
    }

    // Resize table if load factor exceeds threshold
    if ((float)num_elements / size >= load_factor) {
        resize();
    }

    int i = 0;
    int max_limit = getMaxProbingLimit();  // Get the new max probing limit
    while (i < max_limit) {
        int index = quadratic_probe(key, i);
        if (table[index].is_empty || table[index].is_deleted) {
            table[index].key = key;
            table[index].is_empty = false;
            table[index].is_deleted = false;
            ++num_elements;
            return;
        }
        ++i;
    }

    std::cout << "Max probing limit reached!" << std::endl;
}

int HashTable::search(int key) {
    int i = 0;
    int max_limit = getMaxProbingLimit();  // Get the new max probing limit
    while (i < max_limit) {
        int index = quadratic_probe(key, i);
        if (table[index].is_empty) {
            return -1;  // Key not found
        }
        if (table[index].key == key && !table[index].is_deleted) {
            return index;  // Key found
        }
        ++i;
    }
    return -1;  // Key not found after max probing
}

void HashTable::remove(int key) {
    int index = search(key);
    if (index == -1) {
        std::cout << "Element not found" << std::endl;
        return;
    }

    table[index].is_deleted = true;  // Mark the slot as deleted
    --num_elements;
}

void HashTable::printTable() {
    for (int i = 0; i < size; ++i) {
        if (table[i].is_empty || table[i].is_deleted) {
            std::cout << "- ";
        } else {
            std::cout << table[i].key << " ";
        }
    }
    std::cout << std::endl;
}
