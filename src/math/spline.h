#pragma once

#include <vector>
#include <math.h>
#include <iostream>

#include "vector2d.h"

struct Spline
{
	std::vector<Vector2D> Points;

	Vector2D GetSplinePoint(float t, bool isClosed = false)
	{
		if (t < 0 || t > Points.size())
		{
			std::cerr << "WARNING: t is not in range of point vector (0 - " << Points.size() << "). t was " << t << ". Returning point (0, 0)." << std::endl;
			return {0, 0};
		}

		int p0, p1, p2, p3;
		GetPointsFromDistance(Points.size(), t, &p0, &p1, &p2, &p3, isClosed);

		t = fmod(t, 1);

		float q0 = -pow(t, 3) + 2.0f * pow(t, 2) - t;
		float q1 = 3.0f * pow(t, 3) - 5.0f * pow(t, 2) + 2.0f;
		float q2 = -3.0f * pow(t, 3) + 4.0f * pow(t, 2) + t;
		float q3 = pow(t, 3) - pow(t, 2);

		return CalculatePoint(q0, q1, q2, q3, p0, p1, p2, p3, Points);
	}

	Vector2D GetSplineGradient(float t, bool isClosed = false)
	{
		if (t < 0 || t > Points.size())
		{
			std::cerr << "WARNING: t is not in range of point vector (0 - " << Points.size() << "). t was " << t << ". Returning point (0, 0)." << std::endl;
			return {0, 0};
		}

		int p0, p1, p2, p3;
		GetPointsFromDistance(Points.size(), t, &p0, &p1, &p2, &p3, isClosed);

		t = fmod(t, 1);

		float q0 = -3.0f * pow(t, 2) + 4.0f * t - 1.0f;
		float q1 = 9.0f * pow(t, 2) - 10.0f * t;
		float q2 = -9.0f * pow(t, 2) + 8.0f * t + 1.0f;
		float q3 = 3.0f * pow(t, 2) - 2.0f * t;

		return CalculatePoint(q0, q1, q2, q3, p0, p1, p2, p3, Points);
	}

	float ApproximateSegmentLenght(int lowerPointIndex, bool isClosed = false)
	{
		float len = 0;
		float dt = 0.005f;

		Vector2D old = GetSplinePoint((float)lowerPointIndex, isClosed);
		Vector2D newPoint;
		for (float t = dt; t < 1; t += dt)
		{
			newPoint = GetSplinePoint((float)lowerPointIndex + t, isClosed);
			len += euclidDist(old, newPoint);
		}

		return len;
	}

	static Vector2D CalculatePoint(float q0, float q1, float q2, float q3, int p0, int p1, int p2, int p3, std::vector<Vector2D> points)
	{
		float pX = 0.5f * (points[p0].x * q0 + points[p1].x * q1 + points[p2].x * q2 + points[p3].x * q3);
		float pY = 0.5f * (points[p0].y * q0 + points[p1].y * q1 + points[p2].y * q2 + points[p3].y * q3);

		return {pX, pY};
	}

	static void GetPointsFromDistance(int vectorLenght, float t, int* p0, int* p1, int* p2, int* p3, bool isClosed)
	{
		if (!isClosed)
		{
			*p0 = (int)floor(t);
			*p1 = *p0 + 1;
			*p2 = *p1 + 1;
			*p3 = *p2 + 1;
		}
		else
		{
			*p1 = (int)floor(t);
			*p2 = (*p1 + 1) % vectorLenght;
			*p3 = (*p2 + 1) % vectorLenght;
			*p0 = *p1 == 0 ? vectorLenght - 1 : *p1 - 1; 
		}
	}
};
