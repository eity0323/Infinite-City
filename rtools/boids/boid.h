#pragma once

#include <gsim/gs_vec.h>
#include <cmath>

#define NUM_BOIDS 250

class Boid{
private:
	
	GsVec vel;
	GsVec dir;

	GsVec sep;
	GsVec ali;
	GsVec coh;

	bool avoid;
	float counter;

	int id;

public:
	GsVec pos;

	Boid(int _id);
	Boid(int _id, float posx, float posy, float posz);
	void flock(Boid** boids, int flocksize, float diff);
	void apply(Boid** boids, int flocksize, float diff);
	GsVec separation(Boid** boids, int flocksize);
	GsVec alignment(Boid** boids, int flocksize);
	GsVec cohesion(Boid** boids, int flocksize);
};