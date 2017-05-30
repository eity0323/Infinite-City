#pragma once

#include <vector>
#include "../utils.h"

using namespace std;

#define MAX_CARS 16384

class Car{
private:
	GLuint *va, *buf;
	int na, nb;

	int _numVertices;

	void gen_vertex_arrays(GLsizei n) {
		delete[] va;
		va = new GLuint[n];
		glGenVertexArrays(n, va);
		na = (gsbyte)n;
	}
	void gen_buffers(GLsizei n) {
		delete[] buf;
		buf = new GLuint[n];
		glGenBuffers(n, buf);
		nb = (gsbyte)n;
	}

public:
	Car();
	void build(float unitsize);
	void draw();

	bool i_uploaded;
	int i_num;
	float i_pos[MAX_CARS*4];
	float i_rot[MAX_CARS*4];

	// x,y,z, t; rx,ry,tilt, ?
	void addInstance(GsVec pos, float t, GsVec rot);
	void drawInstances();
	void clearInstances();
};