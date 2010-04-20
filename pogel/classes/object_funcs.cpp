#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "point_class.h"
#include "triangle_class.h"
#include "object_funcs.h"

void addSphere(POGEL::OBJECT *obj, float spacev, float spaceh, float R, POGEL::IMAGE *img, float imgscalev, float imgscaleh, unsigned int triprop) {
	addSphere(obj,spacev,spaceh,R,img,imgscalev,imgscaleh,triprop,POGEL::MATRIX());
};

void addSphere(POGEL::OBJECT *obj, float spacev, float spaceh, float R, POGEL::IMAGE *img, float imgscalev, float imgscaleh, unsigned int triprop, POGEL::MATRIX mat) {
	if(POGEL::hasproperty(POGEL_DEBUG)) POGEL::message("Adding Sphere to \"%s\" ...\n", obj->getname());
	float a;
	float b;
	POGEL::POINT center(0.0f,0.0f,0.0f);
	unsigned long cur_vert=0;
	POGEL::VERTEX verts[4];
	POGEL::TRIANGLE tri;
	
	spacev=180.0f/spacev;
	spaceh=360.0f/spaceh;
	unsigned long vertcount=(180 / spacev) * (360 / spaceh);
	for( b = 0.0f; b <= 180.0f - spacev+0.001f; b+=spacev){
		if(vertcount==cur_vert) break;
		for( a = 0.0f; a <= 360.0f - spaceh+0.001f; a+=spaceh){
			if(vertcount==cur_vert) break;
			verts[0].set_values(R*(sin((a)/180.0f*PI)*sin((b)/180.0f*PI))+center.x, \
								R*(cos((b)/180.0f*PI))+center.y, \
								R*(cos((a)/180.0f*PI)*sin((b)/180.0f*PI))+center.z, \
								imgscaleh*(a)/360.0f, \
								imgscalev*-(b)/180.0f);
			
			verts[1].set_values(R*(sin((a)/180.0f*PI)*sin((b+spacev)/180.0f*PI))+center.x, \
								R*(cos((b+spacev)/180.0f*PI))+center.y, \
								R*(cos((a)/180.0f*PI)*sin((b+spacev)/180.0f*PI))+center.z, \
								imgscaleh*(a)/360.0f, \
								imgscalev*-(b+spacev)/180.0f);
			
			verts[2].set_values(R*(sin((a+spaceh)/180.0f*PI)*sin((b)/180.0f*PI))+center.x, \
								R*(cos((b)/180.0f*PI))+center.y, \
								R*(cos((a+spaceh)/180.0f*PI)*sin((b)/180.0f*PI))+center.z, \
								imgscaleh*(a+spaceh)/360.0f, \
								imgscalev*-(b)/180.0f);
			
			verts[3].set_values(R*(sin((a+spaceh)/180.0f*PI)*sin((b+spacev)/180.0f*PI))+center.x, \
								R*(cos((b+spacev)/180.0f*PI))+center.y, \
								R*(cos((a+spaceh)/180.0f*PI)*sin((b+spacev)/180.0f*PI))+center.z, \
								imgscaleh*(a+spaceh)/360.0f, \
								imgscalev*-(b+spacev)/180.0f);
			
			for(int i=0;i<4;i++) {
				mat.transformVertex(&verts[i]);
				verts[i].normal.frompoints(mat.getposition(),verts[i].topoint());
				verts[i].normal.normalize();
			}
			
			tri.load(verts[0],verts[1],verts[2], img, triprop);
			//mat.transformVector(&tri.normal);
			obj->addtriangle(tri);
			tri.load(verts[2],verts[1],verts[3], img, triprop);
			//mat.transformVector(&tri.normal);
			obj->addtriangle(tri);
			cur_vert+=1;
			if(POGEL::hasproperty(POGEL_DEBUG)) POGEL::message("%ld of %ld faces complete in sphere.\r", cur_vert, vertcount);
		}
	}
	if(POGEL::hasproperty(POGEL_DEBUG)) POGEL::message("\n");
};

