#include <iostream>
#include "IntBuffer.hpp"
#include "ScaledBuffer.hpp"

using namespace std;

// Lightweight heap allocation tracker using only <iostream> and C++11 primitives (no size_t, no extra headers)
static char g_heapPool[65536];
static int g_poolOffset = 0;
static int g_activeAllocs = 0;
static int g_totalAllocs = 0;

void* operator new(decltype(sizeof(0)) bytes) {
    int req = static_cast<int>(bytes);
    if (req <= 0) req = 1;
    // 8-byte alignment
    int rem = req % 8;
    if (rem != 0) req += (8 - rem);
    if (g_poolOffset + req > 65536) return nullptr;
    void* ptr = &g_heapPool[g_poolOffset];
    g_poolOffset += req;
    ++g_activeAllocs;
    ++g_totalAllocs;
    return ptr;
}

void* operator new[](decltype(sizeof(0)) bytes) {
    int req = static_cast<int>(bytes);
    if (req <= 0) req = 1;
    int rem = req % 8;
    if (rem != 0) req += (8 - rem);
    if (g_poolOffset + req > 65536) return nullptr;
    void* ptr = &g_heapPool[g_poolOffset];
    g_poolOffset += req;
    ++g_activeAllocs;
    ++g_totalAllocs;
    return ptr;
}

void operator delete(void* ptr) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

void operator delete[](void* ptr) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

void operator delete(void* ptr, decltype(sizeof(0))) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

void operator delete[](void* ptr, decltype(sizeof(0))) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

bool approxEqual(double a, double b) {
    double diff = a - b;
    if (diff < 0.0) diff = -diff;
    return diff < 0.001;
}

