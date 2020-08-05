#pragma once
#include <limits>
#include <algorithm>

extern float bits_to_float(std::uint32_t i);
#define M_PI 3.14159265358979323846
#define M_RADPI		57.295779513082f
#define M_PI_F		((float)(M_PI))	// Shouldn't collide with anything.
#define RAD2DEG( x  )  ( (float)(x) * (float)(180.f / M_PI_F) )
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI_F / 180.f) )

#define FLOAT32_NAN_BITS     ( std::uint32_t ) 0x7FC00000	// not a number!
#define FLOAT32_NAN          bits_to_float( FLOAT32_NAN_BITS )
#define VEC_T_NAN FLOAT32_NAN

#define ASSERT( _exp ) ( (void ) 0 )

template <typename T>
T clip_number(const T & n, const T & lower, const T & upper) {
	if (n < lower) return lower;
	if (n > upper) return upper;
	return n;
}

class Vector3D {
public:
	Vector3D();
	Vector3D(float, float, float);
	~Vector3D();

	float x, y, z;

	Vector3D& operator+=(const Vector3D& v) {
		x += v.x; y += v.y; z += v.z; return *this;
	}
	Vector3D& operator-=(const Vector3D& v) {
		x -= v.x; y -= v.y; z -= v.z; return *this;
	}
	Vector3D& operator*=(float v) {
		x *= v; y *= v; z *= v; return *this;
	}
	Vector3D operator+(const Vector3D& v) {
		return Vector3D{ x + v.x, y + v.y, z + v.z };
	}
	Vector3D operator-(const Vector3D& v) {
		return Vector3D{ x - v.x, y - v.y, z - v.z };
	}
	Vector3D operator*(float fl) const {
		return Vector3D(x * fl, y * fl, z * fl);
	}
	Vector3D operator*(const Vector3D& v) const {
		return Vector3D(x * v.x, y * v.y, z * v.z);
	}
	Vector3D& operator/=(float fl) {
		x /= fl;
		y /= fl;
		z /= fl;
		return *this;
	}
	auto operator-(const Vector3D& other) const -> Vector3D {
		auto buf = *this;

		buf.x -= other.x;
		buf.y -= other.y;
		buf.z -= other.z;

		return buf;
	}
	float& operator[](int i) {
		return ((float*)this)[i];
	}
	float operator[](int i) const {
		return ((float*)this)[i];
	}

	inline float Length2D() const
	{
		return sqrt((x * x) + (y * y));
	}
	void crossproduct(Vector3D v1, Vector3D v2, Vector3D cross_p) const //ijk = xyz
	{
		cross_p.x = (v1.y * v2.z) - (v1.z * v2.y); //i
		cross_p.y = -((v1.x * v2.z) - (v1.z * v2.x)); //j
		cross_p.z = (v1.x * v2.y) - (v1.y * v2.x); //k
	}
	Vector3D Cross(const Vector3D& vOther) const
	{
		Vector3D res;
		crossproduct(*this, vOther, res);
		return res;
	}

	void init(float ix, float iy, float iz);
	void clamp();
	Vector3D normalized();
	float normalize_float();
	float distance_to(const Vector3D& other);
	void normalize();
	void normalize_aimbot();
	float length();
	float length_sqr();
	float dot(const Vector3D other);
	float dot(const float* other);
};

// has to be hear
inline Vector3D operator*(float lhs, const Vector3D& rhs) {
	return Vector3D(rhs.x * lhs, rhs.x * lhs, rhs.x * lhs);
}

struct matrix_t
{
	matrix_t() { }
	matrix_t(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23)
	{
		mat_val[0][0] = m00;	mat_val[0][1] = m01; mat_val[0][2] = m02; mat_val[0][3] = m03;
		mat_val[1][0] = m10;	mat_val[1][1] = m11; mat_val[1][2] = m12; mat_val[1][3] = m13;
		mat_val[2][0] = m20;	mat_val[2][1] = m21; mat_val[2][2] = m22; mat_val[2][3] = m23;
	}

	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	void init(const Vector3D& xAxis, const Vector3D& yAxis, const Vector3D& zAxis, const Vector3D& vecOrigin)
	{
		mat_val[0][0] = xAxis.x; mat_val[0][1] = yAxis.x; mat_val[0][2] = zAxis.x; mat_val[0][3] = vecOrigin.x;
		mat_val[1][0] = xAxis.y; mat_val[1][1] = yAxis.y; mat_val[1][2] = zAxis.y; mat_val[1][3] = vecOrigin.y;
		mat_val[2][0] = xAxis.z; mat_val[2][1] = yAxis.z; mat_val[2][2] = zAxis.z; mat_val[2][3] = vecOrigin.z;
	}

	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	matrix_t(const Vector3D& xAxis, const Vector3D& yAxis, const Vector3D& zAxis, const Vector3D& vecOrigin)
	{
		init(xAxis, yAxis, zAxis, vecOrigin);
	}

	inline void set_origin(Vector3D const& p)
	{
		mat_val[0][3] = p.x;
		mat_val[1][3] = p.y;
		mat_val[2][3] = p.z;
	}

	inline void invalidate(void)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				mat_val[i][j] = VEC_T_NAN;
			}
		}
	}

	float* operator[](int i) { ASSERT((i >= 0) && (i < 3)); return mat_val[i]; }
	const float* operator[](int i) const { ASSERT((i >= 0) && (i < 3)); return mat_val[i]; }
	float* base() { return &mat_val[0][0]; }
	const float* base() const { return &mat_val[0][0]; }

	float mat_val[3][4];
};