void addDisk(POGEL::OBJECT *obj, float divisions, float rings, float outer_radius, float inner_radius, POGEL::IMAGE *img, float imgscalev, float imgscaleh, unsigned int triprop) {
	addDisk(obj,divisions,rings,outer_radius,inner_radius,img,imgscalev,imgscaleh,triprop,false,POGEL::MATRIX());
};

void addDisk(POGEL::OBJECT *obj, float divisions, float rings, float outer_radius, float inner_radius, POGEL::IMAGE *img, float imgscalev, float imgscaleh, unsigned int triprop, bool radial_tex, POGEL::MATRIX mat) {
	if(POGEL::hasproperty(POGEL_DEBUG)) POGEL::message("Adding Disk to \"%s\" ...\n", obj->getname());
	float a;
	float b;
	POGEL::POINT center(0.0f,0.0f,0.0f);
	unsigned long cur_vert=0;
	POGEL::VERTEX verts[4];
	POGEL::TRIANGLE tri;
	
	divisions=360.0f/divisions;
	unsigned long vertcount=((360/divisions)*rings);
	rings=(outer_radius-inner_radius)/rings;
	for(a=inner_radius;a<outer_radius;a+=rings) { // rings
		if(vertcount==cur_vert) break;
		for(b=0.0f;b<=360.0f-divisions;b+=divisions) { // divisions
			if(vertcount==cur_vert) break;
			verts[0].set_point_values(	(a)*sin((b)/180.0f*PI)+center.x, \
										(a)*cos((b)/180.0f*PI)+center.y, \
										0.0f+center.z
			);
			
			verts[1].set_point_values(	(a)*sin((b+divisions)/180.0f*PI)+center.x, \
										(a)*cos((b+divisions)/180.0f*PI)+center.y, \
										0.0f+center.z
			);
			
			verts[2].set_point_values(	(a+rings)*sin((b)/180.0f*PI)+center.x, \
										(a+rings)*cos((b)/180.0f*PI)+center.y, \
										0.0f+center.z
			);
			
			verts[3].set_point_values(	(a+rings)*sin((b+divisions)/180.0f*PI)+center.x, \
										(a+rings)*cos((b+divisions)/180.0f*PI)+center.y, \
										0.0f+center.z
			);
			
			if(radial_tex) {
				verts[0].set_tex_values(imgscalev*(b)/360.0f, \
										imgscaleh*((a-inner_radius))/(outer_radius-inner_radius));
				
				verts[1].set_tex_values(imgscalev*(b+divisions)/360.0f, \
										imgscaleh*((a-inner_radius))/(outer_radius-inner_radius));
				
				verts[2].set_tex_values(imgscalev*(b)/360.0f, \
										imgscaleh*((a-inner_radius)+rings)/(outer_radius-inner_radius));
				
				verts[3].set_tex_values(imgscalev*(b+divisions)/360.0f, \
										imgscaleh*((a-inner_radius)+rings)/(outer_radius-inner_radius));
			}
			else {
				verts[0].set_tex_values(imgscaleh*(0.5f+((a)*sin((b)/180.0f*PI))/2.0f), \
										imgscalev*(0.5f+((a)*cos((b)/180.0f*PI))/2.0f));
				
				verts[1].set_tex_values(imgscaleh*(0.5f+((a)*sin((b+divisions)/180.0f*PI))/2.0f), \
										imgscalev*(0.5f+((a)*cos((b+divisions)/180.0f*PI))/2.0f));
				
				verts[2].set_tex_values(imgscaleh*(0.5f+((a+rings)*sin((b)/180.0f*PI))/2.0f), \
										imgscalev*(0.5f+((a+rings)*cos((b)/180.0f*PI))/2.0f));
				
				verts[3].set_tex_values(imgscaleh*(0.5f+((a+rings)*sin((b+divisions)/180.0f*PI))/2.0f), \
										imgscalev*(0.5f+((a+rings)*cos((b+divisions)/180.0f*PI))/2.0f));
			}
			for(int i=0;i<4;i++) {
				mat.transformVertex(&verts[i]);
				verts[i].normal.frompoints(mat.getposition(),verts[i].topoint());
				verts[i].normal.normalize();
			}
			
			tri.load(verts[0],verts[1],verts[2], img, triprop);
			//mat.transformVector(&tri.normal);
			obj->addtriangle(tri);
			tri.load(verts[2],verts[1],verts[3], img, triprop);
			//mat.transformVector(&tri.normal);
			obj->addtriangle(tri);
			cur_vert+=1;
			if(POGEL::hasproperty(POGEL_DEBUG)) POGEL::message("%ld of %ld faces complete in disk.\r", cur_vert, vertcount);
		}
	}
	if(POGEL::hasproperty(POGEL_DEBUG)) POGEL::message("\n");
}




