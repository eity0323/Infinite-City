#pragma once

#include <iostream>

#include <GL/glew.h>

class Framebuffer{
private:
	void create(int w, int h, bool withDepthBuffer);
	bool _withDepthBuffer;
public:
	GLuint fbId;
	GLuint fbId_depth;

	GLuint texId;
	GLuint texId_depth;

	int w;
	int h;

	Framebuffer(int w, int h){
		create(w, h, false);
	}
	Framebuffer(int w, int h, bool withDepthBuffer){
		create(w, h, withDepthBuffer);
	}

	void clean();
};
