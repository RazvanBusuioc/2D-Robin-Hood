#pragma once
#include <string>
#include <Core/Engine.h>

class Arrow
{
protected:
	std::string name;
	float pos_x, pos_y;
	float tip_width, arrow_length;
	float rectangle_length;
	float rectangle_width;
	float tip_length;
	float radians = 0;
	float velocity = 0;
	Mesh* arrow_mesh;
	glm::vec3 color;

public:
	Arrow();
	Arrow(std::string name, float arrow_length, float tip_width, glm::vec3 color);

	Mesh* get_arrow_mesh();
	float get_arrow_length();
	float get_arrow_tip_width();
	float get_arrow_pos_x();
	float get_tip_pos_x();
	float get_tip_pos_y();
	float get_arrow_pos_y();
	float get_arrow_radians();
	float get_arrow_velocity();
	glm::vec3 get_arrow_color();

	void set_arrow_mesh(Mesh* m);
	void compute_mesh(glm::vec3 corner);
	void set_arrow_tip_width(float h);
	void set_arrow_length(float l);
	void set_arrow_positions(float x, float y);
	void set_arrow_pos_x(float x);
	void set_arrow_pos_y(float y);
	void set_arrow_radians(float r);
	void set_arrow_color(glm::vec3 c);
	void set_arrow_velocity(float v);
	void increase_pos_y(float l);
	void arrow_moving(float sec, bool gravity = false);
};

