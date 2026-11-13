#ifndef SCALEDBUFFER_HPP
#define SCALEDBUFFER_HPP

#include "IntBuffer.hpp"

class ScaledBuffer : public IntBuffer {
public:
    ScaledBuffer();
    ScaledBuffer(int count, int initVal, int extraVal);
    ScaledBuffer(const ScaledBuffer& other);
    virtual ~ScaledBuffer();
    ScaledBuffer& operator=(const ScaledBuffer& other);

    virtual int evaluate() const override;
};

#endif
