#include <iostream>
#include "app_window.h"
#include <stdio.h>


#define PI_F 3.151592654f

static int getFPS(){
	static int frames = 0;
	static clock_t lastTime = clock();
	static int lastFPS = 0;

	frames++;

	clock_t currentTime = clock();
	double ms = double(currentTime - lastTime) / CLOCKS_PER_SEC * 1000;

	if (ms > 1000.0){
		lastTime = currentTime;
		lastFPS = (frames * 1000) / ms;
		frames = 0;
	}
	return lastFPS;
}
static double frameDiff(){
	static int lastFrame = clock();

	double diff = 60.0*double(clock() - lastFrame) / (CLOCKS_PER_SEC);
	lastFrame = clock();

	return diff;
}

AppWindow::AppWindow(const char* label, int x, int y, int w, int h) : GLWindow(label, x, y, w, h) {
	//singleton = this;

	glutInitWindowPosition(x, y);
	glutInitWindowSize(w, h);
	glutCreateWindow(label);

	// Init glew library (after a glut window is created!):
	GLenum res = glewInit();
	if (res!=GLEW_OK) std::cout << glewGetString(GLEW_VERSION) << ", Error: " << glewGetErrorString(res) << "\n";

	// Initialize OpenGL settings as we want
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	glutIdleFunc(glutIdle_callBack);
	glutReshapeFunc(glutReshape_callBack);
	glutDisplayFunc(glutDisplay_callBack);

	glutKeyboardFunc(glutKeyboard_callBack);
	glutKeyboardUpFunc(glutKeyboardUp_callBack);
	glutSpecialFunc(glutSpecial_callBack);
	glutSpecialUpFunc(glutSpecialUp_callBack);
	glutMouseFunc(glutMouse_callBack);
	glutMotionFunc(glutMotion_callBack);

	for (int i = 0; i < NUM_PARTS; i++){
		part[i] = BuildingBuilder::getBuildingPart(i, 1);
	}

	cx = 0;
	cy = 0;
	cz = 0;

	rx = 0;
	ry = 0;

	fvel = 0;
	svel = 0;

	h_up = false;
	h_down = false;
	h_right = false;
	h_left = false;

	r_up = false;
	r_down = false;
	r_right = false;
	r_left = false;

	shift = false;

	groundPlane = new PrimitivePlane();
	groundPlane->gen(2048, 2048, 36);

	//groundTexID = GLUtils::loadImage("../res/textures/building_cube.bmp");

	simpleTexShader = GLUtils::compileShader("../res/shaders/post/simpleShaderTex_vert.glsl", "../res/shaders/post/simpleShaderTex_frag.glsl");
	showDepthShader = GLUtils::compileShader("../res/shaders/post/showDepthShader_vert.glsl", "../res/shaders/post/showDepthShader_frag.glsl");
	showNormShader = GLUtils::compileShader("../res/shaders/post/showNormShader_vert.glsl", "../res/shaders/post/showNormShader_frag.glsl");
	lightingPass = GLUtils::compileShader("../res/shaders/post/deffered_lighting_pass_vert.glsl", "../res/shaders/post/deffered_lighting_pass_frag.glsl");
	alphaTexShader = GLUtils::compileShader("../res/shaders/post/alphaShaderTex_vert.glsl", "../res/shaders/post/alphaShaderTex_frag.glsl");

	groundTexShader = GLUtils::compileShader("../res/shaders/effects/ground_vert.glsl", "../res/shaders/effects/ground_frag.glsl");

	skyShader = GLUtils::compileShader("../res/shaders/sky/simpleShaderTex_vert.glsl", "../res/shaders/sky/simpleShaderTex_frag.glsl");

	//box = new PrimitiveCube();
	//box->gen();

	clouds = new CubeClouds();
	clouds->gen();

	//boxTexID = GLUtils::loadImage("../res/textures/square_green.bmp");
	//boxTexShader = GLUtils::compileShader("../res/shaders/simpleShaderTex_vert.glsl", "../res/shaders/simpleShaderTex_frag.glsl");
	boxTexShader = GLUtils::compileShader("../res/shaders/effects/fog_tex_vert.glsl", "../res/shaders/effects/fog_tex_frag.glsl");
	boxTexPulseShader = GLUtils::compileShader("../res/shaders/effects/pulse_fog_tex_vert.glsl", "../res/shaders/effects/pulse_fog_tex_frag.glsl");
	carShader = GLUtils::compileShader("../res/shaders/effects/car_fog_tex_vert.glsl", "../res/shaders/effects/car_fog_tex_frag.glsl");

	groundTex = GLUtils::loadImage("../res/textures/ground_road.bmp");
	groundTex2 = GLUtils::loadImage("../res/textures/ground_building.bmp");
	buildingTex[0] = GLUtils::loadImage("../res/textures/building_cube.bmp");
	buildingTex[1] = GLUtils::loadImage("../res/textures/building_wall.bmp");
	buildingTex[2] = GLUtils::loadImage("../res/textures/building_decal.bmp");
	carTex = GLUtils::loadImage("../res/textures/car.bmp");

	skySphere = new TexturedSphere();
	skySphere->gen(50, 20);
	skyTex = GLUtils::loadImage("../res/textures/comp_big.bmp");

	quad_m = new ScreenQuad();
	quad_tl = new ScreenQuad();
	quad_tr = new ScreenQuad();
	quad_bl = new ScreenQuad();
	quad_br = new ScreenQuad();

	GLenum formats[4] = { GL_RGBA8, GL_RGBA8, GL_RGB16F, GL_RGB16F};

	gb = new GBuffer(_w, _h, 4, formats, true); // color, bloom, position, normals
	fb_bloom = new Framebuffer(_w, _h);

	fb_clouds = new Framebuffer(_w / 2, _h / 2);

	quad_m->gen(SCREENQUAD_ALL);
	quad_tl->gen(SCREENQUAD_TOPLEFT);
	quad_tr->gen(SCREENQUAD_TOPRIGHT);
	quad_bl->gen(SCREENQUAD_BOTTOMLEFT);
	quad_br->gen(SCREENQUAD_BOTTOMRIGHT);

	blur = new GLBlur(fb_bloom);

	time = 0;

	show_color = false;
	show_depth = false;
	show_bloom = false;
	show_norm = false;

	toggle_clouds = true;
	toggle_bloom = true;
	toggle_lights = true;
	carCam = false;

	znear = .05f;
	zfar = 500.0f;

	sphere = new PrimitiveSphere();
	sphere->gen(1.05, 12);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	first = true;

	buildingPolygons = 0;

	blockSize = 42;

	car = new Car();
	car->build(1);

	numCars = 0;

	// add car
	for (int i = 0; i < 128; i++){
		cars[numCars++] = CarPath(GsVec(int(i / 20)*blockSize, (i % 20)*blockSize, 30 + (rand() % 10)), 0, 1);
	}

	instanceBuildings();

	boids = new Boid*[NUM_BOIDS];
	for (int n = 0; n < NUM_BOIDS; n++){
		int x = -zfar / 2 + (rand() % (int)(zfar + 20 + 1));
		int y = -zfar / 2 + (rand() % (int)(zfar + 1));
		int z = -5 + (rand() % (int)(5 + 5 + 1));

		boids[n] = new Boid(n, x, y, 125 + z);
	}
	cube = new PrimitiveCube();
	cube->gen();
}
void AppWindow::glutReshape ( int w, int h ) {
	// Define that OpenGL should use the whole window for rendering
	glViewport(0, 0, w, h);

	gb->clean();


	GLenum formats[4] = { GL_RGBA8, GL_RGBA8, GL_RGB16F, GL_RGB16F };
	gb = new GBuffer(w, h, 4, formats, true);

	blur->clean();

	fb_bloom->clean();
	fb_bloom = new Framebuffer(w, h);

	fb_clouds->clean();
	fb_clouds = new Framebuffer(w / 2, h / 2);

	blur = new GLBlur(fb_bloom);

	_w=w; _h=h;
}
void AppWindow::glutKeyboard(unsigned char key, int x, int y){
	switch ( key ) {
		case 27:
			exit(0);
		case '1':
			show_color = !show_color;
			break;
		case '2':
			show_depth = !show_depth;
			break;
		case '3':
			show_bloom = !show_bloom;
			break;
		case '4':
			show_norm = !show_norm;
			break;
		case '0':
			toggle_clouds = !toggle_clouds;
			break;
		case 'b':
			toggle_bloom = !toggle_bloom;
			break;
		case 'l':
			toggle_lights = !toggle_lights;
			break;
		case 'w':
			h_up = true;
			break;
		case 's':
			h_down = true;
			break;
		case 'a':
			h_left = true;
			break;
		case 'd':
			h_right = true;
			break;
		case ']':
			if (numCars + 16 <= MAX_CARS){
				for (int i = 0; i < 16; i++){
					cars[numCars++] = CarPath(GsVec(int((rand() % 400) / 20)*blockSize, ((rand()) % 20)*blockSize, 30 + (rand() % 10)), 0, 1);
				}
			}
			break;
		case '[':
			numCars -= 16;
			if (numCars < 0) numCars = 0;

			break;
		case ' ':
			carCam = !carCam;
			break;
	}
}
void AppWindow::glutKeyboardUp(unsigned char key, int x, int y){
	switch (key){
		case 'w':
			h_up = false;
			break;
		case 's':
			h_down = false;
			break;
		case 'a':
			h_left = false;
			break;
		case 'd':
			h_right = false;
			break;
	}
}
void AppWindow::glutSpecial(int key, int x, int y){
	switch(key){
		case GLUT_KEY_LEFT:
			r_left = true;
			break;
		case GLUT_KEY_RIGHT:
			r_right = true;
			break;
		case GLUT_KEY_UP:
			r_up = true;
			break;
		case GLUT_KEY_DOWN:
			r_down = true;
			break;
		case GLUT_KEY_SHIFT_L:
			shift = true;
			break;
		default:
			return;
	}
}
void AppWindow::glutSpecialUp(int key, int x, int y){
	switch(key){
		case GLUT_KEY_LEFT:
			r_left = false;
			break;
		case GLUT_KEY_RIGHT:
			r_right = false;
			break;
		case GLUT_KEY_UP:
			r_up = false;
			break;
		case GLUT_KEY_DOWN:
			r_down = false;
			break;
		case GLUT_KEY_SHIFT_L:
			shift = false;
			break;
		default:
			return;
	}
}
void AppWindow::glutMouse(int b, int s, int x, int y){
	mx = x;
	my = y;
}
void AppWindow::glutMotion(int x, int y){
	//ry = ((float)x)/50.f;
	//rx = ((float)y)/50.f;
	
	rx -= ((float)(mx - x))/200.f;
	ry -= ((float)(my - y))/200.f;

	mx = x;
	my = y;

	//glutPostRedisplay();
}

