// Copyright 2020 Your Name <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#include <algorithm>
#include <cmath>
#include <exception>
#include <utility>

template <class T, unsigned BucketSize>

class unrolled_linked_list {
  class Node {
   public:
    T* arr;
    Node* next;
    unsigned num_elements;

    Node() {
      arr = new T[BucketSize + 1];
      num_elements = 0;
    }
    ~Node() {
      delete[] arr;
      num_elements = 0;
      delete next;
    }
  };

  Node* head;
  Node* tail;
  unsigned ull_size = 0;
  unsigned n_node = 0;

 public:
  class Iterator {
    Node* ptr;
    unsigned index;

   public:
    Iterator(){
      ptr = nullptr;
      index = 0;
    }
    ~Iterator() = default;
    explicit Iterator(Node* it, unsigned i) : ptr(it), index(i) {}
    Iterator(const Iterator& rhs) : ptr(rhs.ptr), index(rhs.index) {}
    T& operator*() { return ptr->arr[index]; }
    T* operator->() const { return &this; }
    Iterator& operator++() {
      index++;
      if (index % BucketSize == 0) {
        ptr = ptr->next;
        index = 0;
      }
      return *this;
    }

    void advance(unsigned n) {
      unsigned i = n;
      while (i) {
        this->operator++();
        i--;
      }
    }

    const Iterator operator++(int) {
      Iterator temp = *this;
      ++(*this);
      return temp;
    }

    Iterator& operator=(const Iterator& it) {
      if (this != it) {
        this->ptr = it.ptr;
        this->index = it.index;
      }
      return *this;
    }

    bool operator==(const Iterator& rhs) {
      return this->ptr == rhs.ptr && this->index == rhs.index;
    }

    bool operator!=(const Iterator& rhs) {
      return this->ptr != rhs.ptr || this->index != rhs.index;
    }
  };

  unrolled_linked_list() {
    head = nullptr;
    tail = head;
    ull_size = 0;
    n_node = 0;
  }
  ~unrolled_linked_list() = default;

  unrolled_linked_list(const unrolled_linked_list& a) {
    if (!a.empty()) {
      n_node = a.n_node;
      ull_size = a.ull_size;
      Node* current = new Node;
      head = current;
      Node* a_current = a.head;
      for (unsigned i = 0; i < n_node; i++) {
        current->num_elements = a_current->num_elements;
        for (unsigned j = 0; j < BucketSize; j++) {
          current->arr[j] = a_current->arr[j];
        }
        if (i < n_node - 1) {
          current->next = new Node;
          current = current->next;
          a_current = a_current->next;
        }
      }
      tail = current;
    }
  }

  Iterator begin() const { return Iterator(head, 0); }

  Iterator end() const { return Iterator(tail, tail->num_elements); }

  [[nodiscard]] bool empty() const { return ull_size == 0; }

  [[nodiscard]] unsigned size() const { return ull_size; }

  [[nodiscard]] unsigned getNode() const { return n_node; }

  T& operator[](const unsigned index) {
    unsigned index_in_node = index;
    Node* current = head;
    while (index_in_node >= current->num_elements) {
      index_in_node -= current->num_elements;
      current = current->next;
    }
    return current->arr[index_in_node];
  }

  unrolled_linked_list& operator=(const unrolled_linked_list& a) {
    if (this != a) {
      this->clear();
      if (!a.empty()) {
        n_node = a.n_node;
        ull_size = a.ull_size;
        Node* current = new Node;
        head = current;
        Node* a_current = a.head;
        for (unsigned i = 0; i < n_node; i++) {
          current->num_elements = a_current->num_elements;
          for (unsigned j = 0; j < BucketSize; j++) {
            current->arr[j] = a_current->arr[j];
          }
          if (i < n_node - 1) {
            current->next = new Node;
            current = current->next;
            a_current = a_current->next;
          }
        }
        tail = current;
      }
    }
    return *this;
  }

  bool operator==(unrolled_linked_list& a) {
    if (ull_size == a.ull_size && n_node == a.n_node) {
      Node* current = head;
      Node* a_current = a.head;
      for (unsigned k = 0; k < n_node; k++) {
        for (unsigned i = 0; i < BucketSize; i++) {
          if (current->arr[i] != a_current->arr[i]) return false;
        }
        current = current->next;
        a_current = a_current->next;
      }
      return true;
    }
    return false;
  }

