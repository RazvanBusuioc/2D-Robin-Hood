#include <string>
#include <Core/Engine.h>

class PowerBar {
protected:
	std::string name;
	float pos_x;
	float pos_y;
	float height;
	float length;
	float scale_x;
	Mesh* powerbar_mesh;

public:
	PowerBar();
	PowerBar(std::string name, float height, float length);

	void set_mesh(Mesh* m);
	void set_positions(float x, float y);
	void set_pos_x(float x);
	void set_pos_y(float y);
	void set_height(float h);
	void set_length(float l);
	void set_scale_x(float s);
	void increase_scale_x(float t);

	float get_pos_x();
	float get_pos_y();
	float get_height();
	float get_length();
	float get_scale_x();
	Mesh* get_mesh();
};