void AppWindow::run(){
	glutMainLoop();
}

// here we will redraw the scene according to the current state of the application.
void AppWindow::glutDisplay() {
	//Set up camera

	GsMat campos;
	GsMat sprx, spry, sprfix1, sprfix2;
	sprx.roty(ry);
	spry.rotz(rx);
	sprfix1.rotx(-PI_F / 2.f);
	sprfix2.roty(-PI_F / 2.f);

	campos.translation(cx, cy, cz);

	float fovy = 45.0f, aspect = ((float)_w / (float)_h);
	proj.perspective(fovy, aspect, znear, zfar);  // scene projection

	camera = campos * (spry * sprx) * (sprfix1 * sprfix2);  // camera location and orientation

	clouds->ox = -cy + time + 256.0f;
	clouds->oy = -cz - 60.f;
	clouds->oz = cx + 256.0f;

	clouds->ar = aspect;

	clouds->xRot = rx;
	clouds->yRot = ry;

	// build & instance buildings

	if (abs(cx - icX) + abs(cy - icY) > 40 || abs(rx - irx) > 0.4){
		clearInstancedBuildings();
		instanceBuildings();
	}

	// clear main gbuffer
	glBindFramebuffer(GL_FRAMEBUFFER, gb->fbId);
	glViewport(0, 0, _w, _h);
	

	GLenum drawBuffers[] = {
		GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3
	};
	glDrawBuffers(4, drawBuffers);

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawBuildings();

	// undo bind of gb

	GLenum drawBuffers2[] = {
		GL_COLOR_ATTACHMENT0
	};
	glDrawBuffers(1, drawBuffers2);

	glDisable(GL_DEPTH_TEST);

	// Copy gbuffer's bloom to the bloom framebuffer (there must be a way to skip this...)
	glClearColor(0, 0, 0, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, fb_bloom->fbId);
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, fb_bloom->w, fb_bloom->h);
	{
		glUseProgram(simpleTexShader->id);

		int loc_tex0 = glGetUniformLocation(simpleTexShader->id, "Texture0");

		glUniform1i(loc_tex0, 0); // refer uniform "Tex1" to texture unit 0 
		glActiveTexture(GL_TEXTURE0); // set texture unit 0 active 
		glBindTexture(GL_TEXTURE_2D, gb->texId[1]);

		quad_m->draw();
		
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, _w, _h);

	// Clear the rendering window
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	glDisable(GL_CULL_FACE);
	// draw sky
	{
		glUseProgram(skyShader->id);

		int loc_tex0 = glGetUniformLocation(skyShader->id, "Texture0");
		int loc_mat = glGetUniformLocation(skyShader->id, "mat");

		glUniform1i(loc_tex0, 0); // refer uniform "Tex1" to texture unit 0 
		glActiveTexture(GL_TEXTURE0); // set texture unit 0 active 
		glBindTexture(GL_TEXTURE_2D, skyTex);

		GsMat rot;
		rot.rotx(.3f);

		glUniformMatrix4fv(loc_mat, 1, GL_FALSE, ((rot * (spry * sprx) * (sprfix1 * sprfix2)) * proj).e);

		skySphere->draw();
	}
	glEnable(GL_CULL_FACE);
	/*
	{
		glUseProgram(simpleTexShader->id);

		int loc_tex0 = glGetUniformLocation(simpleTexShader->id, "Texture0");

		glUniform1i(loc_tex0, 0); // refer uniform "Tex1" to texture unit 0 
		glActiveTexture(GL_TEXTURE0); // set texture unit 0 active 
		glBindTexture(GL_TEXTURE_2D, skyTex);

		quad_m->draw();
	}
	*/

	// draw scene
	/*
	{
		glUseProgram(simpleTexShader->id);

		int loc_tex0 = glGetUniformLocation(simpleTexShader->id, "Texture0");

		glUniform1i(loc_tex0, 0); // refer uniform "Tex1" to texture unit 0 
		glActiveTexture(GL_TEXTURE0); // set texture unit 0 active 
		glBindTexture(GL_TEXTURE_2D, gb->texId[0]);

		quad_m->draw();
	}
	*/

	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// ambient lighting:
	{
		glUseProgram(alphaTexShader->id);

		int loc_tex0 = glGetUniformLocation(alphaTexShader->id, "Texture0");
		int loc_vColor = glGetUniformLocation(alphaTexShader->id, "vColor");

		glUniform1i(loc_tex0, 0); // refer uniform "Tex1" to texture unit 0 
		glActiveTexture(GL_TEXTURE0); // set texture unit 0 active 
		glBindTexture(GL_TEXTURE_2D, gb->texId[0]);

		glUniform4f(loc_vColor, .2f, .2f, .2f, 1);

		quad_m->draw();

	}
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glCullFace(GL_BACK);
	if (toggle_lights){
		glUseProgram(lightingPass->id);

		int loc_m_color = glGetUniformLocation(lightingPass->id, "m_color");
		int loc_m_pos = glGetUniformLocation(lightingPass->id, "m_pos");
		int loc_m_norm = glGetUniformLocation(lightingPass->id, "m_norm");

		int loc_lightPos = glGetUniformLocation(lightingPass->id, "v_lightPos");
		int loc_lightColor = glGetUniformLocation(lightingPass->id, "v_lightColor");

		//int loc_tr = glGetUniformLocation(lightingPass->id, "vTransf");
		int loc_cam = glGetUniformLocation(lightingPass->id, "vCam");
		int loc_pr = glGetUniformLocation(lightingPass->id, "vProj");

		GsMat proj2;
		proj2.perspective(fovy, aspect, znear, zfar * 2.f); // make sure far culling doesn't ruin the lighting

		glUniformMatrix4fv(loc_cam, 1, GL_FALSE, camera.e);
		glUniformMatrix4fv(loc_pr, 1, GL_FALSE, proj2.e);

		glUniform1i(loc_m_color, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gb->texId[0]);

		glUniform1i(loc_m_pos, 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gb->texId[2]);

		glUniform1i(loc_m_norm, 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gb->texId[3]);


		// camera light
			
		int numLights = 70;

		float* lightPositions = new float[numLights * 4 + numCars * 4];
		float* lightColors = new float[numLights * 4 + numCars * 4];

		lightPositions[0] = -cx;
		lightPositions[1] = -cy;
		lightPositions[2] = -cz;
		lightPositions[3] = 40.f;

		lightColors[0] = 1;
		lightColors[1] = 1;
		lightColors[2] = 1;
		lightColors[3] = 1;


		int li = 1;
		for (int i = 1; i < numLights; i++){
			//double time = 0;

			GsVec lightPos = GsVec(
				(cos(time * 0.05f + i * 2.001f)) * 1000.f + i/100.0,
				(sin(time * 0.03f + i * 8.4f))* 100.f,
				10.0 + sin(time * 0.043f + i * 8.4f)*10.f);

			float lightRadius = 50.0f + cos(i + time*0.02f)*4.f;

			lightPositions[li * 4 + 0] = lightPos.x - cx;
			lightPositions[li * 4 + 1] = lightPos.y - cy;
			lightPositions[li * 4 + 2] = lightPos.z;
			lightPositions[li * 4 + 3] = lightRadius;

			lightColors[li * 4 + 0] = cos(i)*.5f + 1;
			lightColors[li * 4 + 1] = cos(i + PI_F / 3.)*.5f + 1;
			lightColors[li * 4 + 2] = cos(i + 2.0 * PI_F / 3.)*.5f + 1;
			lightColors[li * 4 + 3] = 1;

			li++;
		}
		for (int i = 0; i < numCars; i++){
			lightPositions[li * 4 + 0] = cars[i].pos.x;
			lightPositions[li * 4 + 1] = cars[i].pos.y;
			lightPositions[li * 4 + 2] = cars[i].pos.z;
			lightPositions[li * 4 + 3] = 28;

			lightColors[li * 4 + 0] = 1.2f;
			lightColors[li * 4 + 1] = .3f;
			lightColors[li * 4 + 2] = .3f;
			lightColors[li * 4 + 3] = 1;

			li++;
		}

		glBindVertexArray(sphere->va[0]);
		
		glBindBuffer(GL_ARRAY_BUFFER, sphere->buf[2]);
		glBufferData(GL_ARRAY_BUFFER, li * 4 * sizeof(float), lightPositions, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, sphere->buf[3]);
		glBufferData(GL_ARRAY_BUFFER, li * 4 * sizeof(float), lightColors, GL_DYNAMIC_DRAW);

		delete lightPositions;
		delete lightColors;
		
		if (first){
			glBindBuffer(GL_ARRAY_BUFFER, sphere->buf[2]);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);
			glVertexAttribDivisor(2, 1);

			glBindBuffer(GL_ARRAY_BUFFER, sphere->buf[3]);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, 0);
			glVertexAttribDivisor(3, 1);
		}
		
		glDrawElementsInstanced(GL_TRIANGLES, sphere->numIndices, GL_UNSIGNED_INT, sphere->indices, li);

		//glPointSize(3);

		//glDrawArrays(GL_POINTS, 0, numVertices);

		
		

		glBindVertexArray(0);
	}
	
	//glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// draw clouds
	if (toggle_clouds){
		glBindFramebuffer(GL_FRAMEBUFFER, fb_clouds->fbId);
		glViewport(0, 0, fb_clouds->w, fb_clouds->h);

		// uncomment this to have cloud windows
		//glClearColor(1, 1, 1, 1);

		glClear(GL_COLOR_BUFFER_BIT);

		// ^^^ uncomment this to have cloud windows
		//glClearColor(0, 0, 0, 0);

		clouds->draw(gb->texId_depth, znear, zfar);
	}

	// then add bloom on top :D
	if (toggle_bloom){
		// draw clouds to bloom layer :D
		if (toggle_clouds){
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);

			glBindFramebuffer(GL_FRAMEBUFFER, fb_bloom->fbId);
			glViewport(0, 0, fb_bloom->w, fb_bloom->h);

			glUseProgram(simpleTexShader->id);

			int loc_tex0 = glGetUniformLocation(simpleTexShader->id, "Texture0");

			glUniform1i(loc_tex0, 0); // refer uniform "Tex1" to texture unit 0 
			glActiveTexture(GL_TEXTURE0); // set texture unit 0 active 
			glBindTexture(GL_TEXTURE_2D, fb_clouds->texId);

			quad_m->draw();

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		blur->blur();
		glViewport(0, 0, _w, _h);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		blur->drawBloom(-0.0f, 1.5f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}else{
		// draw clouds directly to backbuffer
		if (toggle_clouds){
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, _w, _h);

			glUseProgram(simpleTexShader->id);

			int loc_tex0 = glGetUniformLocation(simpleTexShader->id, "Texture0");

			glUniform1i(loc_tex0, 0); // refer uniform "Tex1" to texture unit 0 
			glActiveTexture(GL_TEXTURE0); // set texture unit 0 active 
			glBindTexture(GL_TEXTURE_2D, fb_clouds->texId);

			quad_m->draw();

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
	}

	
	glDisable(GL_BLEND);
	// draw framebuffer's color
	if (show_color) {
		glUseProgram(simpleTexShader->id);

		int loc_tex0 = glGetUniformLocation(simpleTexShader->id, "Texture0");

		glUniform1i(loc_tex0, 0); // refer uniform "Tex1" to texture unit 0 
		glActiveTexture(GL_TEXTURE0); // set texture unit 0 active 
		glBindTexture(GL_TEXTURE_2D, gb->texId[0]);

		quad_tl->draw();

	}

	
	// draw framebuffer's depth
	if (show_depth) {
		glUseProgram(showDepthShader->id);

		int loc_tex0 = glGetUniformLocation(showDepthShader->id, "Texture0");

		glUniform1i(loc_tex0, 0); // refer uniform "Tex1" to texture unit 0 
		glActiveTexture(GL_TEXTURE0); // set texture unit 0 active 
		glBindTexture(GL_TEXTURE_2D, gb->texId_depth);

		quad_tr->draw();
	}

	if (show_bloom) {
		glUseProgram(simpleTexShader->id);

		int loc_tex0 = glGetUniformLocation(simpleTexShader->id, "Texture0");

		glUniform1i(loc_tex0, 0); // refer uniform "Tex1" to texture unit 0 
		glActiveTexture(GL_TEXTURE0); // set texture unit 0 active 
		//glBindTexture(GL_TEXTURE_2D, gb->texId[1]);
		glBindTexture(GL_TEXTURE_2D, fb_bloom->texId);

		quad_bl->draw();
	}
	if (show_norm) {
		glUseProgram(showNormShader->id);

		int loc_tex0 = glGetUniformLocation(showNormShader->id, "Texture0");

		glUniform1i(loc_tex0, 0); // refer uniform "Tex1" to texture unit 0 
		glActiveTexture(GL_TEXTURE0); // set texture unit 0 active 
		//glBindTexture(GL_TEXTURE_2D, gb->texId[1]);
		glBindTexture(GL_TEXTURE_2D, gb->texId[3]);

		quad_br->draw();
	}
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	
	
	// end
	glFlush();
	glutSwapBuffers();

	first = false;
}
int AppWindow::addBuilding(int x, int y){
	srand(x*(13 * 13 - 1) + y);
	/*
	int polygons = 0;

	int height = abs(x + y)%10 + 1;

	float currAltitude = 0;

	int partNum = rand() % NUM_PARTS;
	for (int i = 0; i < height; i++){
		if (rand() % 2 == 0){
			partNum = rand() % NUM_PARTS;
		}
		//partNum = 4;

		GsVec pos;
		pos.set(x, y, currAltitude);

		part[partNum]->addInstance(pos, (abs(x / 40) + (abs(y / 40))) % 100);

		polygons += part[partNum]->getPolygonCount();

		currAltitude += part[partNum]->getHeight();
	}
	*/

	const int basePeices[] = { 1, 8 };
	const int numBasePeices = 2;

	const int midPices[] = { 0, 1, 4, 10 };
	const int numMidPeices = 4;

	const int preTopPeices[] = { 1, 2, 5, 6, 7, 12 };
	const int numPreTopPeices = 5;

	const int topPeices[] = { 1, 2, 5, 6, 3 };
	const int numTopPeices = 5;

	int polygons = 0;
	int height = 0;
	float currAltitude = 0;

	if (rand() % 10 == 0){
		// type 1

		// pick a base:
		{
			int partNum = basePeices[rand() % numBasePeices];
			int n = rand() % 3 + 1;
			for (int i = 0; i < n; i++){
				part[partNum]->addInstance(GsVec(x, y, currAltitude), (abs(x / blockSize) + (abs(y / blockSize))) % 100);
				currAltitude += part[partNum]->getHeight();
				polygons += part[partNum]->getPolygonCount();
				height++;
			}
		}
		// pick a middle:
		{
			int m = rand() % 2 + 1;
			for (int h = 0; h < m; h++){
				int partNum = midPices[rand() % numMidPeices];
				int n = rand() % 2 + 1;
				for (int i = 0; i < n; i++){
					part[partNum]->addInstance(GsVec(x, y, currAltitude), (abs(x / blockSize) + (abs(y / blockSize))) % 100);
					currAltitude += part[partNum]->getHeight();
					polygons += part[partNum]->getPolygonCount();
					height++;
				}
			}
		}
		// pick a pre top
		{

			if (rand() % 4 != 0){
				int partNum = preTopPeices[rand() % numPreTopPeices];
				part[partNum]->addInstance(GsVec(x, y, currAltitude), (abs(x / blockSize) + (abs(y / blockSize))) % 100);
				currAltitude += part[partNum]->getHeight();
				polygons += part[partNum]->getPolygonCount();
				height++;
			}
		}
		// pick a top:
		{
			
			if (rand() % 4 != 0){
				int partNum = topPeices[rand() % numTopPeices];
				part[partNum]->addInstance(GsVec(x, y, currAltitude), (abs(x / blockSize) + (abs(y / blockSize))) % 100);
				currAltitude += part[partNum]->getHeight();
				polygons += part[partNum]->getPolygonCount();
				height++;
			}
		}
	}else if(rand() % 2 == 0){
		// type 2

		// pick a middle:
		{
			int m = rand() % 2 + 1;
			for (int h = 0; h < m; h++){
				int partNum = midPices[rand() % numMidPeices];
				int n = rand() % 2 + 1;
				for (int i = 0; i < n; i++){
					part[partNum]->addInstance(GsVec(x, y, currAltitude), (abs(x / blockSize) + (abs(y / blockSize))) % 100);
					currAltitude += part[partNum]->getHeight();
					polygons += part[partNum]->getPolygonCount();
					height++;
				}
			}
		}
		// pick a pre top
		{

			if (rand() % 4 != 0){
				int partNum = preTopPeices[rand() % numPreTopPeices];
				part[partNum]->addInstance(GsVec(x, y, currAltitude), (abs(x / blockSize) + (abs(y / blockSize))) % 100);
				currAltitude += part[partNum]->getHeight();
				polygons += part[partNum]->getPolygonCount();
				height++;
			}
		}
		// pick a top:
		{

			if (rand() % 4 != 0){
				int partNum = topPeices[rand() % numTopPeices];
				part[partNum]->addInstance(GsVec(x, y, currAltitude), (abs(x / blockSize) + (abs(y / blockSize))) % 100);
				currAltitude += part[partNum]->getHeight();
				polygons += part[partNum]->getPolygonCount();
				height++;
			}
		}
	}





	return polygons;

}
void AppWindow::drawBuildingParts(){
	GsMat id;

	{
		glUseProgram(boxTexPulseShader->id);

		int loc_tr = glGetUniformLocation(boxTexPulseShader->id, "vTransf");
		int loc_cam = glGetUniformLocation(boxTexPulseShader->id, "vCam");
		int loc_pr = glGetUniformLocation(boxTexPulseShader->id, "vProj");
		int loc_tex0 = glGetUniformLocation(boxTexPulseShader->id, "Texture0");
		int loc_t = glGetUniformLocation(boxTexPulseShader->id, "t");
		int loc_fogStart = glGetUniformLocation(boxTexPulseShader->id, "fogStart");
		int loc_fogDist = glGetUniformLocation(boxTexPulseShader->id, "fogDist");

		glUniformMatrix4fv(loc_cam, 1, GL_FALSE, camera.e);
		glUniformMatrix4fv(loc_pr, 1, GL_FALSE, proj.e);

		glUniform1f(loc_t, (float)time);

		glUniform1i(loc_tex0, 0); // refer uniform "Tex1" to texture unit 0 
		glActiveTexture(GL_TEXTURE0); // set texture unit 0 active 

		glBindTexture(GL_TEXTURE_2D, buildingTex[0]);

		glUniform1f(loc_fogStart, zfar / 2.0);
		glUniform1f(loc_fogDist, zfar / 2.0);


		glUniformMatrix4fv(loc_tr, 1, GL_FALSE, id.e);

		//glUniform1f(loc_t, (float)time);

		for (int i = 0; i < NUM_PARTS; i++){
			part[i]->drawCubeLayerInstanced();
		}
	}

	{
		glUseProgram(boxTexShader->id);

		int loc_tr = glGetUniformLocation(boxTexShader->id, "vTransf");
		int loc_cam = glGetUniformLocation(boxTexShader->id, "vCam");
		int loc_pr = glGetUniformLocation(boxTexShader->id, "vProj");
		int loc_tex0 = glGetUniformLocation(boxTexShader->id, "Texture0");
		int loc_fogStart = glGetUniformLocation(boxTexShader->id, "fogStart");
		int loc_fogDist = glGetUniformLocation(boxTexShader->id, "fogDist");

		glUniformMatrix4fv(loc_cam, 1, GL_FALSE, camera.e);
		glUniformMatrix4fv(loc_pr, 1, GL_FALSE, proj.e);

		//glUniform1f(loc_t, (float)time);

		glUniform1i(loc_tex0, 0); // refer uniform "Tex1" to texture unit 0 
		glActiveTexture(GL_TEXTURE0); // set texture unit 0 active 

		glBindTexture(GL_TEXTURE_2D, buildingTex[1]);

		glUniform1f(loc_fogStart, zfar / 2.0);
		glUniform1f(loc_fogDist, zfar / 2.0);

		glUniformMatrix4fv(loc_tr, 1, GL_FALSE, id.e);

		for (int i = 0; i < NUM_PARTS; i++){
			part[i]->drawWallLayerInstanced();
		}

		glBindTexture(GL_TEXTURE_2D, buildingTex[2]);

		glUniformMatrix4fv(loc_tr, 1, GL_FALSE, id.e);

		for (int i = 0; i < NUM_PARTS; i++){
			part[i]->drawDecalLayerInstanced();
		}

	}


	// cars
	{
		glUseProgram(carShader->id);

		int loc_tr = glGetUniformLocation(carShader->id, "vTransf");
		int loc_cam = glGetUniformLocation(carShader->id, "vCam");
		int loc_pr = glGetUniformLocation(carShader->id, "vProj");
		int loc_tex0 = glGetUniformLocation(carShader->id, "Texture0");
		int loc_t = glGetUniformLocation(carShader->id, "t");
		int loc_fogStart = glGetUniformLocation(carShader->id, "fogStart");
		int loc_fogDist = glGetUniformLocation(carShader->id, "fogDist");

		glUniformMatrix4fv(loc_cam, 1, GL_FALSE, camera.e);
		glUniformMatrix4fv(loc_pr, 1, GL_FALSE, proj.e);

		glUniform1f(loc_t, (float)time);

		glUniform1i(loc_tex0, 0); // refer uniform "Tex1" to texture unit 0 
		glActiveTexture(GL_TEXTURE0); // set texture unit 0 active 

		glBindTexture(GL_TEXTURE_2D, carTex);

		glUniform1f(loc_fogStart, zfar / 2.0);
		glUniform1f(loc_fogDist, zfar / 2.0);

		GsMat carCenter;
		carCenter.translation(-1, 0, 0);

		glUniformMatrix4fv(loc_tr, 1, GL_FALSE, carCenter.e);

		//glUniform1f(loc_t, (float)time);

		car->clearInstances();

		for (int i = 0; i < numCars; i++){
			/*
			GsMat carPos, carRot, carRot2, carCenter;
			carPos.translation(cars[i].pos);

			carCenter.translation(-1, 0, 0);

			//carRot.lookat(cars[i].dir, GsVec(0, 0, 0), GsVec(0, 1, 0));

			//carRot2.roty(PI_F / 2.0f);

			carRot.rotz(-atan2(cars[i].dir.x, cars[i].dir.y));

			carRot2.rotx(atan2(cars[i].dir.z, sqrt((cars[i].dir.x * cars[i].dir.x) + (cars[i].dir.y * cars[i].dir.y))));

			glUniformMatrix4fv(loc_tr, 1, GL_FALSE, (carCenter * carRot2 * carRot * carPos).e);

			glBindTexture(GL_TEXTURE_2D, carTex);

			car->draw();
			*/

			float car_rx = -atan2(cars[i].dir.x, cars[i].dir.y);

			float car_ry = -atan2(cars[i].dir.z, sqrt((cars[i].dir.x * cars[i].dir.x) + (cars[i].dir.y * cars[i].dir.y)));

			car->addInstance(cars[i].pos, i, GsVec(car_rx, car_ry, 0));
		}

		car->drawInstances();
	}
}
void AppWindow::instanceBuildings(){
	icX = cx;
	icY = cy;
	irx = rx;

	int polygons = 0;
	const int distance = zfar / blockSize + 1;

	int xmin = int(-cx / blockSize) * blockSize - distance * blockSize;
	int xmax = xmin + 2 * distance * blockSize;

	int ymin = int(-cy / blockSize) * blockSize - distance * blockSize;
	int ymax = ymin + 2 * distance * blockSize;

	GsMat forward;
	forward.rotz(rx);

	const int offX = (blockSize - 20) / 2;
	const int offY = (blockSize - 20) / 2;

	for (int y = ymin; y <= ymax; y += blockSize){
		for (int x = xmin; x <= xmax; x += blockSize){
			GsVec p = GsVec(x + cx, y + cy, 0);
			
			p = p*forward;

			if (p.x <= blockSize*distance / 8 && abs(p.x) + blockSize > abs(p.y) / 1.5){
				polygons += addBuilding(x + offX, y + offY);
			}
		}
	}

	buildingPolygons = polygons;
}
void AppWindow::clearInstancedBuildings(){
	for (int i = 0; i < NUM_PARTS; i++){
		part[i]->clearInstances();
	}
}
void AppWindow::drawBuildings(){

	{
		glUseProgram(groundTexShader->id);

		int loc_tr = glGetUniformLocation(groundTexShader->id, "vTransf");
		int loc_cam = glGetUniformLocation(groundTexShader->id, "vCam");
		int loc_pr = glGetUniformLocation(groundTexShader->id, "vProj");
		int loc_tex0 = glGetUniformLocation(groundTexShader->id, "Texture0");
		int loc_tex1 = glGetUniformLocation(groundTexShader->id, "Texture1");
		int loc_fogStart = glGetUniformLocation(groundTexShader->id, "fogStart");
		int loc_fogDist = glGetUniformLocation(groundTexShader->id, "fogDist");

		int loc_t = glGetUniformLocation(groundTexShader->id, "t");

		int loc_blockSize = glGetUniformLocation(groundTexShader->id, "blockSize");
		int loc_buildingSize = glGetUniformLocation(groundTexShader->id, "buildingSize");

		GsMat floorMat;
		floorMat.translation(-1024 - (int)cx, -1024 - (int)cy, -1.0f);

		glUniformMatrix4fv(loc_tr, 1, GL_FALSE, floorMat.e);
		glUniformMatrix4fv(loc_cam, 1, GL_FALSE, camera.e);
		glUniformMatrix4fv(loc_pr, 1, GL_FALSE, proj.e);

		glUniform1i(loc_tex0, 0); // refer uniform "Tex1" to texture unit 0 
		glActiveTexture(GL_TEXTURE0); // set texture unit 0 active 
		glBindTexture(GL_TEXTURE_2D, groundTex);

		glUniform1i(loc_tex0, 1); // refer uniform "Tex1" to texture unit 0 
		glActiveTexture(GL_TEXTURE1); // set texture unit 0 active 
		glBindTexture(GL_TEXTURE_2D, groundTex2);

		glUniform1f(loc_fogStart, zfar / 2.0);
		glUniform1f(loc_fogDist, zfar / 2.0);

		glUniform1i(loc_t, time);

		glUniform1i(loc_blockSize, blockSize);
		glUniform1i(loc_buildingSize, 20);

		glDisable(GL_CULL_FACE);

		groundPlane->draw();

		glEnable(GL_CULL_FACE);
	}
	
	//printf("numParts: %i\n", numParts);

	drawBuildingParts();

	// DRAW BOIDS
	{
		glUseProgram(boxTexPulseShader->id);

		int loc_tr = glGetUniformLocation(boxTexPulseShader->id, "vTransf");
		int loc_cam = glGetUniformLocation(boxTexPulseShader->id, "vCam");
		int loc_pr = glGetUniformLocation(boxTexPulseShader->id, "vProj");
		int loc_tex0 = glGetUniformLocation(boxTexPulseShader->id, "Texture0");
		int loc_fogStart = glGetUniformLocation(boxTexPulseShader->id, "fogStart");
		int loc_fogDist = glGetUniformLocation(boxTexPulseShader->id, "fogDist");
		int loc_t = glGetUniformLocation(boxTexPulseShader->id, "t");

		glUniform1f(loc_t, (float)time);

		glDisable(GL_CULL_FACE);
		for (int n = 0; n < NUM_BOIDS; n++){
			GsVec pos = boids[n]->pos;

			//std::cout << "Sep: " << pos->x << ", " << pos->y << ", " << pos->z << std::endl;

			GsMat floorMat;
			floorMat.translation(pos.x, pos.y, pos.z);

			glUniformMatrix4fv(loc_tr, 1, GL_FALSE, floorMat.e);
			glUniformMatrix4fv(loc_cam, 1, GL_FALSE, camera.e);
			glUniformMatrix4fv(loc_pr, 1, GL_FALSE, proj.e);

			glUniform1i(loc_tex0, 0); // refer uniform "Tex1" to texture unit 0 
			glActiveTexture(GL_TEXTURE0); // set texture unit 0 active 
			glBindTexture(GL_TEXTURE_2D, buildingTex[0]);

			glUniform1f(loc_fogStart, zfar / 2.0);
			glUniform1f(loc_fogDist, zfar / 2.0);



			cube->draw();


		}
		glEnable(GL_CULL_FACE);
	}

}
void AppWindow::car_doFrame(CarPath &c, float diff){
	//int curBlockX = int((c.pos.x) / blockSize);
	//int curBlockY = int((c.pos.y) / blockSize);

	if (c.currentAction == ACTION_FORWARD){
		if (c.dirY == 0){
			c.pos.x += c.dirX*diff;
			c.pos.y = round(c.pos.y / blockSize)*blockSize;
		}else{
			c.pos.x = round(c.pos.x / blockSize)*blockSize;
			c.pos.y += c.dirY*diff;
		}
		c.distance += diff;

		c.dir.x = c.dirX;
		c.dir.y = c.dirY;

		int newBlockX = round((c.pos.x) / blockSize);
		int newBlockY = round((c.pos.y) / blockSize);

		if (c.distance >= blockSize){
			c.pos.x = newBlockX*blockSize;
			c.pos.y = newBlockY*blockSize;

			// orient towards center
			if (c.pos.x + cx > zfar){
				c.dirX = 1;
				c.dirY = 0;

				c.pos.x -= zfar * 2;
			}else if (c.pos.x + cx < -zfar){
				c.dirX = -1;
				c.dirY = 0;

				c.pos.x += zfar * 2;
			}else if (c.pos.y + cy > zfar){
				c.dirX = 0;
				c.dirY = 1;

				c.pos.y -= zfar * 2;
			}else if(c.pos.y + cy < -zfar){
				c.dirX = 0;
				c.dirY = -1;

				c.pos.y += zfar * 2;
			}
			else{
				// do random action

				int action = rand() % 3;
				if (action == 0){

					c.currentSpline.clear();

					GsVec direction = GsVec(c.dirX, c.dirY, 0);
					GsVec newDirection;
					// right turn...
					if (rand() % 2 == 0){
						newDirection.set(-c.dirY, c.dirX, 0);
					}
					else{ // left turn
						newDirection.set(c.dirY, -c.dirX, 0);
					}

					c.currentSpline.push_back(c.pos);

					c.currentSpline.push_back(c.pos + (direction)*blockSize);

					c.currentSpline.push_back(c.pos + (direction + newDirection)*blockSize);

					c.currentAction = ACTION_TURN;

					c.dirX = newDirection.x;
					c.dirY = newDirection.y;

					c.t = 0;
				}else if (action == 1){
					// altitude change!
					c.currentSpline.clear();

					float z = 30 + (rand() % 40);

					GsVec direction = GsVec(c.dirX, c.dirY, 0);

					GsVec offZ = GsVec(0, 0, z - c.pos.z);

					c.currentSpline.push_back(c.pos);

					c.currentSpline.push_back(c.pos + (direction)*blockSize);

					c.currentSpline.push_back(c.pos + (direction)*blockSize + offZ);

					c.currentSpline.push_back(c.pos + (direction)*blockSize * 2.0f + offZ);

					c.currentAction = ACTION_TURN;

					c.t = 0;

				}
				else{

				}
			}
			c.distance = 0;
		}

	}
	else if (c.currentAction == ACTION_TURN){
		c.pos = c.eval_bezier(c.t, c.currentSpline);

		c.dir = c.eval_bezier(c.t + .01, c.currentSpline) - c.pos;
		c.dir.normalize();

		c.t += diff / (20. * c.currentSpline.size());

		if (c.t >= 1.){
			c.pos.x = round((c.pos.x) / blockSize)*blockSize;
			c.pos.y = round((c.pos.y) / blockSize)*blockSize;

			c.currentAction = ACTION_FORWARD;
			c.distance = 0;
		}
	}

}


