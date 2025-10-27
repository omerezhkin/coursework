#pragma once

#include <cstddef>
#include <stdexcept>
#include <new>
#include <utility>

template <typename T>
class MyVector {
public:
    MyVector();

    ~MyVector();

    MyVector(const MyVector& other);

    MyVector& operator=(const MyVector& other);

    void push_back(const T& value);

    void pop_back();

    void clear();

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    T& at(size_t index);
    const T& at(size_t index) const;


    size_t size() const;
    size_t capacity() const;
    bool empty() const;

private:
    void reallocate(size_t new_capacity);

private:
    T* m_data;
    size_t m_size;
    size_t m_capacity;
};

template <typename T>
MyVector<T>::MyVector()
    : m_data(nullptr), m_size(0), m_capacity(0) {
}

template <typename T>
MyVector<T>::~MyVector() {
    clear();
    ::operator delete(m_data);
}

template <typename T>
MyVector<T>::MyVector(const MyVector<T>& other)
    : m_data(nullptr), m_size(0), m_capacity(0) {

    if (other.m_capacity > 0) {
        m_data = static_cast<T*>(::operator new(other.m_capacity * sizeof(T)));
        m_capacity = other.m_capacity;
    }

    m_size = other.m_size;
    for (size_t i = 0; i < m_size; ++i) {
        new (m_data + i) T(other.m_data[i]);
    }
}

template <typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector<T>& other) {
    if (this == &other) {
        return *this;
    }

    MyVector<T> temp_copy(other);

    std::swap(m_data, temp_copy.m_data);
    std::swap(m_size, temp_copy.m_size);
    std::swap(m_capacity, temp_copy.m_capacity);

    return *this;
}


template <typename T>
void MyVector<T>::push_back(const T& value) {
    if (m_size >= m_capacity) {
        size_t new_cap = (m_capacity == 0) ? 1 : m_capacity * 2;
        reallocate(new_cap);
    }

    new (m_data + m_size) T(value);

    m_size++;
}

template <typename T>
void MyVector<T>::pop_back() {
    if (m_size > 0) {
        m_size--;
        m_data[m_size].~T();
    }
}

template <typename T>
void MyVector<T>::clear() {
    while (m_size > 0) {
        pop_back();
    }
}

template <typename T>
T& MyVector<T>::operator[](size_t index) {
    return m_data[index];
}

template <typename T>
const T& MyVector<T>::operator[](size_t index) const {
    return m_data[index];
}

template <typename T>
T& MyVector<T>::at(size_t index) {
    if (index >= m_size) {
        throw std::out_of_range("MyVector::at() index out of range");
    }
    return m_data[index];
}

template <typename T>
const T& MyVector<T>::at(size_t index) const {
    if (index >= m_size) {
        throw std::out_of_range("MyVector::at() index out of range");
    }
    return m_data[index];
}

template <typename T>
size_t MyVector<T>::size() const {
    return m_size;
}

template <typename T>
size_t MyVector<T>::capacity() const {
    return m_capacity;
}

template <typename T>
bool MyVector<T>::empty() const {
    return m_size == 0;
}


template <typename T>
void MyVector<T>::reallocate(size_t new_capacity) {
    T* new_data = static_cast<T*>(::operator new(new_capacity * sizeof(T)));

    for (size_t i = 0; i < m_size; ++i) {
        new (new_data + i) T(std::move(m_data[i]));

        m_data[i].~T();
    }

    ::operator delete(m_data);

    m_data = new_data;
    m_capacity = new_capacity;

}
