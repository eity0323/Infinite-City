#include "building_part.h"

BuildingPart::BuildingPart(float unitsize)
{
	cubelayer = new BuildingLayer();
	walllayer = new BuildingLayer();
	decallayer = new BuildingLayer();

	_unitsize = unitsize;
	_height = 0;

	_sizeid = -1;

	layersdraw = false;
}

void BuildingPart::buildLayers(int* layouts, int* layoutdepths, int zsize, int ysize, int xsize)
{
	layersdraw = true;

	vector<float>* cp = new vector<float>();
	vector<float>* cn = new vector<float>();
	vector<int>* ct = new vector<int>();

	vector<float>* wp = new vector<float>();
	vector<float>* wn = new vector<float>();
	vector<int>* wt = new vector<int>();

	vector<float>* dp = new vector<float>();
	vector<float>* dn = new vector<float>();
	vector<int>* dt = new vector<int>();

	vector<float>* tp;
	vector<float>* tn;
	vector<int>* tt;

	int depth = 0;
	int depthfrom = 0;
	int depthto = 0;

	for (int z = 0; z < zsize; z++){
		depth = layoutdepths[z];
		depthfrom = depthto;
		depthto = depthfrom + depth;

		_height += depth * _unitsize;

		for (int y = 0; y < ysize; y++){
			for (int x = 0; x < xsize; x++){
				int target = layouts[z * ysize * xsize + y * xsize + x];
				if (target > 0){
					if (target == 3){
						tp = dp;	tn = dn;	tt = dt;
					}else if (target == 2){
						tp = wp;	tn = wn;	tt = wt;
					}else{ // 1
						tp = cp;	tn = cn;	tt = ct;
					}

					if (y == 0 || layouts[z * ysize * xsize + (y - 1) * xsize + x] == 0){ // Up
						tp->push_back(x * _unitsize);		tp->push_back(y * _unitsize);	tp->push_back(depthfrom * _unitsize);
						tp->push_back((x + 1) * _unitsize);	tp->push_back(y * _unitsize);	tp->push_back(depthfrom * _unitsize);
						tp->push_back((x + 1) * _unitsize);	tp->push_back(y * _unitsize);	tp->push_back(depthto * _unitsize);

						tp->push_back(x * _unitsize);		tp->push_back(y * _unitsize);	tp->push_back(depthfrom * _unitsize);
						tp->push_back((x + 1) * _unitsize);	tp->push_back(y * _unitsize);	tp->push_back(depthto * _unitsize);
						tp->push_back(x * _unitsize);		tp->push_back(y * _unitsize);	tp->push_back(depthto * _unitsize);

						tn->push_back(0);		tn->push_back(-1);	tn->push_back(0);
						tn->push_back(0);		tn->push_back(-1);	tn->push_back(0);
						tn->push_back(0);		tn->push_back(-1);	tn->push_back(0);
						tn->push_back(0);		tn->push_back(-1);	tn->push_back(0);
						tn->push_back(0);		tn->push_back(-1);	tn->push_back(0);
						tn->push_back(0);		tn->push_back(-1);	tn->push_back(0);

						tt->push_back(0);		tt->push_back(0);
						tt->push_back(1);		tt->push_back(0);
						tt->push_back(1);		tt->push_back(depth);
						tt->push_back(0);		tt->push_back(0);
						tt->push_back(1);		tt->push_back(depth);
						tt->push_back(0);		tt->push_back(depth);
					}
					if (y == ysize - 1 || layouts[z * ysize * xsize + (y + 1) * xsize + x] == 0){ // Down
						tp->push_back(x * _unitsize);		tp->push_back((y + 1) * _unitsize);	tp->push_back(depthfrom * _unitsize);

						tp->push_back((x + 1) * _unitsize);	tp->push_back((y + 1) * _unitsize);	tp->push_back(depthto * _unitsize);
						tp->push_back((x + 1) * _unitsize);	tp->push_back((y + 1) * _unitsize);	tp->push_back(depthfrom * _unitsize);

						tp->push_back(x * _unitsize);		tp->push_back((y + 1) * _unitsize);	tp->push_back(depthfrom * _unitsize);

						tp->push_back(x * _unitsize);		tp->push_back((y + 1) * _unitsize);	tp->push_back(depthto * _unitsize);
						tp->push_back((x + 1) * _unitsize);	tp->push_back((y + 1) * _unitsize);	tp->push_back(depthto * _unitsize);

						tn->push_back(0);		tn->push_back(1);	tn->push_back(0);
						tn->push_back(0);		tn->push_back(1);	tn->push_back(0);
						tn->push_back(0);		tn->push_back(1);	tn->push_back(0);
						tn->push_back(0);		tn->push_back(1);	tn->push_back(0);
						tn->push_back(0);		tn->push_back(1);	tn->push_back(0);
						tn->push_back(0);		tn->push_back(1);	tn->push_back(0);

						tt->push_back(0);		tt->push_back(0);
						tt->push_back(1);		tt->push_back(depth);
						tt->push_back(1);		tt->push_back(0);
						tt->push_back(0);		tt->push_back(0);
						tt->push_back(0);		tt->push_back(depth);
						tt->push_back(1);		tt->push_back(depth);
					}
					if (x == 0 || layouts[z * ysize * xsize + y * xsize + (x - 1)] == 0){ // Left
						tp->push_back(x * _unitsize);	tp->push_back(y * _unitsize);		tp->push_back(depthfrom * _unitsize);
						tp->push_back(x * _unitsize);	tp->push_back((y + 1) * _unitsize);	tp->push_back(depthto * _unitsize);
						tp->push_back(x * _unitsize);	tp->push_back((y + 1) * _unitsize);	tp->push_back(depthfrom * _unitsize);
						tp->push_back(x * _unitsize);	tp->push_back(y * _unitsize);		tp->push_back(depthfrom * _unitsize);
						tp->push_back(x * _unitsize);	tp->push_back(y * _unitsize);		tp->push_back(depthto * _unitsize);
						tp->push_back(x * _unitsize);	tp->push_back((y + 1) * _unitsize);	tp->push_back(depthto * _unitsize);

						tn->push_back(-1);		tn->push_back(0);	tn->push_back(0);
						tn->push_back(-1);		tn->push_back(0);	tn->push_back(0);
						tn->push_back(-1);		tn->push_back(0);	tn->push_back(0);
						tn->push_back(-1);		tn->push_back(0);	tn->push_back(0);
						tn->push_back(-1);		tn->push_back(0);	tn->push_back(0);
						tn->push_back(-1);		tn->push_back(0);	tn->push_back(0);

						tt->push_back(0);		tt->push_back(0);
						tt->push_back(1);		tt->push_back(depth);
						tt->push_back(1);		tt->push_back(0);
						tt->push_back(0);		tt->push_back(0);
						tt->push_back(0);		tt->push_back(depth);
						tt->push_back(1);		tt->push_back(depth);
					}
					if (x == xsize - 1 || layouts[z * ysize * xsize + y * xsize + (x + 1)] == 0){ // Right
						tp->push_back((x + 1) * _unitsize);			tp->push_back(y * _unitsize);		tp->push_back(depthfrom * _unitsize);
						tp->push_back((x + 1) * _unitsize);			tp->push_back((y + 1) * _unitsize);	tp->push_back(depthfrom * _unitsize);
						tp->push_back((x + 1) * _unitsize);			tp->push_back((y + 1) * _unitsize);	tp->push_back(depthto * _unitsize);
						tp->push_back((x + 1) * _unitsize);			tp->push_back(y * _unitsize);		tp->push_back(depthfrom * _unitsize);
						tp->push_back((x + 1) * _unitsize);			tp->push_back((y + 1) * _unitsize);	tp->push_back(depthto * _unitsize);
						tp->push_back((x + 1) * _unitsize);			tp->push_back(y * _unitsize);		tp->push_back(depthto * _unitsize);

						tn->push_back(1);		tn->push_back(0);	tn->push_back(0);
						tn->push_back(1);		tn->push_back(0);	tn->push_back(0);
						tn->push_back(1);		tn->push_back(0);	tn->push_back(0);
						tn->push_back(1);		tn->push_back(0);	tn->push_back(0);
						tn->push_back(1);		tn->push_back(0);	tn->push_back(0);
						tn->push_back(1);		tn->push_back(0);	tn->push_back(0);

						tt->push_back(0);		tt->push_back(0);
						tt->push_back(1);		tt->push_back(0);
						tt->push_back(1);		tt->push_back(depth);
						tt->push_back(0);		tt->push_back(0);
						tt->push_back(1);		tt->push_back(depth);
						tt->push_back(0);		tt->push_back(depth);
					}
					if (z == 0 || layouts[(z - 1) * ysize * xsize + y * xsize + x] == 0){ // Bottom
						tp->push_back(x * _unitsize);		tp->push_back(y * _unitsize);		tp->push_back(depthfrom * _unitsize);
						tp->push_back((x + 1) * _unitsize);	tp->push_back((y + 1) * _unitsize);	tp->push_back(depthfrom * _unitsize);
						tp->push_back((x + 1) * _unitsize);	tp->push_back(y * _unitsize);		tp->push_back(depthfrom * _unitsize);

						tp->push_back(x * _unitsize);		tp->push_back(y * _unitsize);		tp->push_back(depthfrom * _unitsize);
						tp->push_back(x * _unitsize);		tp->push_back((y + 1) * _unitsize);	tp->push_back(depthfrom * _unitsize);
						tp->push_back((x + 1) * _unitsize);	tp->push_back((y + 1) * _unitsize);	tp->push_back(depthfrom * _unitsize);

						tn->push_back(0);		tn->push_back(0);	tn->push_back(-1);
						tn->push_back(0);		tn->push_back(0);	tn->push_back(-1);
						tn->push_back(0);		tn->push_back(0);	tn->push_back(-1);
						tn->push_back(0);		tn->push_back(0);	tn->push_back(-1);
						tn->push_back(0);		tn->push_back(0);	tn->push_back(-1);
						tn->push_back(0);		tn->push_back(0);	tn->push_back(-1);

						tt->push_back(0);		tt->push_back(0);
						
						tt->push_back(1);		tt->push_back(1);
						tt->push_back(1);		tt->push_back(0);
						tt->push_back(0);		tt->push_back(0);
						tt->push_back(0);		tt->push_back(1);
						tt->push_back(1);		tt->push_back(1);
					}
					if (z == zsize - 1 || layouts[(z + 1) * ysize * xsize + y * xsize + x] == 0){ // Top
						tp->push_back(x * _unitsize);		tp->push_back(y * _unitsize);		tp->push_back(depthto * _unitsize);
						tp->push_back((x + 1) * _unitsize);	tp->push_back(y * _unitsize);		tp->push_back(depthto * _unitsize);
						tp->push_back((x + 1) * _unitsize);	tp->push_back((y + 1) * _unitsize);	tp->push_back(depthto * _unitsize);

						tp->push_back(x * _unitsize);		tp->push_back(y * _unitsize);		tp->push_back(depthto * _unitsize);
						tp->push_back((x + 1) * _unitsize);	tp->push_back((y + 1) * _unitsize);	tp->push_back(depthto * _unitsize);
						tp->push_back(x * _unitsize);		tp->push_back((y + 1) * _unitsize);	tp->push_back(depthto * _unitsize);

						tn->push_back(0);		tn->push_back(0);	tn->push_back(1);
						tn->push_back(0);		tn->push_back(0);	tn->push_back(1);
						tn->push_back(0);		tn->push_back(0);	tn->push_back(1);
						tn->push_back(0);		tn->push_back(0);	tn->push_back(1);
						tn->push_back(0);		tn->push_back(0);	tn->push_back(1);
						tn->push_back(0);		tn->push_back(0);	tn->push_back(1);

						tt->push_back(0);		tt->push_back(0);
						tt->push_back(1);		tt->push_back(0);
						tt->push_back(1);		tt->push_back(1);
						tt->push_back(0);		tt->push_back(0);
						tt->push_back(1);		tt->push_back(1);
						tt->push_back(0);		tt->push_back(1);
					}
				}
			}
		}
	}

	cubelayer->build(cp, cn, ct);
	walllayer->build(wp, wn, wt);
	decallayer->build(dp, dn, dt);

	/*delete cp;
	delete cn;
	delete ct;

	delete wp;
	delete wn;
	delete wt;

	delete dp;
	delete dn;
	delete dt;

	delete tp;
	delete tn;
	delete tt;*/
}

