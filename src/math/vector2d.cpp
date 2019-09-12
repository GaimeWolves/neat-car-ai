#include "vector2d.hpp"

Vector2D FromAngle(float r)
{
	return {sin(r), cos(r)};
}

float euclidDist(Vector2D a, Vector2D b)
{
	return (float)(sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2)));
}

Vector2D GetClosestPointToLine(Vector2D a, Vector2D b, Vector2D p)
{
	Vector2D a_to_p = p - a;
	Vector2D a_to_b = b - a;
	float mag_a_to_b = pow(a_to_b.x, 2) + pow(a_to_b.y, 2);
	float atp_dot_atb = a_to_p.Dot(a_to_b);
	float t = atp_dot_atb / mag_a_to_b;
	return (a + a_to_b * t);
}

std::ostream& operator<<(std::ostream &strm, const Vector2D &a) 
{
	return strm << "{ " << a.x << " " << a.y << " }";
}