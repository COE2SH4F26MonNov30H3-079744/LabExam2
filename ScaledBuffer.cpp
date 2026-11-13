#include "ScaledBuffer.hpp"

ScaledBuffer::ScaledBuffer() : IntBuffer() {}

ScaledBuffer::ScaledBuffer(int count, int initVal, int extraVal) : IntBuffer(count, initVal, extraVal) {}

ScaledBuffer::ScaledBuffer(const ScaledBuffer& other) : IntBuffer(other) {}

ScaledBuffer::~ScaledBuffer() {}

ScaledBuffer& ScaledBuffer::operator=(const ScaledBuffer& other) {
    if (this != &other) {
        IntBuffer::operator=(other);
    }
    return *this;
}

// TODO 5: Override evaluate() (Maximum 8 lines of implementation)
int ScaledBuffer::evaluate() const {
    return static_cast<int>(0);
}
