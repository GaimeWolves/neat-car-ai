#pragma once

#include <math.h>
#include <iostream>

struct Vector2D
{
	float x;
	float y;

	float Lenght()
	{
		return sqrt(pow(x, 2) + pow(y, 2));
	}

	float Angle()
	{
		return atan2(y, x);
	}

	Vector2D Normalize()
	{
		float len = Lenght();
		x /= len;
		y /= len;

		return {x, y};
	}

	Vector2D Rotate(float r)
	{
		float angle = Angle() + r;
		float len = Lenght();
		x = cos(angle) * len;
		y = sin(angle) * len;

		return {x, y};
	}

	float Dot(Vector2D other)
	{
		return x * other.x + y * other.y;
	}

	Vector2D operator+(Vector2D b) { return {x + b.x, y + b.y}; }
	Vector2D operator-(Vector2D b) { return {x - b.x, y - b.y}; }
	Vector2D operator*(Vector2D b) { return {x * b.x, y * b.y}; }
	Vector2D operator/(Vector2D b) { return {x / b.x, y / b.y}; }

	Vector2D operator*(float s) { return {x * s, y * s}; }
	Vector2D operator/(float s) { return {x / s, y / s}; }

	void operator+=(Vector2D b) { x += b.x; y += b.y; }
	void operator-=(Vector2D b) { x -= b.x; y -= b.y; }
	void operator*=(Vector2D b) { x *= b.x; y *= b.y; }
	void operator/=(Vector2D b) { x /= b.x; y /= b.y; }

	void operator*=(float s) { x *= s; y *= s; }
	void operator/=(float s) { x /= s; y /= s; }

	friend std::ostream& operator<<(std::ostream&, const Vector2D&);
};

Vector2D FromAngle(float r);
float euclidDist(Vector2D a, Vector2D b);
Vector2D GetClosestPointToLine(Vector2D a, Vector2D b, Vector2D p);