void addCylinder(POGEL::OBJECT *obj, float divisions, float rings_in, float height, float lower_radius, float upper_radius, POGEL::IMAGE *img, float imgscalev, float imgscaleh, unsigned int triprop, POGEL::MATRIX mat) {
	if(POGEL::hasproperty(POGEL_DEBUG)) POGEL::message("Adding Cylinder to \"%s\" ...\n", obj->getname());
	float a;
	float b;
	float c;
	POGEL::POINT center(0.0f,0.0f,0.0f);
	unsigned long cur_vert=0;
	POGEL::VERTEX verts[4];
	POGEL::TRIANGLE tri;
	float texdiv=((center.y-(height/2.0f))-(center.y+(height/2.0f)));
	if(texdiv==0.0f) texdiv=1.0f;
	divisions=360.0f/divisions;
	unsigned long vertcount=((360/divisions)*rings_in);
	float hinc=height/rings_in;
	float rings=(lower_radius-upper_radius)/rings_in;
	if(lower_radius>upper_radius) {
		float tmp = lower_radius;
		lower_radius = upper_radius;
		upper_radius = tmp;
		//lower_radius*=-1.0f;
		//upper_radius*=-1.0f;
	}
	else
		imgscaleh*=-1.0f;
	for(a=lower_radius, c=center.y-(height/2.0f);(a<upper_radius || c<center.y+(height/2.0f));a+=rings, c+=hinc) { // rings
		if(vertcount==cur_vert) break;
		for(b=0.0f;b<=360.0f-divisions;b+=divisions) { // divisions
			if(vertcount==cur_vert) break;
			verts[0].set_point_values(	(a)*sin((b)/180.0f*PI)+center.x, \
										(c)+center.y, \
										(a)*cos((b)/180.0f*PI)+center.z
			);
			
			verts[1].set_point_values(	(a)*sin((b+divisions)/180.0f*PI)+center.x, \
										(c)+center.y, \
										(a)*cos((b+divisions)/180.0f*PI)+center.z
			);
			
			verts[2].set_point_values(	(a+rings)*sin((b)/180.0f*PI)+center.x, \
										(c+hinc)+center.y, \
										(a+rings)*cos((b)/180.0f*PI)+center.z
			);
			
			verts[3].set_point_values(	(a+rings)*sin((b+divisions)/180.0f*PI)+center.x, \
										(c+hinc)+center.y, \
										(a+rings)*cos((b+divisions)/180.0f*PI)+center.z
			);
			
			
			verts[0].set_tex_values(imgscalev*(b)/360.0f, \
									//imgscaleh*((c-((height/2.0f)/imgscaleh)))/texdiv);
									(imgscaleh/rings_in)*(((c)/*+hinc*/)/hinc)+(imgscaleh/2.0f));
			
			verts[1].set_tex_values(imgscalev*(b+divisions)/360.0f, \
									//imgscaleh*((c-((height/2.0f)/imgscaleh)))/texdiv);
									(imgscaleh/rings_in)*(((c)/*+hinc*/)/hinc)+(imgscaleh/2.0f));
			
			verts[2].set_tex_values(imgscalev*(b)/360.0f, \
									//imgscaleh*((c-((height/2.0f)/imgscaleh))+hinc)/texdiv);
									(imgscaleh/rings_in)*(((c)+hinc)/hinc)+(imgscaleh/2.0f));
			
			verts[3].set_tex_values(imgscalev*(b+divisions)/360.0f, \
									//imgscaleh*((c-((height/2.0f)/imgscaleh))+hinc)/texdiv);
									(imgscaleh/rings_in)*(((c)+hinc)/hinc)+(imgscaleh/2.0f));
			
			for(int i=0;i<4;i++) {
				mat.transformVertex(&verts[i]);
				verts[i].normal.frompoints(mat.getposition(),verts[i].topoint());
				verts[i].normal.normalize();
			}
			
			tri.load(verts[0],verts[1],verts[2], img, triprop);
			//mat.transformVector(&tri.normal);
			obj->addtriangle(tri);
			tri.load(verts[2],verts[1],verts[3], img, triprop);
			//mat.transformVector(&tri.normal);
			obj->addtriangle(tri);
			cur_vert+=1;
			if(POGEL::hasproperty(POGEL_DEBUG)) POGEL::message("%ld of %ld faces complete in cylinder.\r", cur_vert, vertcount);
		}
	}
	if(POGEL::hasproperty(POGEL_DEBUG)) POGEL::message("\n");
}

