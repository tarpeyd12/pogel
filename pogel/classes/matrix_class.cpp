#include <stdio.h>
#include <stdlib.h>

#include "matrix_class.h"
#include "quat_class.h"

float POGEL::RadiansToDegrees(float r) {return (r/PI*180);}; // does what it says
float POGEL::DegreesToRadians(float d) {return (d/180*PI);}; // ditto

POGEL::MATRIX::MATRIX() {
	// default constructor that sets the brand new matrix as the identity matrix
	matrix[ 0] = 1.0f;	matrix[ 4] = 0.0f;	matrix[ 8] = 0.0f;	matrix[12] = 0.0f;
	matrix[ 1] = 0.0f;	matrix[ 5] = 1.0f;	matrix[ 9] = 0.0f;	matrix[13] = 0.0f;
	matrix[ 2] = 0.0f;	matrix[ 6] = 0.0f;	matrix[10] = 1.0f;	matrix[14] = 0.0f;
	matrix[ 3] = 0.0f;	matrix[ 7] = 0.0f;	matrix[11] = 0.0f;	matrix[15] = 1.0f;
};

POGEL::MATRIX::MATRIX(float* m) {
	// constructs this matrix as the array 'm'
	set(m);
};

POGEL::MATRIX::MATRIX(const POGEL::MATRIX& m) {
	// constructs this matrix as the MATRIX 'm'
	set(m);
};

POGEL::MATRIX::MATRIX(float rot, int axis) {
	*this = POGEL::MATRIX(); // set as identity
	rot=POGEL::DegreesToRadians(rot);
	switch(axis) {
		case MATRIX_CONSTRUCT_X_ROTATION:
			// sets the matrix to the rotation matrix on the x
			setvalue(1,1, (float)cos(rot));
			setvalue(1,2, (float)sin(rot));
			setvalue(2,1, (float)-sin(rot));
			setvalue(2,2, (float)cos(rot));
		break;
		
		case MATRIX_CONSTRUCT_Y_ROTATION:
			// sets the matrix to the rotation matrix on the y
			setvalue(0,0, (float)cos(rot));
			setvalue(2,0, (float)sin(rot));
			setvalue(0,2, (float)-sin(rot));
			setvalue(2,2, (float)cos(rot));
		break;
		
		case MATRIX_CONSTRUCT_Z_ROTATION:
			// sets the matrix to the rotation matrix on the z
			setvalue(0,0, (float)cos(rot));
			setvalue(0,1, (float)sin(rot));
			setvalue(1,0, (float)-sin(rot));
			setvalue(1,1, (float)cos(rot));
		break;
	}
	//print();
};

POGEL::MATRIX::MATRIX(POGEL::POINT point, int i) {
	*this = POGEL::MATRIX(); // set as identity
	switch(i) {
		case MATRIX_CONSTRUCT_ROTATION: // for constructing rotation
			*this = POGEL::MATRIX(point.x, MATRIX_CONSTRUCT_X_ROTATION) * MATRIX(point.y, MATRIX_CONSTRUCT_Y_ROTATION) * MATRIX(point.z, MATRIX_CONSTRUCT_Z_ROTATION); // sets to the product of all the rotation matricies
		break;
		
		case MATRIX_CONSTRUCT_TRANSLATION: // for constructing translation
			matrix[12]=point.x; // sets the x translation of the matrix to the x value of POINT 'point'
			matrix[13]=point.y; // sets the y translation of the matrix to the y value of POINT 'point'
			matrix[14]=point.z; // sets the z translation of the matrix to the z value of POINT 'point'
		break;
		
		case MATRIX_CONSTRUCT_SCALE: // for constructing scales
			matrix[ 0]=point.x; // sets the x scale of the matrix to the x value of POINT 'point'
			matrix[ 5]=point.y; // sets the y scale of the matrix to the y value of POINT 'point'
			matrix[10]=point.z; // sets the z scale of the matrix to the z value of POINT 'point'
		break;
	}
};

POGEL::MATRIX::MATRIX(POGEL::VECTOR axis,float angle) {
	*this = POGEL::MATRIX();
	fromaxis(axis, angle);
};

