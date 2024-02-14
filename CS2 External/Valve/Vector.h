#pragma once
#include <cmath>
#include "SDK/sdk.h"
#include "SDK/utils.h"
class Vector_t {
public:
	float x, y, z;
	Vector_t(float x = 0.0f, float y = 0.0f, float z = 0.0f) :x(x), y(y), z(z) { }

	Vector_t operator + (const Vector_t& other) {
		return{ x + other.x,y + other.y,z + other.z };
	}

	Vector_t operator - (const Vector_t& other) {
		return{ x - other.x,y - other.y,z - other.z };
	}

	bool operator < (const Vector_t& other) {
		return (x < other.x && y < other.y && z < other.x);
	}

	bool operator > (const Vector_t& other) {
		return (x > other.x && y > other.y && z > other.x);
	}

	bool operator == (const Vector_t& other) {
		return (x == other.x && y == other.y && z == other.x);
	}

	const Vector_t W2S() const {
		CMatrix4x4 vm = interfaces::dwViewMatrix;
		float _x = vm[0][0] * x + vm[0][1] * y + vm[0][2] * z + vm[0][3];
		float _y = vm[1][0] * x + vm[1][1] * y + vm[1][2] * z + vm[1][3];

		float w = vm[3][0] * x + vm[3][1] * y + vm[3][2] * z + vm[3][3];
		float inv_w = 1.f / w;
		_x *= inv_w;
		_y	 *= inv_w;

		float screenx = GetSystemMetrics(SM_CXFULLSCREEN) * .5f;
		float screeny = GetSystemMetrics(SM_CYFULLSCREEN) * .5f;

		screenx += 0.5f * _x * GetSystemMetrics(SM_CXFULLSCREEN) + 0.5f;
		screeny -= 0.5f * _y * GetSystemMetrics(SM_CYFULLSCREEN) + 0.5f;
		return Vector_t(screenx, screeny, w);
	}

	float Dist(Vector_t other) {
		return std::sqrt(std::pow(other.x - x, 2) + std::pow(other.y - y, 2));
	};

	ImVec2 To2D() {
		return { x,y };
	}

};