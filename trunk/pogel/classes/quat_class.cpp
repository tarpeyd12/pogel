#include "quat_class.h"

POGEL::QUAT::QUAT(POGEL::VECTOR v, float _w) {
	x = v.x;
	y = v.y;
	z = v.z;
	w = _w;
};

POGEL::QUAT::QUAT(POGEL::MATRIX& mat) {
	*this = mat.toquat();
};

POGEL::MATRIX POGEL::QUAT::tomatrix() {
	float m[16];
	
	m[ 0] = (1.0f-(2.0f*y*y + 2.0f*z*z));	m[ 4] = (2.0f*x*y + 2.0f*z*w);			m[ 8] = (2.0f*x*z - 2.0f*y*w);			m[12] = (0.0f);
	m[ 1] = (2.0f*x*y - 2.0f*z*w);			m[ 5] = (1.0f-(2.0f*x*x + 2.0f*z*z));	m[ 9] = (2.0f*y*z + 2.0f*z*w);			m[13] = (0.0f);
	m[ 2] = (2.0f*x*z + 2.0f*y*z);			m[ 6] = (2.0f*y*z - 2.0f*x*w);			m[10] = (1.0f-(2.0f*x*x + 2.0f*y*y));	m[14] = (0.0f);
	m[ 3] = (0.0f);							m[ 7] = (0.0f);							m[11] = (0.0f);							m[15] = (1.0f);
	
	return POGEL::MATRIX(m);
};

POGEL::VECTOR POGEL::QUAT::getvector() {
	return POGEL::VECTOR(x, y, z);
};

POGEL::QUAT POGEL::QUAT::operator+ (float angle) {
	POGEL::QUAT ret;
	
	ret.x = x;
	ret.y = y;
	ret.z = z;
	
	ret.w = w + angle;
	
	return ret;
};

POGEL::QUAT POGEL::QUAT::operator+ (POGEL::VECTOR axis) {
	POGEL::QUAT ret;
	
	ret.x = x + axis.x;
	ret.y = y + axis.y;
	ret.z = z + axis.x;
	
	ret.w = w;
	
	return ret;
};

POGEL::QUAT& POGEL::QUAT::operator= (const POGEL::QUAT& q) {
	x = q.x;
	y = q.y;
	z = q.z;
	w = q.w;
	
	return *this;
}