POGEL::MATRIX::MATRIX(POGEL::POINT position, POGEL::POINT rotation) {
	// creates the matrix with position: 'position', and rotation: 'rotation'
	*this = (POGEL::MATRIX(rotation, MATRIX_CONSTRUCT_ROTATION) * POGEL::MATRIX(position, MATRIX_CONSTRUCT_TRANSLATION) /* POGEL::MATRIX()*/);
	//print();
};

void POGEL::MATRIX::set(float* m) {
	// sets the current matrix to the values in the array 'm'
	if(m!=NULL)
		for(int i=0;i<16;i++)
			matrix[i]=m[i];
	else printf("matrix loading failed, null pointer to matrix array.\n");
};

void POGEL::MATRIX::set(POGEL::MATRIX m) {
	// sets the current matrix to the values of MATRIX 'm'
	*this = m;
};

POGEL::POINT POGEL::MATRIX::getrotation() {
	// returnes the rotation of the matrx in a POINT object(don't know how it works)
	float angle_x;
	float angle_z;
	
	float tr_x, tr_y;
	
	float angle_y =  asin( matrix[2]);        /* Calculate Y-axis angle */
	//float D = angle_y;
    float C           =  cos( angle_y );
    angle_y=POGEL::RadiansToDegrees(angle_y);
    if ( fabs( C ) > 0.005 )             /* Gimball lock? */
      {
      tr_x      =  matrix[10] / C;           /* No, so get X-axis angle */
      tr_y      = -matrix[6]  / C;
      angle_x  = POGEL::RadiansToDegrees(atan2( tr_y, tr_x ));
      tr_x      =  matrix[0] / C;            /* Get Z-axis angle */
      tr_y      = -matrix[1] / C;
      angle_z  = POGEL::RadiansToDegrees(atan2( tr_y, tr_x ));
      }
    else                                 /* Gimball lock has occurred */
      {
      angle_x  = 0;                      /* Set X-axis angle to zero */
      tr_x      =  matrix[5];                 /* And calculate Z-axis angle */
      tr_y      =  matrix[4];
      angle_z  = POGEL::RadiansToDegrees(atan2( tr_y, tr_x ));
      }

    /* return only positive angles in [0,360] */
    if (angle_x < 0) angle_x += 360;
    if (angle_y < 0) angle_y += 360;
    if (angle_z < 0) angle_z += 360;
	return POGEL::POINT(angle_x, angle_y, angle_z);
};

POGEL::POINT POGEL::MATRIX::getposition() {
	// returnes a POINT object of the translation of the matrix
	return POGEL::POINT(matrix[12], matrix[13], matrix[14]);
};

float *POGEL::MATRIX::getcolumn(int i) {
	// returnes an array of 4 floats that is the column 'i' of the matrix
	float *ret=(float*)malloc(sizeof(float)*4);
	//for(int i=0;i<4;i++) // columns
		for(int c=0;c<4;c++) // rows
			ret[c]=matrix[i*4+c];
	return ret;
};

float *POGEL::MATRIX::getrow(int c) {
	// returnes an array of 4 floats that is the row 'c' of the matrix
	float *ret=(float*)malloc(sizeof(float)*4);
	for(int i=0;i<4;i++) // columns
		//for(int c=0;c<4;c++) // rows
			ret[i]=matrix[i*4+c];
	return ret;
};

void POGEL::MATRIX::multiplyby(float m) {
	// multiplies the matrix by the value 'm'
	for(int i=0;i<16;i++)
		matrix[i]*=m;
};

void POGEL::MATRIX::raistopower(int p) {
	// raises the matrix to the power of 'p'
	POGEL::MATRIX a, m(*this);
	int i=p;
	while(i<0) {
		if(i%2) a=a*m;
		i=i/2;
		m=m*m;
	}
	*this = a;
};

