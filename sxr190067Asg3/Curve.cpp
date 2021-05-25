#include "Curve.h"
using namespace glm;
using namespace std;

/******************************************************************************
* Written by Srivastchavan Rengarajan starting September 26, 2020.
******************************************************************************/


float arr[16] = { -0.5, 1.5,-1.5, 0.5,
					 1.0,-2.5, 2.0,-0.5,
					-0.5, 0.0, 0.5, 0.0,
					 0.0, 1.0, 0.0, 0.0 };

Curve::Curve()
{
}

Curve::~Curve()
{
}

void Curve::init()
{
	this->control_points_pos = {
		{ 0.0, 8.5, -2.0 },
		{ -3.0, 11.0, 2.3 },
		{ -6.0, 8.5, -2.5 },
		{ -4.0, 5.5, 2.8 },
		{ 1.0, 2.0, -4.0 },
		{ 4.0, 2.0, 3.0 },
		{ 7.0, 8.0, -2.0 },
		{ 3.0, 10.0, 3.7 }
	};
	//calculate_curve();
}


vector<vec3> Curve::catmull_rom(const vec3& P0, const vec3& P1, const vec3& P2, const vec3& P3)
{
	mat4 cubic_mat = make_mat4(arr);
	mat4x3 control_points(P0, P1, P2, P3);
	vec4 uVar;
	vec3 single_point;
	vector<vec3> segment_Points;

	for (int i = 0; i < num_points_per_segment; i++) {
		float u = (1.0 / (float)num_points_per_segment) * (i);
		uVar = vec4(u * u * u, u * u, u, 1);
		single_point = control_points * cubic_mat * uVar;
		segment_Points.push_back(single_point);
	}
	return segment_Points;
}


void Curve::calculate_curve()
{
	//this->curve_points_pos = {
	//	{ 0.0, 8.5, -2.0 },
	//	{ -3.0, 11.0, 2.3 },
	//	{ -6.0, 8.5, -2.5 },
	//	{ -4.0, 5.5, 2.8 },
	//	{ 1.0, 2.0, -4.0 },
	//	{ 4.0, 2.0, 3.0 },
	//	{ 7.0, 8.0, -2.0 },
	//	{ 3.0, 10.0, 3.7 }
	//};

	int control_points_num = control_points_pos.size();
	vec3 P0;
	vec3 P1;
	vec3 P2;
	vec3 P3;
	vector<vec3> segment_Points;
	vector<vec3>::iterator itr;

	/*if (curve_on == false) {
		curve_points_pos.clear();
		itr = curve_points_pos.end();
		curve_points_pos.insert(itr, control_points_pos.begin(), control_points_pos.end());
	}
	else {
		curve_points_pos.clear();	
	}*/

	for (int i = 0; i < control_points_num; i++) {

		P0 = control_points_pos[((i + 7) % control_points_num)];
		P1 = control_points_pos[i % control_points_num];
		P2 = control_points_pos[((i + 1) % control_points_num)];
		P3 = control_points_pos[((i + 2) % control_points_num)];

		segment_Points = catmull_rom(P0, P1, P2, P3);

		itr = curve_points_pos.end();

		curve_points_pos.insert(itr, segment_Points.begin(), segment_Points.end());
		curve_points_pos.push_back(P2);
	}
}