void AppWindow::glutIdle(){
	float diff = (float)frameDiff();

	// game physics
	if(h_up){
		fvel += diff * 0.1f;

		if (shift){
			fvel /= pow((1.0f - 1.0f / (20.0f)), diff);
		}

	}
	if(h_down){
		fvel -= diff * 0.1f;

		if (shift){
			fvel /= pow((1.0f - 1.0f / (20.0f)), diff);
		}
	}

	if(h_right){
		svel += diff * 0.1f;
		if (shift){
			svel /= pow((1.0f - 1.0f / (20.0f)), diff);
		}
	}
	if(h_left){
		svel -= diff * 0.1f;
		if (shift){
			svel /= pow((1.0f - 1.0f / (20.0f)), diff);
		}
	}

	
	if (abs(fvel) > 10){
		fvel *= pow((1.0f - 1.0f / (2.0f)), diff);
	}
	if (abs(svel) > 10){
		svel *= pow((1.0f - 1.0f / (2.0f)), diff);
	}

	fvel *= pow((1.0f - 1.0f / (20.0f)), diff);
	svel *= pow((1.0f - 1.0f / (20.0f)), diff);

	cx += cos(rx)*fvel * cos(ry)*diff;
	cy -= sin(rx)*fvel* cos(ry)*diff;
	
	cz += sin(ry)*fvel*diff;

	if (cz > 0){
		cz = 0;
	}

	cx += cos(rx + PI_F / 2.0f)*svel*diff;
	cy -= sin(rx + PI_F / 2.0f)*svel*diff;

	if (r_up){
		ry -= diff * .05f;
	}
	if (r_down){
		ry += diff * .05f;
	}
	if (r_left){
		rx -= diff * .05f;
	}
	if (r_right){
		rx += diff * .05f;
	}

	for (int i = 0; i < numCars; i++){
		//cars[i].doFrame(diff);
		car_doFrame(cars[i], diff);
	}

	if (carCam && numCars > 0){
		cx = -cars[0].pos.x + cars[0].dir.x*0.8f;
		cy = -cars[0].pos.y + cars[0].dir.y*0.8f;
		cz = -cars[0].pos.z - 2.2f + cars[0].dir.z*0.8f;

		rx = PI_F - atan2(cars[0].dir.y, cars[0].dir.x);

		ry = -atan2(cars[0].dir.z, sqrt((cars[0].dir.x * cars[0].dir.x) + (cars[0].dir.y * cars[0].dir.y)));
	}
	else{
		carCam = false;
	}


	time += diff/60.;

	for (int n = 0; n < NUM_BOIDS; n++){
		boids[n]->flock(boids, NUM_BOIDS, diff);
	}
	for (int n = 0; n < NUM_BOIDS; n++){
		boids[n]->apply(boids, NUM_BOIDS, diff);

		if (boids[n]->pos.x + cx > zfar){
			boids[n]->pos.x -= zfar * 2;
		}
		if (boids[n]->pos.y + cy > zfar){
			boids[n]->pos.y -= zfar * 2;
		}

		if (boids[n]->pos.x + cx < -zfar){
			boids[n]->pos.x += zfar * 2;
		}
		if (boids[n]->pos.y + cy < -zfar){
			boids[n]->pos.y += zfar * 2;
		}
	}
	
	//Sleep(16);

	printf("\rfps: %i\tPolygons: %i\tcars %i\t", getFPS(), buildingPolygons + numCars*12, numCars);
	
	glutPostRedisplay();
}
