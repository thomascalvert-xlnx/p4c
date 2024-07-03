#include "test.h"

/*
 * This file implements the following P4 extern object:
 *
 *   extern IntComparator {
 *       IntComparator(int reference, bool invert);
 *
 *       // Returns val minus reference, clamped to [-128, 127].
 *       // result is set to: (val==reference) XOR invert
 *       int doCompare(in bit<16> val, out bool result);
 *   }
 *
 * There isn't a good mapping to C99 primitives, so we use a C++ class.
 */

class IntComparator {
    public:
        IntComparator(int reference, bool invert);
        s8 doCompare(u16 val, bool* result);

    private:
        int m_ref;
        bool m_inv;
};

IntComparator::IntComparator(int reference, bool invert) :
    m_ref(reference),
    m_inv(invert)
{
}

s8 IntComparator::doCompare(u16 val, bool* result)
{
    int diff = val - m_ref;
    *result = (diff == 0) ? !m_inv : m_inv;
    s8 ret = 0;
    if (diff < -128) ret = -128;
    else if (diff > 127) ret = 127;
    else ret = (s8)diff;
    return ret;
}
