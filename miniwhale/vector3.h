#pragma once

namespace vec3
{
	void inverse(GLfloat[]);
	void diff(const GLfloat[], const GLfloat[], GLfloat[]);
	bool cmp(const GLfloat[], const GLfloat[]);
	bool iszero(const GLfloat[]);
	void copy(const GLfloat[], GLfloat[]);
	void translate(const GLfloat[], const GLfloat[], GLfloat[]);
	void scale(const GLfloat[], const GLfloat[], GLfloat[]);
	GLfloat dist(const GLfloat[], const GLfloat[]);
	GLfloat lensq(const GLfloat[]);
	GLfloat magnitude(const GLfloat[]);
	GLfloat dot(const GLfloat[], const GLfloat[]);
	GLfloat angle(const GLfloat[], const GLfloat[]);
	void cross(const GLfloat[], const GLfloat[], GLfloat[]);
	void norm(const GLfloat[], GLfloat[]);
	GLfloat theta(const GLfloat[]);
	GLfloat phi(const GLfloat[]);
	void print(const GLfloat[]);

}

{
inline bool cmpf(float A, float B, float epsilon = 0.005f)
{
	return (fabs(A - B) < epsilon);
}

GLfloat rad2deg(GLfloat radians) {
	return radians * (180.0f / 3.141592653589793238f);
}

GLfloat deg2rad(float degrees) {
	return degrees * (3.141592653589793238f / 180.0f);
}

inline void vec3::inverse(GLfloat v[3])
{
	v[0] = -v[0];
	v[1] = -v[1];
	v[2] = -v[2];
}

inline void vec3::diff(const GLfloat v0[3], const GLfloat v1[3], GLfloat out[3])
{
	out[0] = v0[0] - v1[0];
	out[1] = v0[1] - v1[1];
	out[2] = v0[2] - v1[2];
}

inline bool vec3::cmp(const GLfloat v0[3], const GLfloat v1[3])
{
	// compare with epsilon
	return (cmpf(v0[0], v1[0]) &&
		cmpf(v0[1], v1[1]) &&
		cmpf(v0[2], v1[2]));
}

inline bool vec3::iszero(const GLfloat v[3])
{
	GLfloat zero[3] = { 0,0,0 };
	return vec3::cmp(v, zero);
}

inline void vec3::copy(const GLfloat v[3], GLfloat out[3])
{
	out[0] = v[0];
	out[1] = v[1];
	out[2] = v[2];
}

inline void vec3::translate(const GLfloat v0[3], const GLfloat v1[3], GLfloat out[3])
{
	out[0] = v0[0] + v1[0];
	out[1] = v0[1] + v1[1];
	out[2] = v0[2] + v1[2];
}

inline void vec3::scale(const GLfloat v0[3], const GLfloat v1[3], GLfloat out[3])
{
	out[0] = v0[0] * v1[0];
	out[1] = v0[1] * v1[1];
	out[2] = v0[2] * v1[2];
}

inline GLfloat vec3::dist(const GLfloat p1[3], const GLfloat p2[3])
{
	GLfloat x = p1[0] - p2[0];
	GLfloat y = p1[1] - p2[1];
	GLfloat z = p1[2] - p2[2];
	return sqrt(x*x + y * y + z * z);
}

inline GLfloat vec3::lensq(const GLfloat v[3])
{
	return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}

inline GLfloat vec3::magnitude(const GLfloat v[3])
{
	return sqrt(vec3::lensq(v));
}

inline GLfloat vec3::dot(const GLfloat v0[3], const GLfloat v1[3])
{
	return v0[0] * v1[0] + v0[1] * v1[1] + v0[2] * v1[2];
}

inline GLfloat vec3::angle(const GLfloat v0[3], const GLfloat v1[3])
{
	return rad2deg(acos(vec3::dot(v0,v1) / sqrt(vec3::lensq(v0) * vec3::lensq(v1))));
}

inline void vec3::cross(const GLfloat v0[3], const GLfloat v1[3], GLfloat out[3])
{
	out[0] = v0[1] * v1[2] - v0[2] * v1[1];
	out[1] = v0[0] * v1[2] - v0[2] * v1[0];
	out[2] = v0[0] * v1[1] - v0[1] * v1[0];
}

inline void vec3::norm(const GLfloat v[3], GLfloat out[3])
{
	GLfloat mag = vec3::magnitude(v);
	assert(mag != 0);
	out[0] = v[0] / mag;
	out[1] = v[1] / mag;
	out[2] = v[2] / mag;
}

inline GLfloat theta(const GLfloat v[3])
{
	GLfloat xymag = sqrt(v[0] * v[0] + v[1] * v[1]);
	return rad2deg(atan2(xymag, v[3]));

}

inline GLfloat phi(const GLfloat v[3])
{
	return rad2deg(atan2(v[1], v[0]));
}

inline void vec3::print(const GLfloat v[3])
{
	std::cout << "{" << v[0] << "," << v[1] << "," << v[2] << "}";
}