void addCube(POGEL::OBJECT *obj, float height, float width, float depth, POGEL::IMAGE* img, float imgscale_h, float imgscale_w, unsigned int triprop, POGEL::MATRIX mat) {
	if(POGEL::hasproperty(POGEL_DEBUG)) POGEL::message("Adding Cube to \"%s\" ...\n", obj->getname());
	POGEL::VERTEX verts[4];
	
	//front
	verts[0].set_values(-width/2.0f,-height/2.0f, depth/2.0f,       0.0f,      0.0f); // lowerleft
	verts[1].set_values( width/2.0f,-height/2.0f, depth/2.0f, imgscale_w,      0.0f); // lowerright
	verts[2].set_values( width/2.0f, height/2.0f, depth/2.0f, imgscale_w,imgscale_h); // upperright
	verts[3].set_values(-width/2.0f, height/2.0f, depth/2.0f,       0.0f,imgscale_h); // upperleft
	for(int i=0;i<4;i++) {
		mat.transformVertex(&verts[i]);
		verts[i].normal.frompoints(mat.getposition(),verts[i].topoint());
		verts[i].normal.normalize();
	}
	obj->addtriangle(POGEL::TRIANGLE(verts[0],verts[1],verts[2],img,triprop));
	obj->addtriangle(POGEL::TRIANGLE(verts[3],verts[0],verts[2],img,triprop));
	
	if(POGEL::hasproperty(POGEL_DEBUG)) POGEL::message("1 of 6 faces complete in cube.\r");
	
	//back
	verts[0].set_values(-width/2.0f,-height/2.0f,-depth/2.0f, imgscale_w,      0.0f); // lowerleft
	verts[1].set_values(-width/2.0f, height/2.0f,-depth/2.0f, imgscale_w,imgscale_h); // lowerright
	verts[2].set_values( width/2.0f, height/2.0f,-depth/2.0f,       0.0f,imgscale_h); // upperright
	verts[3].set_values( width/2.0f,-height/2.0f,-depth/2.0f,       0.0f,      0.0f); // upperleft
	for(int i=0;i<4;i++) {
		mat.transformVertex(&verts[i]);
		verts[i].normal.frompoints(mat.getposition(),verts[i].topoint());
		verts[i].normal.normalize();
	}
	obj->addtriangle(POGEL::TRIANGLE(verts[0],verts[1],verts[2],img,triprop));
	obj->addtriangle(POGEL::TRIANGLE(verts[3],verts[0],verts[2],img,triprop));
	
	if(POGEL::hasproperty(POGEL_DEBUG)) POGEL::message("2 of 6 faces complete in cube.\r");
	
	//top
	verts[0].set_values(-width/2.0f, height/2.0f,-depth/2.0f,       0.0f,imgscale_h); // lowerleft
	verts[1].set_values(-width/2.0f, height/2.0f, depth/2.0f,       0.0f,      0.0f); // lowerright
	verts[2].set_values( width/2.0f, height/2.0f, depth/2.0f, imgscale_w,      0.0f); // upperright
	verts[3].set_values( width/2.0f, height/2.0f,-depth/2.0f, imgscale_w,imgscale_h); // upperleft
	for(int i=0;i<4;i++) {
		mat.transformVertex(&verts[i]);
		verts[i].normal.frompoints(mat.getposition(),verts[i].topoint());
		verts[i].normal.normalize();
	}
	obj->addtriangle(POGEL::TRIANGLE(verts[0],verts[1],verts[2],img,triprop));
	obj->addtriangle(POGEL::TRIANGLE(verts[3],verts[0],verts[2],img,triprop));
	
	if(POGEL::hasproperty(POGEL_DEBUG)) POGEL::message("3 of 6 faces complete in cube.\r");
	
	//bottom
	verts[0].set_values(-width/2.0f,-height/2.0f,-depth/2.0f, imgscale_w,imgscale_h); // lowerleft
	verts[1].set_values( width/2.0f,-height/2.0f,-depth/2.0f,       0.0f,imgscale_h); // lowerright
	verts[2].set_values( width/2.0f,-height/2.0f, depth/2.0f,       0.0f,      0.0f); // upperright
	verts[3].set_values(-width/2.0f,-height/2.0f, depth/2.0f, imgscale_w,      0.0f); // upperleft
	for(int i=0;i<4;i++) {
		mat.transformVertex(&verts[i]);
		verts[i].normal.frompoints(mat.getposition(),verts[i].topoint());
		verts[i].normal.normalize();
	}
	obj->addtriangle(POGEL::TRIANGLE(verts[0],verts[1],verts[2],img,triprop));
	obj->addtriangle(POGEL::TRIANGLE(verts[3],verts[0],verts[2],img,triprop));
	
	if(POGEL::hasproperty(POGEL_DEBUG)) POGEL::message("4 of 6 faces complete in cube.\r");
	
	//left
	verts[0].set_values(-width/2.0f,-height/2.0f,-depth/2.0f,       0.0f,      0.0f); // lowerleft
	verts[1].set_values(-width/2.0f,-height/2.0f, depth/2.0f, imgscale_w,      0.0f); // lowerright
	verts[2].set_values(-width/2.0f, height/2.0f, depth/2.0f, imgscale_w,imgscale_h); // upperright
	verts[3].set_values(-width/2.0f, height/2.0f,-depth/2.0f,       0.0f,imgscale_h); // upperleft
	for(int i=0;i<4;i++) {
		mat.transformVertex(&verts[i]);
		verts[i].normal.frompoints(mat.getposition(),verts[i].topoint());
		verts[i].normal.normalize();
	}
	obj->addtriangle(POGEL::TRIANGLE(verts[0],verts[1],verts[2],img,triprop));
	obj->addtriangle(POGEL::TRIANGLE(verts[3],verts[0],verts[2],img,triprop));
	
	if(POGEL::hasproperty(POGEL_DEBUG)) POGEL::message("5 of 6 faces complete in cube.\r");
	
	//right
	verts[0].set_values( width/2.0f,-height/2.0f,-depth/2.0f, imgscale_w,      0.0f); // lowerleft
	verts[1].set_values( width/2.0f, height/2.0f,-depth/2.0f, imgscale_w,imgscale_h); // lowerright
	verts[2].set_values( width/2.0f, height/2.0f, depth/2.0f,       0.0f,imgscale_h); // upperright
	verts[3].set_values( width/2.0f,-height/2.0f, depth/2.0f,       0.0f,      0.0f); // upperleft
	for(int i=0;i<4;i++) {
		mat.transformVertex(&verts[i]);
		verts[i].normal.frompoints(mat.getposition(),verts[i].topoint());
		verts[i].normal.normalize();
	}
	obj->addtriangle(POGEL::TRIANGLE(verts[0],verts[1],verts[2],img,triprop));
	obj->addtriangle(POGEL::TRIANGLE(verts[3],verts[0],verts[2],img,triprop));
	
	if(POGEL::hasproperty(POGEL_DEBUG)) POGEL::message("6 of 6 faces complete in cube.\r");
	
	if(POGEL::hasproperty(POGEL_DEBUG)) POGEL::message("\n");
};