float POGEL::MATRIX::determinant() {
	// returnes the determinant of the matrix
	float inv[16];
	
	inv[0] =   matrix[ 5]*matrix[10]*matrix[15] - matrix[ 5]*matrix[11]*matrix[14] - matrix[ 9]*matrix[ 6]*matrix[15] + matrix[ 9]*matrix[ 7]*matrix[14] + matrix[13]*matrix[ 6]*matrix[11] - matrix[13]*matrix[ 7]*matrix[10];
	inv[4] =  -matrix[ 4]*matrix[10]*matrix[15] + matrix[ 4]*matrix[11]*matrix[14] + matrix[ 8]*matrix[ 6]*matrix[15] - matrix[ 8]*matrix[ 7]*matrix[14] - matrix[12]*matrix[ 6]*matrix[11] + matrix[12]*matrix[ 7]*matrix[10];
	inv[8] =   matrix[ 4]*matrix[ 9]*matrix[15] - matrix[ 4]*matrix[11]*matrix[13] - matrix[ 8]*matrix[ 5]*matrix[15] + matrix[ 8]*matrix[ 7]*matrix[13] + matrix[12]*matrix[ 5]*matrix[11] - matrix[12]*matrix[ 7]*matrix[ 9];
	inv[12] = -matrix[ 4]*matrix[ 9]*matrix[14] + matrix[ 4]*matrix[10]*matrix[13] + matrix[ 8]*matrix[ 5]*matrix[14] - matrix[ 8]*matrix[ 6]*matrix[13] - matrix[12]*matrix[ 5]*matrix[10] + matrix[12]*matrix[ 6]*matrix[ 9];
	
	return matrix[0]*inv[0] + matrix[1]*inv[4] + matrix[2]*inv[8] + matrix[3]*inv[12];
};

