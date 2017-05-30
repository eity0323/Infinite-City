#include "gbuffer.h"

void GBuffer::create(int _w, int _h, int _numBuffers, GLenum* internalFormats, bool withDepthBuffer){
	_withDepthBuffer = withDepthBuffer;
	w = _w;
	h = _h;

	numBuffers = _numBuffers;

	texId = new GLuint[numBuffers];

	glGenFramebuffers(1, &fbId);
	glGenTextures(numBuffers, texId);

    glBindFramebuffer(GL_FRAMEBUFFER, fbId);
	for (int i = 0; i < numBuffers; i++){
		glBindTexture(GL_TEXTURE_2D, texId[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormats[i], w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, 0); // GL_RGB32F
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texId[i], 0);
	}

    if (withDepthBuffer) {
		glGenRenderbuffers(1, &fbId_depth);
		glGenTextures(1, &texId_depth);

        glBindRenderbuffer(GL_RENDERBUFFER, fbId_depth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbId_depth);

		glBindTexture(GL_TEXTURE_2D, texId_depth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texId_depth, 0);

		// ?
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    

    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        printf("Warning: incomplete framebuffer created! (%d)\n", status);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void GBuffer::clean(){
	// TODO:

	/*
	for (int i = 0; i < numBuffers; i++){
		glDeleteTextures(1, &(texId[i]));
	}
	if (_withDepthBuffer){
		glDeleteTextures(1, &texId_depth);
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &fbId);
	if (_withDepthBuffer){
		glDeleteFramebuffers(1, &fbId_depth);
	}
	*/

}