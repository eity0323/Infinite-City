#ifndef PRIMITIVE_SPHERE_H
#define PRIMITIVE_SPHERE_H

#include <GL/glew.h>
#include <freeglut.h>

# include <gsim/gs_color.h>
# include <gsim/gs_mat.h>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include <gsim/gs_image.h>

class PrimitiveSphere{
private:
	int na, nb;


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
	GLuint *va, *buf;

	int numVertices;
	int numIndices;

	int* indices;

	void gen(float radius, int subdivisions);
	void draw();
};
#endif // PRIMITIVE_SPHERE_H