bool POGEL::MATRIX::invert() {
	// inverts the matrix, if the matrix is not invertable, it returnes false
	float inv[16], det;
	int i;
	
	// this baheamoth was pulled from: http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
	inv[0] =   matrix[ 5]*matrix[10]*matrix[15] - matrix[ 5]*matrix[11]*matrix[14] - matrix[ 9]*matrix[ 6]*matrix[15] + matrix[ 9]*matrix[ 7]*matrix[14] + matrix[13]*matrix[ 6]*matrix[11] - matrix[13]*matrix[ 7]*matrix[10];
	inv[4] =  -matrix[ 4]*matrix[10]*matrix[15] + matrix[ 4]*matrix[11]*matrix[14] + matrix[ 8]*matrix[ 6]*matrix[15] - matrix[ 8]*matrix[ 7]*matrix[14] - matrix[12]*matrix[ 6]*matrix[11] + matrix[12]*matrix[ 7]*matrix[10];
	inv[8] =   matrix[ 4]*matrix[ 9]*matrix[15] - matrix[ 4]*matrix[11]*matrix[13] - matrix[ 8]*matrix[ 5]*matrix[15] + matrix[ 8]*matrix[ 7]*matrix[13] + matrix[12]*matrix[ 5]*matrix[11] - matrix[12]*matrix[ 7]*matrix[ 9];
	inv[12] = -matrix[ 4]*matrix[ 9]*matrix[14] + matrix[ 4]*matrix[10]*matrix[13] + matrix[ 8]*matrix[ 5]*matrix[14] - matrix[ 8]*matrix[ 6]*matrix[13] - matrix[12]*matrix[ 5]*matrix[10] + matrix[12]*matrix[ 6]*matrix[ 9];
	inv[1] =  -matrix[ 1]*matrix[10]*matrix[15] + matrix[ 1]*matrix[11]*matrix[14] + matrix[ 9]*matrix[ 2]*matrix[15] - matrix[ 9]*matrix[ 3]*matrix[14] - matrix[13]*matrix[ 2]*matrix[11] + matrix[13]*matrix[ 3]*matrix[10];
	inv[5] =   matrix[ 0]*matrix[10]*matrix[15] - matrix[ 0]*matrix[11]*matrix[14] - matrix[ 8]*matrix[ 2]*matrix[15] + matrix[ 8]*matrix[ 3]*matrix[14] + matrix[12]*matrix[ 2]*matrix[11] - matrix[12]*matrix[ 3]*matrix[10];
	inv[9] =  -matrix[ 0]*matrix[ 9]*matrix[15] + matrix[ 0]*matrix[11]*matrix[13] + matrix[ 8]*matrix[ 1]*matrix[15] - matrix[ 8]*matrix[ 3]*matrix[13] - matrix[12]*matrix[ 1]*matrix[11] + matrix[12]*matrix[ 3]*matrix[ 9];
	inv[13] =  matrix[ 0]*matrix[ 9]*matrix[14] - matrix[ 0]*matrix[10]*matrix[13] - matrix[ 8]*matrix[ 1]*matrix[14] + matrix[ 8]*matrix[ 2]*matrix[13] + matrix[12]*matrix[ 1]*matrix[10] - matrix[12]*matrix[ 2]*matrix[ 9];
	inv[2] =   matrix[ 1]*matrix[ 6]*matrix[15] - matrix[ 1]*matrix[ 7]*matrix[14] - matrix[ 5]*matrix[ 2]*matrix[15] + matrix[ 5]*matrix[ 3]*matrix[14] + matrix[13]*matrix[ 2]*matrix[ 7] - matrix[13]*matrix[ 3]*matrix[ 6];
	inv[6] =  -matrix[ 0]*matrix[ 6]*matrix[15] + matrix[ 0]*matrix[ 7]*matrix[14] + matrix[ 4]*matrix[ 2]*matrix[15] - matrix[ 4]*matrix[ 3]*matrix[14] - matrix[12]*matrix[ 2]*matrix[ 7] + matrix[12]*matrix[ 3]*matrix[ 6];
	inv[10] =  matrix[ 0]*matrix[ 5]*matrix[15] - matrix[ 0]*matrix[ 7]*matrix[13] - matrix[ 4]*matrix[ 1]*matrix[15] + matrix[ 4]*matrix[ 3]*matrix[13] + matrix[12]*matrix[ 1]*matrix[ 7] - matrix[12]*matrix[ 3]*matrix[ 5];
	inv[14] = -matrix[ 0]*matrix[ 5]*matrix[14] + matrix[ 0]*matrix[ 6]*matrix[13] + matrix[ 4]*matrix[ 1]*matrix[14] - matrix[ 4]*matrix[ 2]*matrix[13] - matrix[12]*matrix[ 1]*matrix[ 6] + matrix[12]*matrix[ 2]*matrix[ 5];
	inv[3] =  -matrix[ 1]*matrix[ 6]*matrix[11] + matrix[ 1]*matrix[ 7]*matrix[10] + matrix[ 5]*matrix[ 2]*matrix[11] - matrix[ 5]*matrix[ 3]*matrix[10] - matrix[ 9]*matrix[ 2]*matrix[ 7] + matrix[ 9]*matrix[ 3]*matrix[ 6];
	inv[7] =   matrix[ 0]*matrix[ 6]*matrix[11] - matrix[ 0]*matrix[ 7]*matrix[10] - matrix[ 4]*matrix[ 2]*matrix[11] + matrix[ 4]*matrix[ 3]*matrix[10] + matrix[ 8]*matrix[ 2]*matrix[ 7] - matrix[ 8]*matrix[ 3]*matrix[ 6];
	inv[11] = -matrix[ 0]*matrix[ 5]*matrix[11] + matrix[ 0]*matrix[ 7]*matrix[ 9] + matrix[ 4]*matrix[ 1]*matrix[11] - matrix[ 4]*matrix[ 3]*matrix[ 9] - matrix[ 8]*matrix[ 1]*matrix[ 7] + matrix[ 8]*matrix[ 3]*matrix[ 5];
	inv[15] =  matrix[ 0]*matrix[ 5]*matrix[10] - matrix[ 0]*matrix[ 6]*matrix[ 9] - matrix[ 4]*matrix[ 1]*matrix[10] + matrix[ 4]*matrix[ 2]*matrix[ 9] + matrix[ 8]*matrix[ 1]*matrix[ 6] - matrix[ 8]*matrix[ 2]*matrix[ 5];
	
	det = matrix[0]*inv[0] + matrix[1]*inv[4] + matrix[2]*inv[8] + matrix[3]*inv[12];
	if (det == 0.0f)
        return false;
	
	det = 1.0 / det;
	
	for (i = 0; i < 16; i++)
        matrix[i] = inv[i] * det;
	
	return true;
};

void POGEL::MATRIX::print() {
	// prints the matrix out to the terminal
	printf("\n");
	for(int i=0;i<4;i+=1) { // rows
		POGEL::message("%7.3f,%7.3f,%7.3f,%7.3f\n", matrix[i+0], matrix[i+4], matrix[i+8], matrix[i+12]);
	}	
};

