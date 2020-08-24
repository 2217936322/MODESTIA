#include "VMatrix.hpp"

inline VMatrix::VMatrix()
{
}

inline VMatrix::VMatrix(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
{
    Init(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33);
}

inline VMatrix::VMatrix(const Matrix3x4_t& matrix3x4)
{
    Init(matrix3x4);
}

inline VMatrix::VMatrix(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis)
{
    Init(xAxis.x, yAxis.x, zAxis.x, 0.0f, xAxis.y, yAxis.y, zAxis.y, 0.0f, xAxis.z, yAxis.z, zAxis.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}


inline void VMatrix::Init(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
{
    m[0][0] = m00;
    m[0][1] = m01;
    m[0][2] = m02;
    m[0][3] = m03;

    m[1][0] = m10;
    m[1][1] = m11;
    m[1][2] = m12;
    m[1][3] = m13;

    m[2][0] = m20;
    m[2][1] = m21;
    m[2][2] = m22;
    m[2][3] = m23;

    m[3][0] = m30;
    m[3][1] = m31;
    m[3][2] = m32;
    m[3][3] = m33;
}

inline void VMatrix::Init(const Matrix3x4_t& matrix3x4)
{
    memcpy(m, matrix3x4.Base(), sizeof(Matrix3x4_t));

    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = 0.0f;
    m[3][3] = 1.0f;
}

inline void Vector3DMultiplyPosition(const VMatrix& src1, const Vector& src2, Vector& dst)
{
    dst[0] = src1[0][0] * src2.x + src1[0][1] * src2.y + src1[0][2] * src2.z + src1[0][3];
    dst[1] = src1[1][0] * src2.x + src1[1][1] * src2.y + src1[1][2] * src2.z + src1[1][3];
    dst[2] = src1[2][0] * src2.x + src1[2][1] * src2.y + src1[2][2] * src2.z + src1[2][3];
}

inline Vector VMatrix::GetForward() const
{
    return Vector(m[0][0], m[1][0], m[2][0]);
}

inline Vector VMatrix::GetLeft() const
{
    return Vector(m[0][1], m[1][1], m[2][1]);
}

inline Vector VMatrix::GetUp() const
{
    return Vector(m[0][2], m[1][2], m[2][2]);
}

inline void VMatrix::SetForward(const Vector &forward)
{
    m[0][0] = forward.x;
    m[1][0] = forward.y;
    m[2][0] = forward.z;
}

inline void VMatrix::SetLeft(const Vector &left)
{
    m[0][1] = left.x;
    m[1][1] = left.y;
    m[2][1] = left.z;
}

inline void VMatrix::SetUp(const Vector &up)
{
    m[0][2] = up.x;
    m[1][2] = up.y;
    m[2][2] = up.z;
}

inline void VMatrix::GetBasisVectors(Vector &forward, Vector &left, Vector &up) const
{
    forward.Init(m[0][0], m[1][0], m[2][0]);
    left.Init(m[0][1], m[1][1], m[2][1]);
    up.Init(m[0][2], m[1][2], m[2][2]);
}

inline void VMatrix::SetBasisVectors(const Vector &forward, const Vector &left, const Vector &up)
{
    SetForward(forward);
    SetLeft(left);
    SetUp(up);
}

inline Vector VMatrix::GetTranslation() const
{
    return Vector(m[0][3], m[1][3], m[2][3]);
}

inline Vector& VMatrix::GetTranslation(Vector &trans) const
{
    trans.x = m[0][3];
    trans.y = m[1][3];
    trans.z = m[2][3];
    return trans;
}

inline void VMatrix::SetTranslation(const Vector &trans)
{
    m[0][3] = trans.x;
    m[1][3] = trans.y;
    m[2][3] = trans.z;
}

inline void VMatrix::PreTranslate(const Vector &trans)
{
    Vector tmp;
    Vector3DMultiplyPosition(*this, trans, tmp);
    m[0][3] = tmp.x;
    m[1][3] = tmp.y;
    m[2][3] = tmp.z;
}

inline void VMatrix::PostTranslate(const Vector &trans)
{
    m[0][3] += trans.x;
    m[1][3] += trans.y;
    m[2][3] += trans.z;
}

inline const Matrix3x4_t& VMatrix::As3x4() const
{
    return *((const Matrix3x4_t*)this);
}

inline Matrix3x4_t& VMatrix::As3x4()
{
    return *((Matrix3x4_t*)this);
}

inline void VMatrix::CopyFrom3x4(const Matrix3x4_t &m3x4)
{
    memcpy(m, m3x4.Base(), sizeof(Matrix3x4_t));
    m[3][0] = m[3][1] = m[3][2] = 0;
    m[3][3] = 1;
}

inline void VMatrix::Set3x4(Matrix3x4_t& matrix3x4) const
{
    memcpy(matrix3x4.Base(), m, sizeof(Matrix3x4_t));
}

inline const VMatrix& VMatrix::operator+=(const VMatrix &other)
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            m[i][j] += other.m[i][j];
        }
    }

    return *this;
}

