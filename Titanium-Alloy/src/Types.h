#pragma once

class Vec4f
{
public:
    float x, y, z, w;
    Vec4f() : x(0), y(0), z(0), w(0) {};
    Vec4f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};
};