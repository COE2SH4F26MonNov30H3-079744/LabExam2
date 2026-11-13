#ifndef INTBUFFER_HPP
#define INTBUFFER_HPP

#include <iostream>
using namespace std;

class IntBuffer {
protected:
    int* data;
    int capacity;
    int scaleFactor;

public:
    IntBuffer();
    IntBuffer(int count, int initVal, int extraVal);
    IntBuffer(const IntBuffer& other);
    virtual ~IntBuffer();
    IntBuffer& operator=(const IntBuffer& other);

    int getAt(int index) const;
    virtual int evaluate() const;
};

#endif