inline VMatrix VMatrix::operator+(const VMatrix &other) const
{
    VMatrix ret;
    for(int i = 0; i < 16; i++) 
    {
        ((float*)ret.m)[i] = ((float*)m)[i] + ((float*)other.m)[i];
    }
    return ret;
}

inline VMatrix VMatrix::operator-(const VMatrix &other) const
{
    VMatrix ret;

    for(int i = 0; i < 4; i++) 
    {
        for(int j = 0; j < 4; j++)
        {
            ret.m[i][j] = m[i][j] - other.m[i][j];
        }
    }

    return ret;
}

inline VMatrix VMatrix::operator-() const
{
    VMatrix ret;
    for(int i = 0; i < 16; i++) 
    {
        ((float*)ret.m)[i] = -((float*)m)[i];
    }
    return ret;
}

inline Vector VMatrix::operator*(const Vector &vec) const
{
    Vector vRet;
    vRet.x = m[0][0] * vec.x + m[0][1] * vec.y + m[0][2] * vec.z + m[0][3];
    vRet.y = m[1][0] * vec.x + m[1][1] * vec.y + m[1][2] * vec.z + m[1][3];
    vRet.z = m[2][0] * vec.x + m[2][1] * vec.y + m[2][2] * vec.z + m[2][3];

    return vRet;
}

inline Vector VMatrix::VMul4x3(const Vector &vec) const
{
    Vector vResult;
    Vector3DMultiplyPosition(*this, vec, vResult);
    return vResult;
}


inline Vector VMatrix::VMul4x3Transpose(const Vector &vec) const
{
    Vector tmp = vec;
    tmp.x -= m[0][3];
    tmp.y -= m[1][3];
    tmp.z -= m[2][3];

    return Vector(
        m[0][0] * tmp.x + m[1][0] * tmp.y + m[2][0] * tmp.z,
        m[0][1] * tmp.x + m[1][1] * tmp.y + m[2][1] * tmp.z,
        m[0][2] * tmp.x + m[1][2] * tmp.y + m[2][2] * tmp.z
    );
}

inline Vector VMatrix::VMul3x3(const Vector &vec) const
{
    return Vector(
        m[0][0] * vec.x + m[0][1] * vec.y + m[0][2] * vec.z,
        m[1][0] * vec.x + m[1][1] * vec.y + m[1][2] * vec.z,
        m[2][0] * vec.x + m[2][1] * vec.y + m[2][2] * vec.z
    );
}

inline Vector VMatrix::VMul3x3Transpose(const Vector &vec) const
{
    return Vector(
        m[0][0] * vec.x + m[1][0] * vec.y + m[2][0] * vec.z,
        m[0][1] * vec.x + m[1][1] * vec.y + m[2][1] * vec.z,
        m[0][2] * vec.x + m[1][2] * vec.y + m[2][2] * vec.z
    );
}


inline void VMatrix::V3Mul(const Vector &in, Vector &out) const
{
    float rw;

    rw = 1.0f / (m[3][0] * in.x + m[3][1] * in.y + m[3][2] * in.z + m[3][3]);
    out.x = (m[0][0] * in.x + m[0][1] * in.y + m[0][2] * in.z + m[0][3]) * rw;
    out.y = (m[1][0] * in.x + m[1][1] * in.y + m[1][2] * in.z + m[1][3]) * rw;
    out.z = (m[2][0] * in.x + m[2][1] * in.y + m[2][2] * in.z + m[2][3]) * rw;
}

inline void VMatrix::Identity()
{
    m[0][0] = 1.0f; 
    m[0][1] = 0.0f; 
    m[0][2] = 0.0f;
    m[0][3] = 0.0f;
    m[1][0] = 0.0f; 
    m[1][1] = 1.0f; 
    m[1][2] = 0.0f; 
    m[1][3] = 0.0f;
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = 1.0f; 
    m[2][3] = 0.0f;
    m[3][0] = 0.0f;
    m[3][1] = 0.0f; 
    m[3][2] = 0.0f;
    m[3][3] = 1.0f;
}

inline bool VMatrix::IsIdentity() const
{
    return m[0][0] == 1.0f && m[0][1] == 0.0f && m[0][2] == 0.0f && m[0][3] == 0.0f && m[1][0] == 0.0f && m[1][1] == 1.0f && m[1][2] == 0.0f && m[1][3] == 0.0f && m[2][0] == 0.0f && m[2][1] == 0.0f && m[2][2] == 1.0f && m[2][3] == 0.0f && m[3][0] == 0.0f && m[3][1] == 0.0f && m[3][2] == 0.0f && m[3][3] == 1.0f;
}

inline Vector VMatrix::ApplyRotation(const Vector &vec) const
{
    return VMul3x3(vec);
}
