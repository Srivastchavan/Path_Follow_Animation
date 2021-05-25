#pragma once

#include <vector>
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Curve.h"

class Aircraft_Animation
{

public:
	float total_moving_time = 10;
	float t1 = 0.3;
	float t2 = 0.9;

	float time_animation;
	float distance_animation;
	bool move_end;
	bool is_moving;

private:
	glm::mat4 m_model_mat;
	Curve* m_animation_curve = nullptr;

	float distance_total = 0;
	std::vector<glm::vec4> u_length;
	std::vector<glm::vec4>::iterator cur_itr;
	glm::vec3 pt_cur;
	glm::vec3 pt_next;
	glm::vec3 vector;

public:
	Aircraft_Animation();
	~Aircraft_Animation();

	void init();
	void init(Curve* animation_curve);

	void update(float delta_time);

	void reset();
	glm::mat4 get_model_mat() { return m_model_mat; };


	void move_reset();
	void buildSamplingTable();
	float calculateNormalDist(float time_updated);
	glm::vec3 findNextPoint(float distance);
	glm::vec3 interpolation(float distance, glm::vec4 start, glm::vec4 end);
};

