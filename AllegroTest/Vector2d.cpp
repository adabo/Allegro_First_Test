#include "Vector2d.h"
#include <math.h>

Vector2d::Vector2d()
{
}

Vector2d::Vector2d(float _x, float _y)
	:	x(_x),
		y(_y)
{
}

Vector2d::Vector2d(Vector2d &_v0, Vector2d &_v1)
{
	*this = _v1 - _v0;
}

void Vector2d::print_vector2d() const
{
	std::cout << x << " " << y << std::endl;
}

float Vector2d::dot_product(Vector2d &_v0, Vector2d &_v1) const
{
	return _v0.x * _v1.x + _v0.y * _v1.y;
}

float Vector2d::get_distance()
{
	return sqrt(x * x + y * y);
}

void Vector2d::get_normal(float _distance)
{
	normal_x = x / _distance;
	normal_y = y / _distance;
}

Vector2d Vector2d::operator+(const Vector2d &vector_input) const
{
	return Vector2d(x + vector_input.x, y + vector_input.y);
}

Vector2d Vector2d::operator*(const Vector2d &vector_input) const
{
	return Vector2d(x * vector_input.x, y * vector_input.y);
}

Vector2d Vector2d::operator-(const Vector2d &vector_input) const
{
	return Vector2d(x - vector_input.x, y - vector_input.y);
}
