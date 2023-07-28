#pragma once
#include"Cal.h"
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHalfWidth = 2.0f;
	const uint32_t kSubdivision = 10;
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);

	Vector3 localBorderVer[2]{};
	Vector3 localStripeVer[2]{};

	Vector3 screenBorderVer[2]{};
	Vector3 screenStripeVer[2]{};


	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {


		localBorderVer[0] = { -kGridHalfWidth, 0.0f, kGridEvery * (float(xIndex) - 5) };
		localBorderVer[1] = { kGridHalfWidth, 0.0f, kGridEvery * (float(xIndex) - 5) };

		localStripeVer[0] = { kGridEvery * (int(xIndex) - 5) , 0.0f, -kGridHalfWidth };
		localStripeVer[1] = { kGridEvery * (int(xIndex) - 5) , 0.0f, kGridHalfWidth };


		Vector3 ndcBorderStart = TransformCoord(localBorderVer[0], viewProjectionMatrix);
		Vector3 ndcBorderEnd = TransformCoord(localBorderVer[1], viewProjectionMatrix);

		Vector3 ndcStripeStart = TransformCoord(localStripeVer[0], viewProjectionMatrix);
		Vector3 ndcStripeEnd = TransformCoord(localStripeVer[1], viewProjectionMatrix);

		screenBorderVer[0] = TransformCoord(ndcBorderStart, viewportMatrix);
		screenBorderVer[1] = TransformCoord(ndcBorderEnd, viewportMatrix);

		screenStripeVer[0] = TransformCoord(ndcStripeStart, viewportMatrix);
		screenStripeVer[1] = TransformCoord(ndcStripeEnd, viewportMatrix);

		Novice::DrawLine(
			int(screenBorderVer[0].x), int(screenBorderVer[0].y),
			int(screenBorderVer[1].x), int(screenBorderVer[1].y),
			0xAAAAAAFF);

		Novice::DrawLine(
			int(screenStripeVer[0].x), int(screenStripeVer[0].y),
			int(screenStripeVer[1].x), int(screenStripeVer[1].y),
			0xAAAAAAFF);

		if (localBorderVer[0].z == 0) {
			Novice::DrawLine(
				int(screenStripeVer[0].x), int(screenStripeVer[0].y),
				int(screenStripeVer[1].x), int(screenStripeVer[1].y),
				0x000000FF);

			Novice::DrawLine(
				int(screenBorderVer[0].x), int(screenBorderVer[0].y),
				int(screenBorderVer[1].x), int(screenBorderVer[1].y),
				0x000000FF);
		}

	}

};

void DrawShere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	const uint32_t kSubdivision = 10;
	const float kLonEvery = (2 * 3.14f) / kSubdivision;
	const float kLatEvery = 3.14f / kSubdivision;
	//
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -3.14f / 2.0f + kLatEvery * latIndex;
		//
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;
			//
			Vector3 a, b, c;
			a.x = sphere.center.x + sphere.radius * std::cosf(lat) * std::cosf(lon);
			a.y = sphere.center.y + sphere.radius * std::sinf(lat);
			a.z = sphere.center.z + sphere.radius * std::cosf(lat) * std::sinf(lon);

			b.x = sphere.center.x + sphere.radius * std::cosf(lat + (3.14f / kSubdivision)) * std::cosf(lon);
			b.y = sphere.center.y + sphere.radius * std::sinf(lat + (3.14f / kSubdivision));
			b.z = sphere.center.z + sphere.radius * std::cosf(lat + (3.14f / kSubdivision)) * std::sinf(lon);

			c.x = sphere.center.x + sphere.radius * std::cosf(lat) * std::cosf(lon + ((3.14f * 2) / kSubdivision));
			c.y = sphere.center.y + sphere.radius * std::sinf(lat);
			c.z = sphere.center.z + sphere.radius * std::cosf(lat) * std::sinf(lon + ((3.14f * 2) / kSubdivision));

			Vector3 ndcA = TransformCoord(a, viewProjectionMatrix);
			Vector3 ndcB = TransformCoord(b, viewProjectionMatrix);
			Vector3 ndcC = TransformCoord(c, viewProjectionMatrix);

			Vector3 screenA = TransformCoord(ndcA, viewportMatrix);
			Vector3 screenB = TransformCoord(ndcB, viewportMatrix);
			Vector3 screenC = TransformCoord(ndcC, viewportMatrix);

			//
			//
			Novice::DrawLine(int(screenA.x), int(screenA.y), int(screenB.x), int(screenB.y), color);
			Novice::DrawLine(int(screenA.x), int(screenA.y), int(screenC.x), int(screenC.y), color);

		}
	}
};
void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 center = Multiply(plane.distance, plane.normal);
	Vector3 perpendiculars[4];
	perpendiculars[0] = Normalize(Perpendicular(plane.normal));
	perpendiculars[1] = { -perpendiculars[0].x, -perpendiculars[0].y, -perpendiculars[0].z };
	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);
	perpendiculars[3] = { -perpendiculars[2].x, -perpendiculars[2].y, -perpendiculars[2].z };

	Vector3 points[4];
	for (uint32_t index = 0; index < 4; ++index) {
		Vector3 extend = Multiply(2.0f, perpendiculars[index]);
		Vector3 point = Add(center, extend);
		points[index] = TransformCoord(TransformCoord(point, viewProjectionMatrix), viewportMatrix);
	}

	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[3].x), int(points[3].y), color);
	Novice::DrawLine(int(points[3].x), int(points[3].y), int(points[1].x), int(points[1].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[2].x), int(points[2].y), color);
	Novice::DrawLine(int(points[2].x), int(points[2].y), int(points[0].x), int(points[0].y), color);

};

