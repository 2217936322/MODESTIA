#include "Vector3D.hpp"
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <random>
float bits_to_float(std::uint32_t i) {
	union convertor_t
	{
		float f;
		unsigned long ul;
	}tmp;

	tmp.ul = i;
	return tmp.f;
}

Vector3D::Vector3D(void) {
	x = y = z = 0.0f;
}

Vector3D::Vector3D(float fx, float fy, float fz) {
	x = fx;
	y = fy;
	z = fz;
}

Vector3D::~Vector3D(void) {
};

void Vector3D::init(float ix, float iy, float iz) {
	x = ix; y = iy; z = iz;
}

void Vector3D::clamp(void) {
	x = std::clamp(x, -89.0f, 89.0f);
	y = std::clamp(std::remainder(y, 360.0f), -180.0f, 180.0f);
	z = std::clamp(z, -50.0f, 50.0f);
}

float Vector3D::normalize_float() {
	Vector3D res = *this;
	float l = res.length();
	if (l != 0.0f) {
		res /= l;
	}
	else {
		res.x = res.y = res.z = 0.0f;
	}
	return l;
}

float Vector3D::distance_to(const Vector3D& other) {
	Vector3D delta;
	delta.x = x - other.x;
	delta.y = y - other.y;
	delta.z = z - other.z;

	return delta.length();
}

void Vector3D::normalize(void) {
	auto vec_normalize = [&](Vector3D& v) {
		auto l = v.length();

		if (l != 0.0f) {
			v.x /= l;
			v.y /= l;
			v.z /= l;
		}
		else {
			v.x = v.y = 0.0f; v.z = 1.0f;
		}

		return l;
	};

	vec_normalize(*this);
}

void Vector3D::normalize_aimbot() {
	x = std::isfinite(x) ? std::remainderf(x, 360.0f) : 0.0f;
	y = std::isfinite(y) ? std::remainderf(y, 360.0f) : 0.0f;
	z = 0.0f;
}

Vector3D Vector3D::normalized(void) {
	Vector3D vec(*this);
	vec.normalize();

	return vec;
}

float Vector3D::length(void) {
	float root = 0.0f, sqsr = this->length_sqr();

	__asm        sqrtss xmm0, sqsr
	__asm        movss root, xmm0

	return root;
}

float Vector3D::length_sqr(void) {
	auto sqr = [](float n) {
		return static_cast<float>(n * n);
	};

	return (sqr(x) + sqr(y) + sqr(z));
}

float Vector3D::dot(const Vector3D other) {
	return (x * other.x + y * other.y + z * other.z);
}

float Vector3D::dot(const float* other) {
	const Vector3D& a = *this;

	return(a.x * other[0] + a.y * other[1] + a.z * other[2]);
}
