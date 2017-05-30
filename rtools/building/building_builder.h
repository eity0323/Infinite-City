#pragma once

#include "building_part.h"

#define NUM_PARTS 12

class BuildingBuilder{
private:

public:
	static BuildingPart* getBuildingPart(int id, float unitsize);
};