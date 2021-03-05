#pragma once
#include <string>
#include <Core/Engine.h>

class Bow
{
protected:
	std::string name;
	float pos_x, pos_y;
	float bow_height, bow_length;
	float radians;
	Mesh* bow_mesh;
	glm::vec3 color;

public:
	Bow();
	Bow(std::string name, float bow_height, float bow_length, glm::vec3 color);

	Mesh* get_bow_mesh();
	float get_bow_height();
	float get_bow_length();
	float get_bow_pos_x();
	float get_bow_pos_y();
	float get_bow_radians();
	glm::vec3 get_bow_color();

	bool check_collision(float x, float y, float radius);

	void set_bow_mesh(Mesh* m);
	void compute_mesh(glm::vec3 corner);
	void set_bow_height(float h);
	void set_bow_length(float l);
	void set_bow_positions(float x, float y);
	void set_bow_pos_x(float x);
	void set_bow_pos_y(float y);
	void set_bow_radians(float r);
	void set_bow_color(glm::vec3 c);
	void increase_pos_y(float d);
};