void BuildingPart::drawCubeLayer()
{
	if (layersdraw){
		cubelayer->draw();
	}
}

void BuildingPart::drawWallLayer()
{
	if (layersdraw){
		walllayer->draw();
	}
}

void BuildingPart::drawDecalLayer()
{
	if (layersdraw){
		decallayer->draw();
	}
}

void BuildingPart::addInstance(GsVec pos, float t){
	if (layersdraw){
		cubelayer->addInstance(pos, t);
		walllayer->addInstance(pos, t);
		decallayer->addInstance(pos, t);
	}
}

void BuildingPart::drawCubeLayerInstanced()
{
	if (layersdraw){
		cubelayer->drawInstanced();
	}
}

void BuildingPart::drawWallLayerInstanced()
{
	if (layersdraw){
		walllayer->drawInstanced();
	}
}

void BuildingPart::drawDecalLayerInstanced()
{
	if (layersdraw){
		decallayer->drawInstanced();
	}
}

void BuildingPart::clearInstances(){
	cubelayer->clearInstances();
	walllayer->clearInstances();
	decallayer->clearInstances();
}

void BuildingPart::setSizeId(int sizeid)
{
	_sizeid = sizeid;
}

int BuildingPart::getSizeid()
{
	return _sizeid;
}

float BuildingPart::getHeight()
{
	return _height;
}
int BuildingPart::getPolygonCount(){
	return cubelayer->getPolygonCount() + walllayer->getPolygonCount() + decallayer->getPolygonCount();
}