int main() {
    int passed = 0;
    int failed = 0;

    // Expected reference values computed dynamically
    // Array of 3 elements initialized as: initVal + 0, initVal + 1, initVal + 2
    int initV = static_cast<int>(0);
    int extV = static_cast<int>(2);

    // --- PARENT CLASS TEST CASES (1 to 5) ---
    // TC 1: Default constructor & empty compute
    {
        IntBuffer pDef;
        if (approxEqual(static_cast<double>(pDef.evaluate()), 0.0) &&
            approxEqual(static_cast<double>(pDef.getAt(0)), 0.0)) {
            cout << "[PASS] TC 01: IntBuffer default constructor" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 01: IntBuffer default constructor" << endl;
            ++failed;
        }
    }

    // TC 2: Parameterized constructor & base compute()
    {
        IntBuffer p1(3, initV, extV);
        bool ok = approxEqual(static_cast<double>(p1.getAt(0)), static_cast<double>(initV)) &&
                  approxEqual(static_cast<double>(p1.getAt(2)), static_cast<double>(initV + static_cast<int>(2)));
        // Verify compute() is non-trivial when initV > 0
        if (ok && p1.getAt(1) == static_cast<int>(initV + static_cast<int>(1))) {
            // Compare against a second identical instance
            IntBuffer pCheck(3, initV, extV);
            if (approxEqual(static_cast<double>(p1.evaluate()), static_cast<double>(pCheck.evaluate())) &&
                !approxEqual(static_cast<double>(p1.evaluate()), 0.0)) {
                cout << "[PASS] TC 02: IntBuffer parameterized constructor & evaluate()" << endl;
                ++passed;
            } else {
                cout << "[FAIL] TC 02: IntBuffer evaluate() returned unexpected value" << endl;
                ++failed;
            }
        } else {
            cout << "[FAIL] TC 02: IntBuffer element initialization mismatch" << endl;
            ++failed;
        }
    }

    // TC 3: Parent Copy Constructor (Deep Copy verification)
    {
        IntBuffer orig(3, initV, extV);
        IntBuffer copyObj(orig);
        if (approxEqual(static_cast<double>(copyObj.evaluate()), static_cast<double>(orig.evaluate())) &&
            approxEqual(static_cast<double>(copyObj.getAt(1)), static_cast<double>(orig.getAt(1))) &&
            !approxEqual(static_cast<double>(copyObj.evaluate()), 0.0)) {
            cout << "[PASS] TC 03: IntBuffer copy constructor deep copy" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 03: IntBuffer copy constructor failed" << endl;
            ++failed;
        }
    }

    // TC 4: Parent Copy Assignment Operator & Self-Assignment
    {
        IntBuffer a(3, initV, extV);
        IntBuffer b(2, initV, extV);
        b = a;
        IntBuffer* selfPtr = &b;
        b = *selfPtr; // Self-assignment check
        if (approxEqual(static_cast<double>(b.evaluate()), static_cast<double>(a.evaluate())) &&
            approxEqual(static_cast<double>(b.getAt(2)), static_cast<double>(a.getAt(2))) &&
            !approxEqual(static_cast<double>(b.evaluate()), 0.0)) {
            cout << "[PASS] TC 04: IntBuffer copy assignment & self-assignment" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 04: IntBuffer copy assignment failed" << endl;
            ++failed;
        }
    }

    // TC 5: Parent Destructor Heap Release Check
    {
        int before = g_activeAllocs;
        {
            IntBuffer temp(4, initV, extV);
        }
        int after = g_activeAllocs;
        if (before == after && g_totalAllocs > 0) {
            cout << "[PASS] TC 05: IntBuffer destructor released heap memory" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 05: IntBuffer destructor leaked heap memory" << endl;
            ++failed;
        }
    }

    // --- CHILD CLASS TEST CASES (6 to 10) ---
    // TC 6: Child Parameterized Construction & Overridden compute()
    {
        IntBuffer baseObj(3, initV, extV);
        ScaledBuffer childObj(3, initV, extV);
        if (!approxEqual(static_cast<double>(childObj.evaluate()), 0.0) &&
            !approxEqual(static_cast<double>(childObj.evaluate()), static_cast<double>(baseObj.evaluate()))) {
            cout << "[PASS] TC 06: ScaledBuffer overridden evaluate()" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 06: ScaledBuffer overridden evaluate() failed" << endl;
            ++failed;
        }
    }

    // TC 7: Polymorphic Dispatch via Base Pointer
    {
        ScaledBuffer directChild(3, initV, extV);
        IntBuffer* polyPtr = &directChild;
        if (!approxEqual(static_cast<double>(polyPtr->evaluate()), 0.0) &&
            approxEqual(static_cast<double>(polyPtr->evaluate()), static_cast<double>(directChild.evaluate()))) {
            cout << "[PASS] TC 07: ScaledBuffer polymorphic dispatch via IntBuffer*" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 07: ScaledBuffer polymorphic dispatch failed" << endl;
            ++failed;
        }
    }

    // TC 8: Child Copy Constructor
    {
        ScaledBuffer c1(3, initV, extV);
        ScaledBuffer c2(c1);
        if (approxEqual(static_cast<double>(c1.evaluate()), static_cast<double>(c2.evaluate())) &&
            !approxEqual(static_cast<double>(c2.evaluate()), 0.0)) {
            cout << "[PASS] TC 08: ScaledBuffer copy constructor" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 08: ScaledBuffer copy constructor failed" << endl;
            ++failed;
        }
    }

    // TC 9: Child Copy Assignment Operator
    {
        ScaledBuffer c1(3, initV, extV);
        ScaledBuffer c2;
        c2 = c1;
        if (approxEqual(static_cast<double>(c1.evaluate()), static_cast<double>(c2.evaluate())) &&
            !approxEqual(static_cast<double>(c2.evaluate()), 0.0)) {
            cout << "[PASS] TC 09: ScaledBuffer copy assignment operator" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 09: ScaledBuffer copy assignment operator failed" << endl;
            ++failed;
        }
    }

    // TC 10: Virtual Destructor via Base Pointer Deletion
    {
        int before = g_activeAllocs;
        IntBuffer* heapChild = new ScaledBuffer(4, initV, extV);
        delete heapChild;
        int after = g_activeAllocs;
        if (before == after) {
            cout << "[PASS] TC 10: ScaledBuffer virtual destructor via IntBuffer*" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 10: ScaledBuffer virtual destructor leaked memory" << endl;
            ++failed;
        }
    }

    cout << "========================================" << endl;
    cout << "FINAL SCORE: " << passed << " / 10 PASSED (" << failed << " FAILED)" << endl;
    if (g_activeAllocs == 0) {
        cout << "MEMORY LEAK REPORT: NO MEMORY LEAKS DETECTED (0 active allocations)." << endl;
    } else {
        cout << "MEMORY LEAK REPORT: WARNING! " << g_activeAllocs << " active heap allocation(s) leaked!" << endl;
    }
    cout << "========================================" << endl;

    return (failed == 0 && g_activeAllocs == 0) ? 0 : 1;
}