  bool operator!=(unrolled_linked_list& a) {
    if (this == a) return false;
    return true;
  }

  void push_back(T value) {
    if (head == nullptr) {
      head = new Node;
      head->arr[0] = value;
      head->num_elements++;
      tail = head;
      n_node++;
    } else if (tail->num_elements < BucketSize) {
      tail->arr[tail->num_elements] = value;
      tail->num_elements++;
    } else {
      Node* new_node = new Node;
      unsigned j = 0;
      for (unsigned i = tail->num_elements / 2 + 1; i < tail->num_elements;
           i++) {
        new_node->arr[j++] = tail->arr[i];
        tail->arr[i] = 0;
      }
      new_node->arr[j++] = value;
      new_node->num_elements = j;
      tail->num_elements = tail->num_elements / 2 + 1;
      tail->next = new_node;
      tail = new_node;
      n_node++;
    }
    ull_size++;
  }

  void pop_back() {
    if (!this->empty()) {
      if (tail->num_elements > BucketSize / 2 + 1 || head == tail) {
        tail->arr[tail->num_elements - 1] = 0;
        tail->num_elements--;
      } else {
        Node* current = head;
        while (current->next != tail) {
          current = current->next;
        }
        unsigned i = 0;
        if (current->num_elements > BucketSize / 2 + 1) {
          for (; i < tail->num_elements - 1; i++) {
            tail->arr[BucketSize / 2 - i] = tail->arr[BucketSize / 2 - 1 - i];
          }
          tail->arr[0] = current->arr[current->num_elements - 1];
          current->arr[current->num_elements - 1] = 0;
          current->num_elements--;
        } else {
          for (; i < tail->num_elements - 1; i++) {
            current->arr[BucketSize / 2 + 1 + i] = tail->arr[i];
          }
          delete tail;
          current->next = nullptr;
          tail = current;
          tail->num_elements += i;
          n_node--;
        }
      }
      ull_size--;
    }
    if (ull_size == 0) {
      this->clear();
    }
  }

  T& at(const unsigned i) {
    if (i < ull_size) return this->operator[](i);
    throw std::out_of_range("Out of range");
  }

  T& front() {
    if (head) return head->arr[0];
    throw std::out_of_range("Out of range");
  }

  T& back() {
    if (head) return tail->arr[tail->num_elements - 1];
    throw std::out_of_range("Out of range");
  }

  void clear() {
    n_node = 0;
    ull_size = 0;
    Node* current = head;
    head = nullptr;
    tail = head;
    while (current) {
      Node* next = current->next;
      delete current;
      current = next;
    }
  }

  void swap(unrolled_linked_list& a) {
    std::swap(tail, a.tail);
    std::swap(head, a.head);
    std::swap(n_node, a.n_node);
    std::swap(ull_size, a.ull_size);
  }

  void insert(Iterator it, T value) {
    unsigned i = 0;
    for (Iterator ptr = this->begin(); ptr != it; ptr++) i++;
    unsigned index_in_node = i;
    Node* current = head;
    while (index_in_node >= current->num_elements) {
      index_in_node -= current->num_elements;
      current = current->next;
    }
    if (current->num_elements < BucketSize) {
      for (unsigned j = BucketSize - 1; j > index_in_node; j--) {
        current->arr[j] = current->arr[j - 1];
      }
      current->num_elements++;
      ull_size++;
    } else {
      this->push_back(0);
      for (unsigned j = 1; j < ull_size - i; j++) {
        this->operator[](ull_size - j) = this->operator[](ull_size - j - 1);
      }
    }
    this->operator[](i) = value;
  }

  void erase(Iterator it) {
    unsigned i = 0;
    for (Iterator ptr = this->begin(); ptr != it; ptr++) i++;
    for (; i < ull_size - 1; i++) {
      this->operator[](i) = this->operator[](i + 1);
    }
    this->pop_back();
  }
};

#endif  // INCLUDE_HEADER_HPP_
