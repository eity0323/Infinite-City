#pragma once

#include <vector>
#include <list>
#include "../utils.h"

using namespace std;

class BuildingLayer{
private:
	GLuint *va, *buf;
	int na, nb;

	int _numVertices;

	float elements[4048*4];
	bool uploadedElements;
	int numElements;

	void gen_vertex_arrays(GLsizei n) {
		delete [] va;
		va = new GLuint[n];
		glGenVertexArrays(n, va);
		na = (gsbyte)n;
	}
	void gen_buffers(GLsizei n) {
		delete [] buf;
		buf = new GLuint[n];
		glGenBuffers (n, buf);
		nb = (gsbyte)n;
	}

public:
	BuildingLayer();
	void build(vector<float>* p, vector<float>* n, vector<int>* t);
	void draw();
	void addInstance(GsVec pos, float t);
	void drawInstanced();
	void clearInstances();
	int getPolygonCount();
};