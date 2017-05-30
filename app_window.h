#ifndef APP_WINDOW_H
#define APP_WINDOW_H

#include <GL/glew.h>
#include <freeglut.h>

#include <gsim/gs_color.h>
#include <gsim/gs_array.h>
#include <gsim/gs_vec.h>
#include <gsim/gs_image.h>
#include <rtools/window.h>
#include <rtools/utils.h>
#include <rtools/plane.h>
#include <rtools/cube.h>
#include <rtools/sphere.h>
#include <rtools/screenquad.h>
#include <rtools/framebuffer.h>
#include <rtools/gbuffer.h>
#include <rtools/clouds/clouds.h>
#include <rtools/blur.h>
#include <rtools/building/building_builder.h>
#include <rtools/sphere_tex.h>
#include <rtools/highway/car.h>
#include "carpaths.h"
#include <rtools/boids/boid.h>

#include <ctime>

class AppWindow : GLWindow{
private:
	virtual void glutDisplay();
	virtual void glutReshape(int w, int h);
	virtual void glutIdle();
	virtual void glutKeyboard(unsigned char key, int x, int y);
	virtual void glutKeyboardUp(unsigned char key, int x, int y);
	virtual void glutSpecial(int key, int x, int y);
	virtual void glutSpecialUp(int key, int x, int y);
	virtual void glutMouse(int b, int s, int x, int y);
    virtual void glutMotion(int x, int y);
public:
	AppWindow(const char* label, int x, int y, int w, int h);

	PrimitivePlane* groundPlane;

	GlProgram* groundTexShader;

	GlProgram* boxTexShader;
	GlProgram* boxTexPulseShader;
	GlProgram* carShader;

	GlProgram* simpleTexShader;
	GlProgram* showDepthShader;
	GlProgram* showNormShader;
	GlProgram* lightingPass;
	GlProgram* alphaTexShader;
	GlProgram* skyShader;
	
	CubeClouds* clouds;

	BuildingPart* part[NUM_PARTS];

	Car* car;

	GBuffer* gb;
	Framebuffer* fb_bloom;
	Framebuffer* fb_clouds;

	ScreenQuad* quad_m;
	ScreenQuad* quad_tl;
	ScreenQuad* quad_tr;
	ScreenQuad* quad_bl;
	ScreenQuad* quad_br;

	PrimitiveSphere* sphere;
	TexturedSphere* skySphere;
	int skyTex;

	GLBlur* blur;

	Boid** boids;
	PrimitiveCube* cube;

	bool first;

	int groundTex;
	int groundTex2;
	int buildingTex[3];
	int carTex;
	
	double time;

	int mx;
	int my;

	float cx;
	float cy;
	float cz;

	float rx;
	float ry;

	float fvel;
	float svel;

	bool h_up;
	bool h_down;

	bool h_right;
	bool h_left;

	bool r_up;
	bool r_down;

	bool r_right;
	bool r_left;

	bool shift;

	GsMat camera;
	GsMat proj;

	bool show_depth;
	bool show_color;
	bool show_bloom;
	bool show_norm;

	bool toggle_clouds;
	bool toggle_bloom;
	bool toggle_lights;

	bool carCam;
	

	float znear;
	float zfar;

	int blockSize;

	int numCars;
	CarPath cars[MAX_CARS];

	void run();

	void drawBuildings();

	float icX;
	float icY;
	float irx;

	int buildingPolygons;

	void instanceBuildings();
	void clearInstancedBuildings();
	int addBuilding(int x, int y);
	void drawBuildingParts();

	void car_doFrame(CarPath &c, float diff);
 };

#endif // APP_WINDOW_H
