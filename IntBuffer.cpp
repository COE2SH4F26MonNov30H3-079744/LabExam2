#include "IntBuffer.hpp"

IntBuffer::IntBuffer() : data(nullptr), capacity(0), scaleFactor(2) {}

IntBuffer::IntBuffer(int count, int initVal, int extraVal) : data(nullptr), capacity(count), scaleFactor(extraVal) {
    if (capacity > 0) {
        data = new int[capacity];
        for (int i = 0; i < capacity; ++i) {
            data[i] = initVal + static_cast<int>(i);
        }
    } else {
        capacity = 0;
    }
}

// TODO 1: Complete the Copy Constructor (Deep Copy)
IntBuffer::IntBuffer(const IntBuffer& other) {
    (void)other;
}

// TODO 2: Complete the Destructor (Release DMA)
IntBuffer::~IntBuffer() {
}

// TODO 3: Complete the Copy Assignment Operator (Deep Copy + Self-Assignment Guard)
IntBuffer& IntBuffer::operator=(const IntBuffer& other) {
    (void)other;
    return *this;
}

int IntBuffer::getAt(int index) const {
    if (index >= 0 && index < capacity && data != nullptr) {
        return data[index];
    }
    return static_cast<int>(0);
}

// TODO 4: Implement evaluate() (Maximum 8 lines of implementation)
int IntBuffer::evaluate() const {
    return static_cast<int>(0);
}