void POGEL::MATRIX::transformPoint(POGEL::POINT* p) {
	POGEL::POINT tmp=(*p);
	p->x = tmp.x * getvalue(0,0) + tmp.y * getvalue(1,0) + tmp.z * getvalue(2,0) + getvalue(3,0);
	p->y = tmp.x * getvalue(0,1) + tmp.y * getvalue(1,1) + tmp.z * getvalue(2,1) + getvalue(3,1);
	p->z = tmp.x * getvalue(0,2) + tmp.y * getvalue(1,2) + tmp.z * getvalue(2,2) + getvalue(3,2);
};

POGEL::POINT POGEL::MATRIX::transformPoint(POGEL::POINT p) {
	transformPoint(&p);
	return (p);
};

void POGEL::MATRIX::transformVector(POGEL::VECTOR* v) {
	// transforms the VECTOR 'v' by the rotation of the matrix
	POGEL::VECTOR tmp=(*v);
	v->x = tmp.x * getvalue(0,0) + tmp.y * getvalue(1,0) + tmp.z * getvalue(2,0);// + getvalue(3,0);
	v->y = tmp.x * getvalue(0,1) + tmp.y * getvalue(1,1) + tmp.z * getvalue(2,1);// + getvalue(3,1);
	v->z = tmp.x * getvalue(0,2) + tmp.y * getvalue(1,2) + tmp.z * getvalue(2,2);// + getvalue(3,2);
};

POGEL::VECTOR POGEL::MATRIX::transformVector(POGEL::VECTOR v) {
	// transforms the VECTOR 'v' by the rotation of the matrix
	transformVector(&v);
	return (v);
};

void POGEL::MATRIX::transformVertex(POGEL::VERTEX* v) {
	// get the vector from center to vertex v, then get its length, 
	// normalize it, transform it myltiply it by the vector length, 
	// then use that as the new vertex. or do the same as converting a vector 'cause it's simpler
	POGEL::VERTEX tmp=(*v);
	v->x = (tmp.x * getvalue(0,0) + tmp.y * getvalue(1,0) + tmp.z * getvalue(2,0) + getvalue(3,0));// + getposition().x;
	v->y = (tmp.x * getvalue(0,1) + tmp.y * getvalue(1,1) + tmp.z * getvalue(2,1) + getvalue(3,1));// + getposition().y;
	v->z = (tmp.x * getvalue(0,2) + tmp.y * getvalue(1,2) + tmp.z * getvalue(2,2) + getvalue(3,2));// + getposition().z;
	transformVector(&v->normal); // now transform the 
};

POGEL::VERTEX POGEL::MATRIX::transformVertex(POGEL::VERTEX v) {
	transformVertex(&v);
	return (v);
};

void POGEL::MATRIX::transformTriangle(POGEL::TRIANGLE* tri) {
	for(int i=0;i<3;i++)
		transformVertex(&tri->vertex[i]);
	transformVector(&tri->normal);
};

POGEL::TRIANGLE POGEL::MATRIX::transformTriangle(POGEL::TRIANGLE tri) {
	transformTriangle(&tri);
	return tri;
};

POGEL::QUAT POGEL::MATRIX::toquat() {
	// converts the matrix to a Quatranation (dont know how it works)
	POGEL::QUAT q;
	float t=1.0+matrix[0]+matrix[5]+matrix[10];
	if(t>0.00000001) { // testing for gimble lock (I think)
		float s=(float)sqrt(t)*2.0;
		q.x=(matrix[9]-matrix[6])/s;
		q.y=(matrix[2]-matrix[8])/s;
		q.z=(matrix[4]-matrix[1])/s;
		q.w=0.25*s;
		return q;
	}
	else {
		if ( matrix[0] > matrix[5] && matrix[0] > matrix[10] )  {	// Column 0: 
			float S  = sqrt( 1.0 + matrix[0] - matrix[5] - matrix[10] ) * 2;
			q.x = 0.25 * S;
			q.y = (matrix[4] + matrix[1] ) / S;
			q.z = (matrix[2] + matrix[8] ) / S;
			q.w = (matrix[9] - matrix[6] ) / S;
			return q;
		} else if ( matrix[5] > matrix[10] ) {			// Column 1: 
			float S  = sqrt( 1.0 + matrix[5] - matrix[0] - matrix[10] ) * 2;
			q.x = (matrix[4] + matrix[1] ) / S;
			q.y = 0.25 * S;
			q.z = (matrix[9] + matrix[6] ) / S;
			q.w = (matrix[2] - matrix[8] ) / S;
			return q;
		} else {						// Column 2:
			float S  = sqrt( 1.0 + matrix[10] - matrix[0] - matrix[5] ) * 2;
			q.x = (matrix[2] + matrix[8] ) / S;
			q.y = (matrix[9] + matrix[6] ) / S;
			q.z = 0.25 * S;
			q.w = (matrix[4] - matrix[1] ) / S;
			return q;
		}
	}
};

