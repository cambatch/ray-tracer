#include <cassert>

#include "Vector3.hpp"
#include "Matrix.hpp"

using Math::Vector3;
using Math::Mat2;
using Math::Mat3;
using Math::Mat4;

int main() {
    Vector3 a;
    Vector3 b;

    assert(a == b);

    a = Vector3(1, 2, 3);
    assert(a + b == Vector3(1, 2, 3));

    b = Vector3(1, 2, 3);
    assert(a + b == Vector3(2, 4, 6));

    assert(a - b == Vector3());

    assert(a * 2 == Vector3(2, 4, 6));
}
