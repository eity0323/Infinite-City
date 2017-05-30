#pragma once

#include "building_layer.h"

class BuildingPart{
private:
	BuildingLayer *cubelayer;
	BuildingLayer *walllayer;
	BuildingLayer *decallayer;

	float _unitsize;
	float _height;

	int _sizeid;

	bool layersdraw;

public:
	BuildingPart(float unitsize);

	void buildLayers(int* layouts, int* layoutdepths, int zsize, int ysize, int xsize);

	void drawCubeLayer();
	void drawWallLayer();
	void drawDecalLayer();

	void addInstance(GsVec pos, float t);

	void drawCubeLayerInstanced();
	void drawWallLayerInstanced();
	void drawDecalLayerInstanced();

	void clearInstances();

	void setSizeId(int sizeid);

	int getSizeid();

	float getHeight();
	int getPolygonCount();
};