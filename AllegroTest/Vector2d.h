#include <iostream>

class Vector2d
{
public:
	/* Maybe make a vector struct so no need for vector objects
	 * struct Vector_struct
	 * {
	 * 		float x, y;
	 * };
	 */
	Vector2d();
	Vector2d(float _x, float _y);
	Vector2d(Vector2d &_v0, Vector2d &_v1);
	void print_vector2d() const;
	float dot_product(Vector2d &_v0, Vector2d &_v1) const;
	float get_distance();
	void get_normal(float _distance);

	Vector2d operator*(const Vector2d &vector_input) const;
	Vector2d operator+(const Vector2d &vector_input) const;
	Vector2d operator-(const Vector2d &vector_input) const;

	float x, y;
	float normal_x, normal_y;
};