//TODO: make this work
void POGEL::MATRIX::fromaxis(POGEL::VECTOR axis, float angle) {
	float u = axis.x, v = axis.y, w = axis.z;
	
	*this = POGEL::MATRIX();
	
	angle = POGEL::DegreesToRadians(angle);
	
	float rcos = cos(angle);
	float rsin = sin(angle);
	setvalue(0, 0,       rcos + u*u*(1-rcos));
	setvalue(1, 0,   w * rsin + v*u*(1-rcos));
	setvalue(2, 0,  -v * rsin + w*u*(1-rcos));
	setvalue(0, 1,  -w * rsin + u*v*(1-rcos));
	setvalue(1, 1,       rcos + v*v*(1-rcos));
	setvalue(2, 1,   u * rsin + w*v*(1-rcos));
	setvalue(0, 2,   v * rsin + u*w*(1-rcos));
	setvalue(1, 2,  -u * rsin + v*w*(1-rcos));
	setvalue(2, 2,       rcos + w*w*(1-rcos));
};

POGEL::MATRIX POGEL::MATRIX::operator+(POGEL::MATRIX m) {
	// simply adds the two matricies together
	POGEL::MATRIX ret;
	for(int i=0;i<16;i++)
		ret.matrix[i]=m.matrix[i]+matrix[i];
	return ret;
};

POGEL::MATRIX POGEL::MATRIX::operator-(POGEL::MATRIX m) {
	// simply subtracts the two matricies from each other
	POGEL::MATRIX ret;
	for(int i=0;i<16;i++)
		ret.matrix[i] = matrix[i] - m.matrix[i];
	return ret;
};

POGEL::MATRIX POGEL::MATRIX::operator*(POGEL::MATRIX a) {
	// multiplies the two matricies together
	POGEL::MATRIX ret;
	for(int i=0;i<4;i++) { // columns
		for(int c=0;c<4;c++) { // rows
			//ret.matrix[i*4+c] = 0.0f;
			//for(int f=0;f<4;f++)
				//ret.matrix[i*4+c]+=(this->getvalue(i, f))*(a.getvalue(f, c));
			
			ret.matrix[i*4+c] = (this->getvalue(i, 0))*(a.getvalue(0, c)) + (this->getvalue(i, 1))*(a.getvalue(1, c)) + (this->getvalue(i, 2))*(a.getvalue(2, c)) + (this->getvalue(i, 3))*(a.getvalue(3, c));
			
		}
	}
	//ret = ret - POGEL::MATRIX(); // subtract the identity matrix
	return ret;
};

POGEL::MATRIX& POGEL::MATRIX::operator=(const POGEL::MATRIX& m) {
	// assignes one matrix the value of another
	for(int i=0;i<16;i++) matrix[i]=m.matrix[i];
	
	/*matrix[ 0]=m.matrix[ 0];
	matrix[ 1]=m.matrix[ 1];
	matrix[ 2]=m.matrix[ 2];
	matrix[ 3]=m.matrix[ 3];
	matrix[ 4]=m.matrix[ 4];
	matrix[ 5]=m.matrix[ 5];
	matrix[ 6]=m.matrix[ 6];
	matrix[ 7]=m.matrix[ 7];
	matrix[ 8]=m.matrix[ 8];
	matrix[ 9]=m.matrix[ 9];
	matrix[10]=m.matrix[10];
	matrix[11]=m.matrix[11];
	matrix[12]=m.matrix[12];
	matrix[13]=m.matrix[13];
	matrix[14]=m.matrix[14];
	matrix[15]=m.matrix[15];*/
	
	return *this;
	//return m;
};

