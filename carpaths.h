#pragma once

#include <gsim/gs_color.h>
#include <gsim/gs_array.h>
#include <gsim/gs_vec.h>

#include "app_window.h"

#define ACTION_FORWARD 0
#define ACTION_TURN 1
#define ACTION_SWITCHLANE 2
#define ACTION_HELIX 3

class CarPath{
private:
	
public:
	CarPath(){

	}
	CarPath(GsVec _pos, int _dirX, int _dirY){
		pos = _pos;
		dirX = _dirX;
		dirY = _dirY;

		exists = true;

		distance = 0;

		t = 0;

		currentAction = ACTION_FORWARD;
	}

	GsVec pos;
	GsVec dir;
	int dirX;
	int dirY;

	float distance;

	float t;
	vector<GsVec> currentSpline;

	int currentAction;

	

	bool exists;

	int binomial(int n, int k){
		int num, den;
		if (n < k){
			return(0);
		}
		else{
			den = 1;
			num = 1;
			for (int i = 1; i <= k; i = i + 1){
				den = den * i;
			}
			for (int j = n - k + 1; j <= n; j = j + 1){
				num = num * j;
			}
			return (num / den);
		}
	}

	GsVec eval_bezier(float t, const vector<GsVec> &points){
		GsVec value;
		value.set(0, 0, 0);

		for (int i = 0; i<points.size(); i++){
			// (t*t)*p0 + t*(1-t)*p1 + (1-t)*(1-t)*p2;
			// (t*t*t)*p1 + t*t*(1-t)*p2 + 

			float frac = pow(t, i) * pow(1 - t, points.size() - 1 - i);

			value += frac * binomial(points.size() - 1, i) * points[i];
		}

		return value;
	}

	GsVec eval_bspline(float _t, const vector<GsVec> &points) {
		// This one I know works:

		_t *= (points.size() - 2);
		// quadtratic
		int i = int(_t);
		if (i + 2 >= points.size()){
			i = points.size() - 2;
		}

		float t = _t - i;

		GsVec a = (points[i] + points[i + 1]) / 2;
		GsVec b = points[i + 1];
		GsVec c = (points[i + 1] + points[i + 2]) / 2;

		return (t*t)*c + 2 * t*(1 - t)*b + (1 - t)*(1 - t)*a;
	}
		

};
