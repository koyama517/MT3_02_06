#pragma once
#include"Vector3.h"
#include"Novice.h"
#include"Matrix4x4.h"

struct Plane {
	Vector3 normal;
	float distance;
};

struct Line {
	Vector3 origin;	//!< 始点
	Vector3 diff; //!< 終点への差分ベクトル
};

struct  Ray {
	Vector3 origin; //!< 始点
	Vector3 diff; //!< 終点への差分ベクトル
};

struct  Segment {
	Vector3 origin; //!< 始点
	Vector3 diff; //!< 終点への差分ベクトル
};

struct Sphere {
	Vector3 center;
	float radius;
};

struct Triangle {
	Vector3 vertices[3];
};

struct AABB {
	Vector3 min;
	Vector3 max;
};