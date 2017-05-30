#pragma once

#include <rtools\framebuffer.h>
#include <rtools\utils.h>
#include <rtools\screenquad.h>

class GLBlur{
private:
	Framebuffer* large2;
	Framebuffer* med1;
	Framebuffer* med2;
	Framebuffer* small1;
	Framebuffer* small2;

	GlProgram* shader_blurRow;
	GlProgram* shader_combine;

	ScreenQuad* quad;

	void create(Framebuffer* _top);
	void blurXYTo(Framebuffer* a, Framebuffer* b, float xoff, float yoff);
public:
	Framebuffer* top;

	GLBlur(Framebuffer* top){
		quad = new ScreenQuad();
		quad->gen();
		create(top);
	}
	void blur();

	// TODO: if motion blur is wanted:
	//void blurOnVelocityMap();
	//void putOnTexture();

	void drawBloom(float add, float mult);

	void clean();
};
