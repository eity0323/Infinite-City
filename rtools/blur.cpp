#include "blur.h"
#include <iostream>


void GLBlur::create(Framebuffer* _top){
	top = _top;

	large2 = new Framebuffer(top->w, top->h);
	med1 = new Framebuffer(top->w / 2, top->h / 2);
	med2 = new Framebuffer(top->w / 2, top->h / 2);
	small1 = new Framebuffer(top->w / 2, top->h / 2);
	small2 = new Framebuffer(top->w / 4, top->h / 4);

	shader_blurRow = GLUtils::compileShader("../res/shaders/blur/blur_row.vp", "../res/shaders/blur/blur_row.fp");
	shader_combine = GLUtils::compileShader("../res/shaders/blur/blur_combine.vp", "../res/shaders/blur/blur_combine.fp");
}
void GLBlur::blur(){
	blurXYTo(top, large2, 1.2f, 0);
	blurXYTo(large2, top, 0, 1.2f);

	blurXYTo(top, med2, 1.2f, 0);
	blurXYTo(med2, med1, 0, 1.2f);

	blurXYTo(med1, small2, 1.2, 0);
	blurXYTo(small2, small1, 0, 1.2f);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void GLBlur::blurXYTo(Framebuffer* a, Framebuffer* b, float offx, float offy){
	glBindFramebuffer(GL_FRAMEBUFFER, b->fbId);
	glViewport(0, 0, b->w, b->h);

	glUseProgram(shader_blurRow->id);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, a->texId);

	int shader_tex = glGetUniformLocation(shader_blurRow->id, "Texture0");
	int shader_off = glGetUniformLocation(shader_blurRow->id, "off");

	glUniform1i(shader_tex, 0);
	glUniform2f(shader_off, offx / (float)a->w, offy / (float)a->h);

	quad->draw();

}
void GLBlur::drawBloom(float sh_add, float sh_mult){
	glUseProgram(shader_combine->id);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, top->texId);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, med1->texId);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, small1->texId);

	int shader_tex0 = glGetUniformLocation(shader_combine->id, "Texture0");
	int shader_tex1 = glGetUniformLocation(shader_combine->id, "Texture1");
	int shader_tex2 = glGetUniformLocation(shader_combine->id, "Texture2");

	int shader_sh_add = glGetUniformLocation(shader_combine->id, "sh_add");
	int shader_sh_mult = glGetUniformLocation(shader_combine->id, "sh_mult");

	glUniform1i(shader_tex0, 0);
	glUniform1i(shader_tex1, 1);
	glUniform1i(shader_tex2, 2);

	glUniform1f(shader_sh_add, sh_add);
	glUniform1f(shader_sh_mult, sh_mult);

	quad->draw();
}
void GLBlur::clean(){
	large2->clean();
	med1->clean();
	med2->clean();
	small1->clean();
	small2->clean();

	// TODO: delete programs?
}