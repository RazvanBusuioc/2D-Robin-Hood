#pragma once
#include <string>
#include <Core/Engine.h>

class Ballon
{
protected:
	std::string name;
	float pos_x, pos_y;
	float ballon_height, ballon_length;
	float scale = 1;
	Mesh* ballon_mesh;
	glm::vec3 color;

public:
	Ballon();
	Ballon(std::string name, float ballon_height, float ballon_length, glm::vec3 color);

	Mesh* get_ballon_mesh();
	float get_ballon_height();
	float get_ballon_length();
	float get_ballon_pos_x();
	float get_ballon_pos_y();
	float get_ballon_scale();
	glm::vec3 get_ballon_color();

	void set_ballon_mesh(Mesh* m);
	void compute_mesh(glm::vec3 corner);
	void set_ballon_height(float h);
	void set_ballon_length(float l);
	void set_ballon_positions(float x, float y);
	void set_ballon_pos_x(float x);
	void set_ballon_pos_y(float y);
	void set_ballon_color(glm::vec3 c);
	void decrease_ballon_scale(float l);
	void increase_pos_y(float l);

	/* checks if posx and posy inside ballon */
	bool check_collision(float pos_x, float pos_y); 
};

