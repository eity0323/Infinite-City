#ifndef CUBE_CLOUDS
#define CUBE_CLOUDS

#include <GL/glew.h>
#include <freeglut.h>

# include <gsim/gs_color.h>
# include <gsim/gs_mat.h>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include <gsim/gs_image.h>

# include <rtools/utils.h>

class CubeClouds{
private:
	GLuint *va, *buf;
	int na, nb;

	int numVertices;
	int numIndices;

	int* indices;

	GlProgram* cloudShader;

	// Shader uniform locations:
	int shader_xRot;
	int shader_yRot;
		
	int shader_yCos;
	int shader_ySin;
	int shader_xCos;
	int shader_xSin;
		
	int shader_ox;
	int shader_oy;
	int shader_oz;

	int shader_ar;
		
	int shader_maptex;
	int shader_depthtex;
		
	int shader_drawDistance;

	int shader_m_near;
	int shader_m_far;
		
	int shader_mapWidth;
	int shader_mapHeight;
	int shader_mapDepth;
	// ^^^

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

	int mapTexID;

	uint8_t* map;

	int mapWidth;
	int mapHeight;
	int mapDepth;

	void initMap();

	int genTextureID(){
		GLuint tex[1];
		glGenTextures(1, tex);
		return tex[1];
	}
public:
	double xRot;
	double yRot;
	
	double yCos;
	double ySin;
	double xCos;
	double xSin;

	double ox;
	double oy;
	double oz;

	double ar;
	
	double drawDistance;

	void gen();
	void draw(int depthTexId, float m_near, float m_far);
};
#endif // CUBE_CLOUDS