void DrawLine(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {

	Vector3 localLine[2];
	Vector3 screenLine[2];

	localLine[0] = segment.origin;
	localLine[1] = Add(segment.origin, segment.diff);

	Vector3 ndcBorderStart = TransformCoord(localLine[0], viewProjectionMatrix);
	Vector3 ndcBorderEnd = TransformCoord(localLine[1], viewProjectionMatrix);

	screenLine[0] = TransformCoord(ndcBorderStart, viewportMatrix);
	screenLine[1] = TransformCoord(ndcBorderEnd, viewportMatrix);

	Novice::DrawLine(
		int(screenLine[0].x), int(screenLine[0].y),
		int(screenLine[1].x), int(screenLine[1].y),
		color);
}

void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix4x4, uint32_t color) {

	Vector3 screenVertices[3];
	Vector3 ndcVertex;

	//スクリーンへ変換
	for (uint32_t i = 0; i < 3; ++i) {
		ndcVertex = TransformCoord(triangle.vertices[i], viewProjectionMatrix);
		screenVertices[i] = TransformCoord(ndcVertex, viewportMatrix4x4);

	}
	Novice::DrawTriangle(int(screenVertices[0].x), int(screenVertices[0].y),
		int(screenVertices[1].x), int(screenVertices[1].y),
		int(screenVertices[2].x), int(screenVertices[2].y),
		color, kFillModeWireFrame);
}

void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewPortMatrix, uint32_t color) {

	Vector3 aabbVertices[8]{};
	Vector3 aabbWorldVertices[8]{};
	Vector3 aabbScreenVertices[8]{};

	// 左下手前
	aabbVertices[0] = aabb.min;
	// 右下手前
	aabbVertices[1] = { aabb.max.x, aabb.min.y, aabb.min.z };
	// 右下奥
	aabbVertices[2] = { aabb.max.x, aabb.min.y, aabb.max.z };
	// 左下奥
	aabbVertices[3] = { aabb.min.x, aabb.min.y, aabb.max.z };

	// 左上手前
	aabbVertices[4] = { aabb.min.x, aabb.max.y , aabb.min.z };
	// 右上手前
	aabbVertices[5] = { aabb.max.x, aabb.max.y , aabb.min.z };
	// 右上奥
	aabbVertices[6] = { aabb.max.x, aabb.max.y, aabb.max.z };
	// 左上奥
	aabbVertices[7] = { aabb.min.x, aabb.max.y, aabb.max.z };

	// スクリーン座標系に変換
	for (int i = 0; i < 8; i++) {
		// スクリーン座標系に変換
		aabbWorldVertices[i] = TransformCoord(aabbVertices[i], viewProjectionMatrix);
		aabbScreenVertices[i] = TransformCoord(aabbWorldVertices[i], viewPortMatrix);
	}

	/// 線を引く

	// 四角形下
	Novice::DrawLine((int)aabbScreenVertices[0].x, (int)aabbScreenVertices[0].y, (int)aabbScreenVertices[1].x, (int)aabbScreenVertices[1].y, color);
	Novice::DrawLine((int)aabbScreenVertices[1].x, (int)aabbScreenVertices[1].y, (int)aabbScreenVertices[2].x, (int)aabbScreenVertices[2].y, color);
	Novice::DrawLine((int)aabbScreenVertices[2].x, (int)aabbScreenVertices[2].y, (int)aabbScreenVertices[3].x, (int)aabbScreenVertices[3].y, color);
	Novice::DrawLine((int)aabbScreenVertices[3].x, (int)aabbScreenVertices[3].y, (int)aabbScreenVertices[0].x, (int)aabbScreenVertices[0].y, color);

	// 四角形上
	Novice::DrawLine((int)aabbScreenVertices[4].x, (int)aabbScreenVertices[4].y, (int)aabbScreenVertices[5].x, (int)aabbScreenVertices[5].y, color);
	Novice::DrawLine((int)aabbScreenVertices[5].x, (int)aabbScreenVertices[5].y, (int)aabbScreenVertices[6].x, (int)aabbScreenVertices[6].y, color);
	Novice::DrawLine((int)aabbScreenVertices[6].x, (int)aabbScreenVertices[6].y, (int)aabbScreenVertices[7].x, (int)aabbScreenVertices[7].y, color);
	Novice::DrawLine((int)aabbScreenVertices[7].x, (int)aabbScreenVertices[7].y, (int)aabbScreenVertices[4].x, (int)aabbScreenVertices[4].y, color);

	// 四角形辺
	Novice::DrawLine((int)aabbScreenVertices[0].x, (int)aabbScreenVertices[0].y, (int)aabbScreenVertices[4].x, (int)aabbScreenVertices[4].y, color);
	Novice::DrawLine((int)aabbScreenVertices[1].x, (int)aabbScreenVertices[1].y, (int)aabbScreenVertices[5].x, (int)aabbScreenVertices[5].y, color);
	Novice::DrawLine((int)aabbScreenVertices[2].x, (int)aabbScreenVertices[2].y, (int)aabbScreenVertices[6].x, (int)aabbScreenVertices[6].y, color);
	Novice::DrawLine((int)aabbScreenVertices[3].x, (int)aabbScreenVertices[3].y, (int)aabbScreenVertices[7].x, (int)aabbScreenVertices[7].y, color);

}