#include "boid.h"

Boid::Boid(int _id)
{
	pos = GsVec(0, 0, 100);
	vel = GsVec();
	id = _id;

	avoid = false;
	counter = rand() % 2500;

	sep = GsVec();
	ali = GsVec();
	coh = GsVec();
	dir = GsVec(1, 0, 0);
}

Boid::Boid(int _id, float posx, float posy, float posz)
{
	pos = GsVec(posx, posy, posz);
	vel = GsVec();
	id = _id;

	avoid = false;
	counter = rand() % 2500;

	sep = GsVec();
	ali = GsVec();
	coh = GsVec();
	dir = GsVec(1, 0, 0);
}

void Boid::flock(Boid** boids, int flocksize, float diff)
{
	diff = 1;
	//GsVec* sep = new GsVec();
	//GsVec* ali = new GsVec();
	//GsVec* coh = new GsVec();

	sep = separation(boids, flocksize);
	ali = alignment(boids, flocksize);
	coh = cohesion(boids, flocksize);
}
void Boid::apply(Boid** boids, int flocksize, float diff)
{
	diff = 1;

	if (avoid == false){
		vel += (sep + ali + coh) * 0.001f * diff + dir * .0005f;
		
	}else{
		vel += (sep + ali - coh) * 0.001f * diff + dir * .0005f;
	}
	vel += dir * .001f;

	pos += vel * diff;

	//(*vel) += GsVec((rand() % 1000) / 1000.f - .5f, (rand() % 1000) / 1000.f - .5f, (rand() % 1000) / 1000.f - .5f)*0.000000001f;

	dir += vel / 10.f;
	dir.normalize();

	vel *= 0.98* diff;

	vel.z += (100.0f - pos.z) / 10000.f;
	//pos->z = 100.0f;

	counter += diff;

	if (counter > 800.0){
		avoid = false;
	}

	if (counter > 2500.0f){
		vel += GsVec(float(gs_randomd()) - .5f, float(gs_randomd()) - .5f, float(gs_randomd()) - .5f)/10.0f;
		if (counter > 2500.0f + 15.0f){
			counter = rand()%300;

			if (rand() % 2 == 0){
				avoid = !avoid;
			}
		}
	}

}

GsVec Boid::separation(Boid** boids, int flocksize)
{
	GsVec c = GsVec();
	for (int n = 0; n < flocksize; n++){
		if (id != boids[n]->id){
			if ((boids[n]->pos - pos).norm() < 10.0f){
				c -= (boids[n]->pos - pos) / (boids[n]->pos - pos).norm();
			}
		}
	}


	c /= 8.;
	return c;
}

GsVec Boid::alignment(Boid** boids, int flocksize)
{
	GsVec c = GsVec();
	for (int n = 0; n < flocksize; n++){
		if (id != boids[n]->id){
			if (((boids[n]->pos) - pos).norm() < 100.0f){
				c += (vel - boids[n]->vel);
			}
		}
	}

	c /= (flocksize - 1);

	//(*c) *= 2.;

	return c; // move an eight of the way
}

GsVec Boid::cohesion(Boid** boids, int flocksize)
{
	GsVec c = GsVec();
	for (int n = 0; n < flocksize; n++){
		if (id != boids[n]->id){
			if (((boids[n]->pos) - pos).norm() < 120.0f){
				if (((boids[n]->pos) - pos).norm() > 5.0f){
					c += (boids[n]->pos - pos) / (boids[n]->pos - pos).norm();
				}
			}
			else{
				c += ((boids[n]->pos - pos) / (boids[n]->pos - pos).norm()) / 200.0f;
			}
		}
	}

	c /= (flocksize - 1);

	c *= 54.;

	return c;
}