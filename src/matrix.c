// incluir cabecera 42

#include "../inc/minirt.h"

// Function to sum tuples
t_vector	add(t_vector a, t_vector b)
{
	t_vector	result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return (result);
}
// Function to subtract tuples
t_vector	subtract(t_vector a, t_vector b)
{
	t_vector	result;

	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
	return (result);
}
// Function to multiply tuples
t_vector dott(t_vector a, t_vector b)
{
	t_vector	result;

	result.x = a.x * b.x;
	result.y = a.y * b.y;
	result.z = a.z * b.z;
	return (result);
}
// Function to calculate the dot product of two vectors
float	dot(t_vector a, t_vector b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}
// Function to multiply a tuple by a scalar
t_vector	multiply(t_vector a, float scalar)
{
	t_vector	result;

	result.x = a.x * scalar;
	result.y = a.y * scalar;
	result.z = a.z * scalar;
	return (result);
}
// Function to divide a tuple by a scalar
t_vector	divide(t_vector a, float scalar)
{
	t_vector	result;

	result.x = a.x / scalar;
	result.y = a.y / scalar;
	result.z = a.z / scalar;
	return (result);
}
t_vector transpose(t_vector a)
{
	t_vector	result;

	result.x = a.x;
	result.y = a.z;
	result.z = a.y;
	return (result);
}
// Function to calculate the magnitude of a vector
float	magnitude(t_vector a)
{
	return (sqrt(a.x * a.x + a.y * a.y + a.z * a.z));
}
// Function to normalize a vector
t_vector	normalize(t_vector a)
{
	float	mag;
	t_vector	result;

	mag = magnitude(a);
	result.x = a.x / mag;
	result.y = a.y / mag;
	result.z = a.z / mag;
	return (result);
}
// Function to calculate the cross product of two vectors
t_vector	cross(t_vector a, t_vector b)
{
	t_vector	result;

	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
	return (result);
}
// Function to reflect a vector
t_vector	reflect(t_vector a, t_vector b)
{
	t_vector	result;
	float		dot_product;

	dot_product = dot(a, b);
	result = subtract(a, multiply(b, 2 * dot_product));
	return (result);
}
// Function to rotate a vector
t_vector	rotate(t_vector a, t_vector b, float angle)
{
	t_vector	result;
	float		cosine;
	float		sine;

	cosine = cos(angle);
	sine = sin(angle);
	result.x = (cosine + (1 - cosine) * b.x * b.x) * a.x +
		((1 - cosine) * b.x * b.y - b.z * sine) * a.y +
		((1 - cosine) * b.x * b.z + b.y * sine) * a.z;
	result.y = ((1 - cosine) * b.x * b.y + b.z * sine) * a.x +
		(cosine + (1 - cosine) * b.y * b.y) * a.y +
		((1 - cosine) * b.y * b.z - b.x * sine) * a.z;
	result.z = ((1 - cosine) * b.x * b.z - b.y * sine) * a.x +
		((1 - cosine) * b.y * b.z + b.x * sine) * a.y +
		(cosine + (1 - cosine) * b.z * b.z) * a.z;
	return (result);
}
// Function to rotate a vector around the x axis
t_vector	rotate_x(t_vector a, float angle)
{
	t_vector	result;

	result.x = a.x;
	result.y = a.y * cos(angle) - a.z * sin(angle);
	result.z = a.y * sin(angle) + a.z * cos(angle);
	return (result);
}
// Function to rotate a vector around the y axis
t_vector	rotate_y(t_vector a, float angle)
{
	t_vector	result;

	result.x = a.x * cos(angle) + a.z * sin(angle);
	result.y = a.y;
	result.z = -a.x * sin(angle) + a.z * cos(angle);
	return (result);
}
// Function to rotate a vector around the z axis
t_vector	rotate_z(t_vector a, float angle)
{
	t_vector	result;

	result.x = a.x * cos(angle) - a.y * sin(angle);
	result.y = a.x * sin(angle) + a.y * cos(angle);
	result.z = a.z;
	return (result);
}
// Function to calculate the angle between two vectors
float	angle(t_vector a, t_vector b)
{
	float	mag_a;
	float	mag_b;
	float	dot_product;

	mag_a = magnitude(a);
	mag_b = magnitude(b);
	dot_product = dot(a, b);
	return (acos(dot_product / (mag_a * mag_b)));
}
