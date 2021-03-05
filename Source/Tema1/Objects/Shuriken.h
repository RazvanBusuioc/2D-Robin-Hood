#pragma once
#include <string>
#include <Core/Engine.h>

class Shuriken
{
protected:
	std::string name;
	float pos_x, pos_y;
	float shuriken_length;
	float radians = 0;
	float scale = 1;
	Mesh* shuriken_mesh;
	glm::vec3 color;

public:
	Shuriken();
	Shuriken(std::string name, float shuriken_length, glm::vec3 color);

	Mesh* get_shuriken_mesh();
	float get_shuriken_length();
	float get_shuriken_tip_width();
	float get_shuriken_pos_x();
	float get_shuriken_pos_y();
	float get_shuriken_radians();
	float get_shuriken_scale();
	glm::vec3 get_shuriken_color();

	bool check_collision(float x, float y);

	void set_shuriken_mesh(Mesh* m);
	void compute_mesh(glm::vec3 corner);
	void set_shuriken_length(float l);
	void set_shuriken_positions(float x, float y);
	void set_shuriken_pos_x(float x);
	void set_shuriken_pos_y(float y);
	void set_shuriken_color(glm::vec3 c);
	void set_shuriken_radians(float r);
	void increase_shuriken_radians(float t);
	void decrease_shuriken_scale(float t);
	void decrease_pos_x(float t);
};

