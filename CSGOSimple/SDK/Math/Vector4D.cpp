#include "Vector4D.hpp"

#include <cmath>
#include <limits>

void VectorCopy(const Vector4D& src, Vector4D& dst)
{
    dst.x = src.x;
    dst.y = src.y;
    dst.z = src.z;
    dst.w = src.w;
}

void VectorLerp(const Vector4D& src1, const Vector4D& src2, vec_t t, Vector4D& dest)
{
    dest.x = src1.x + (src2.x - src1.x) * t;
    dest.y = src1.y + (src2.y - src1.y) * t;
    dest.z = src1.z + (src2.z - src1.z) * t;
    dest.w = src1.w + (src2.w - src1.w) * t;
}

float VectorLength(const Vector4D& v)
{
    return sqrt(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w);
}

vec_t NormalizeVector(Vector4D& v)
{
    vec_t l = v.Length();
    if(l != 0.0f) {
        v /= l;
    } else {
        v.x = v.y = v.z = v.w = 0.0f;
    }
    return l;
}

Vector4D::Vector4D(void)
{
    Invalidate();
}
Vector4D::Vector4D(vec_t X, vec_t Y, vec_t Z, vec_t W)
{
    x = X;
    y = Y;
    z = Z;
    w = W;
}
Vector4D::Vector4D(vec_t* color)
{
    x = color[0];
    y = color[1];
    z = color[2];
    w = color[3];
}

void Vector4D::Init(vec_t X, vec_t Y, vec_t Z, vec_t W)
{
    x = X; y = Y; z = Z; w = W;
}

void Vector4D::Random(vec_t minVal, vec_t maxVal)
{
    x = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
    y = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
    z = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
    w = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
}

void Vector4D::Zero()
{
    x = y = z = w = 0.0f;
}

Vector4D& Vector4D::operator=(const Vector4D &other)
{
    x = other.x; y = other.y; z = other.z; w = other.w;
    return *this;
}

vec_t& Vector4D::operator[](int i)
{
    return ((vec_t*)this)[i];
}

vec_t Vector4D::operator[](int i) const
{
    return ((vec_t*)this)[i];
}


vec_t* Vector4D::Base()
{
    return (vec_t*)this;
}

vec_t const* Vector4D::Base() const
{
    return (vec_t const*)this;
}

bool Vector4D::IsValid() const
{
    return !isinf(x) && !isinf(y) && !isinf(z) && !isinf(w);
}

void Vector4D::Invalidate()
{
    x = y = z = w = std::numeric_limits<float>::infinity();
}

bool Vector4D::operator==(const Vector4D& src) const
{
    return (src.x == x) && (src.y == y) && (src.z == z) && (src.w == w);
}

bool Vector4D::operator!=(const Vector4D& src) const
{
    return (src.x != x) || (src.y != y) || (src.z != z) || (src.w != w);
}

void Vector4D::CopyToArray(float* rgfl) const
{
    rgfl[0] = x, rgfl[1] = y, rgfl[2] = z; rgfl[3] = w;
}

void Vector4D::Negate()
{
    x = -x; y = -y; z = -z; w = -w;
}

Vector4D Vector4D::ProjectOnto(const Vector4D& onto)
{
    return onto * (this->Dot(onto) / (onto.LengthSqr()));
}

void Vector4D::MulAdd(const Vector4D& a, const Vector4D& b, float scalar)
{
    x = a.x + b.x * scalar;
    y = a.y + b.y * scalar;
    z = a.z + b.z * scalar;
    w = a.w + b.w * scalar;
}

Vector4D VectorLerp(const Vector4D& src1, const Vector4D& src2, vec_t t)
{
    Vector4D result;
    VectorLerp(src1, src2, t, result);
    return result;
}

vec_t Vector4D::Dot(const Vector4D& b) const
{
    return (x*b.x + y*b.y + z*b.z + w*b.w);
}
void VectorClear(Vector4D& a)
{
    a.x = a.y = a.z = a.w = 0.0f;
}

vec_t Vector4D::Length(void) const
{
    return sqrt(x*x + y*y + z*z + w*w);
}

bool Vector4D::WithinAABox(Vector4D const &boxmin, Vector4D const &boxmax)
{
    return (
        (x >= boxmin.x) && (x <= boxmax.x) &&
        (y >= boxmin.y) && (y <= boxmax.y) &&
        (z >= boxmin.z) && (z <= boxmax.z) &&
        (w >= boxmin.w) && (w <= boxmax.w)
        );
}

vec_t Vector4D::DistTo(const Vector4D &other) const
{
    Vector4D delta;
    delta = *this - other;
    return delta.Length();
}

Vector4D Vector4D::Min(const Vector4D &other) const
{
    return Vector4D(x < other.x ? x : other.x,
        y < other.y ? y : other.y,
        z < other.z ? z : other.z,
        w < other.w ? w : other.w);
}

Vector4D Vector4D::Max(const Vector4D &other) const
{
    return Vector4D(x > other.x ? x : other.x,
        y > other.y ? y : other.y,
        z > other.z ? z : other.z,
        w > other.w ? w : other.w);
}

Vector4D Vector4D::operator-(void) const
{
    return Vector4D(-x, -y, -z, -w);
}

Vector4D Vector4D::operator+(const Vector4D& v) const
{
    return Vector4D(x + v.x, y + v.y, z + v.z, w + v.w);
}

Vector4D Vector4D::operator-(const Vector4D& v) const
{
    return Vector4D(x - v.x, y - v.y, z - v.z, w - v.w);
}

Vector4D Vector4D::operator*(float fl) const
{
    return Vector4D(x * fl, y * fl, z * fl, w * fl);
}

Vector4D Vector4D::operator*(const Vector4D& v) const
{
    return Vector4D(x * v.x, y * v.y, z * v.z, w * v.w);
}

Vector4D Vector4D::operator/(float fl) const
{
    return Vector4D(x / fl, y / fl, z / fl, w / fl);
}

Vector4D Vector4D::operator/(const Vector4D& v) const
{
    return Vector4D(x / v.x, y / v.y, z / v.z, w / v.w);
}

Vector4D operator*(float fl, const Vector4D& v)
{
    return v * fl;
}