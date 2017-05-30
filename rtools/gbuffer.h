#pragma once

#include <iostream>

#include <GL/glew.h>

class GBuffer{
private:
	void create(int w, int h, int numBuffers, GLenum* internalFormats, bool withDepthBuffer);
	bool _withDepthBuffer;
public:
	GLuint fbId;
	GLuint fbId_depth;

	GLuint* texId;
	GLuint texId_depth;

	int numBuffers;

	int w;
	int h;

	GBuffer(int w, int h, int numBuffers, GLenum* internalFormats, bool withDepthBuffer){
		create(w, h, numBuffers, internalFormats, withDepthBuffer);
	}

	void clean();
};
