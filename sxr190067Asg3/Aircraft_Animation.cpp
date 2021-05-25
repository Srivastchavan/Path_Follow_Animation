#include "Aircraft_Animation.h"


/*****************************************************************************
    * Written by Srivastchavan Rengarajan starting September 26, 2020.
******************************************************************************/



Aircraft_Animation::Aircraft_Animation()
{
	this->m_model_mat = glm::mat4(1.0f);
}


Aircraft_Animation::~Aircraft_Animation()
{
}

void Aircraft_Animation::init()
{
	reset();
}

void Aircraft_Animation::init(Curve* animation_curve)
{
	m_animation_curve = animation_curve;
	
	time_animation = 0;
	pt_cur = glm::vec3(0.0, 8.5, -2.0);
	pt_next = glm::vec3(0.0, 8.5, -2.0);

    reset();
}

void Aircraft_Animation::reset()
{
    m_model_mat = glm::mat4(1.0f);

    if (m_animation_curve != nullptr && m_animation_curve->control_points_pos.size() > 0)
    {
        m_model_mat = glm::translate(m_model_mat, m_animation_curve->control_points_pos[0]);
    }

    move_reset();
}

void Aircraft_Animation::move_reset()
{
    is_moving = false;
    move_end = true;
    time_animation = 0.0;
    distance_animation = 0.0;
    cur_itr = u_length.begin();
    pt_cur = glm::vec3(0.0, 8.5, -2.0);
    pt_next = glm::vec3(0.0, 8.5, -2.0);
}

void Aircraft_Animation::update(float delta_time)
{
    float normalised_time = 0.0;
    float normalised_dist = 0.0;

    if (m_animation_curve->curve_on == true && m_animation_curve->curve_points_pos.empty() == false && u_length.empty() == true) {
        buildSamplingTable();
        distance_total = u_length[u_length.size() - 1][3];
        cur_itr = u_length.begin();
    }
    else if (is_moving == true && u_length.empty() == false) {

        time_animation += delta_time;
        normalised_time = (time_animation / total_moving_time);

        normalised_dist = calculateNormalDist(normalised_time);
        distance_animation = (distance_total * normalised_dist);

        if (distance_animation == 0.0) {
            pt_cur = glm::vec3(0.0, 8.5, -2.0);
            pt_next = glm::vec3(0.0, 8.5, -2.0);
        }
        else {
            pt_cur = pt_next;
            pt_next = findNextPoint(distance_animation);
        }
        vector = pt_next - pt_cur;
        m_model_mat = glm::translate(m_model_mat, vector);
    }

    else if (is_moving == false && distance_animation >= distance_total) {
        move_reset();
    }
}



void Aircraft_Animation::buildSamplingTable()
{
    float piece_len;
    float arc_len = 0;
    
    std::vector<glm::vec3> pts = m_animation_curve->curve_points_pos;
    std::vector<vec3>::iterator itr;

    for (itr = pts.begin(); itr != pts.end(); itr++) {

        if (itr == pts.begin()) {
            arc_len = 0;
        }
        else {
            piece_len = sqrt(pow((*(itr))[0] - (*(itr - 1))[0], 2) +
                pow((*(itr))[1] - (*(itr - 1))[1], 2) +
                pow((*(itr))[2] - (*(itr - 1))[2], 2) * 1.0);
            arc_len = arc_len + piece_len;
            u_length.push_back({ *itr, arc_len });
        }
    }
}

glm::vec3 Aircraft_Animation::interpolation(float distance, glm::vec4 start, glm::vec4 end)
{
    float total_len = end[3] - start[3];
    float delta_len = distance - start[3];
    float ratio = delta_len / total_len;
    glm::vec3 ipl_pt = { (start[0] + ratio * (end[0] - start[0])),
                                    (start[1] + ratio * (end[1] - start[1])),
                                    (start[2] + ratio * (end[2] - start[2])) };

    return ipl_pt;
}

float Aircraft_Animation::calculateNormalDist(float time_updated)
{
    float velocity;
    float dist;
    float v0 = 2 / (1 - t1 + t2);

    if (time_updated > 0 && time_updated <= t1) {
        velocity = v0 * (time_updated / t1);
        dist = (v0 * time_updated * time_updated) / (2 * t1);
    }
    else if (time_updated > t1&& time_updated <= t2) {
        velocity = v0;
        dist = (v0 * t1) / 2 + v0 * (time_updated - t1);
    }
    else if (time_updated > t2&& time_updated <= 1) {
        velocity = v0 * (1 - (time_updated - t2) / (1 - t2));
        dist = 1 - velocity * (1 - time_updated) / 2;
    }
    else if (time_updated > 1) {
        velocity = 0;
        dist = 1;
    }
    else {
        velocity = 0;
        dist = 0;
    }

    return dist;

}

glm::vec3 Aircraft_Animation::findNextPoint(float distance)
{
    glm::vec3 pos;

    if (distance == (*cur_itr)[3]) {
        pos = { (*cur_itr)[0], (*cur_itr)[1], (*cur_itr)[2] };
    }
    else if (distance < (*cur_itr)[3]) {
        if (cur_itr == u_length.begin()) {
            pos = interpolation(distance, glm::vec4(0.0, 8.5, -2.0, 0.0), *cur_itr);
        }
        else {
            pos = interpolation(distance, *(cur_itr - 1), *cur_itr);
        }
    }
    else {
        cur_itr = cur_itr + 1;
        pos = findNextPoint(distance);
    }

    return pos;
}

