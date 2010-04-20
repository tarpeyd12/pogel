#include "quat_class.h"

POGEL::MATRIX POGEL::QUAT::tomatrix() {
	float m[16];
	
	m[ 0]=(1.0-(2.0*y*y + 2.0*z*z));	m[ 4]=(2.0*x*y + 2.0*z*w);			m[ 8]=(2.0*x*z - 2.0*y*w);		m[12]=0.0;
	m[ 1]=(2.0*x*y - 2.0*z*w);			m[ 5]=(1.0-(2.0*x*x + 2.0*z*z));	m[ 9]=(2.0*y*z + 2.0*z*w);		m[13]=0.0;
	m[ 2]=(2.0*x*z + 2.0*y*z);			m[ 6]=(2.0*y*z - 2.0*x*w);			m[10]=(1.0-(2*x*x + 2.0*y*y));	m[14]=0.0;
	m[ 3]=0.0;							m[ 7]=0.0;							m[11]=0.0;						m[15]=1.0;
	
	return POGEL::MATRIX(m);
};

