#include <Tema1/Objects/Arrow.h>
#include <vector>
#include <iostream>
#include <string>

#include <Core/Engine.h>
#include <Tema1\Object2D.h>


Arrow::Arrow(std::string name, float arrow_length, float tip_width, glm::vec3 color) {
	this->name = name;
	this->arrow_length = arrow_length;
	this->tip_width = tip_width;
	this->color = color;
	this->rectangle_length = ((float)4 / 5) * arrow_length;
	this->tip_length = arrow_length - this->rectangle_length;
	this->rectangle_width =  tip_width / 10;
}

Mesh* Arrow::get_arrow_mesh() {
	return this->arrow_mesh;
}
float Arrow::get_arrow_length() {
	return this->arrow_length;
}
float Arrow::get_arrow_tip_width() {
	return this->tip_width;
}
float Arrow::get_arrow_pos_x() {
	return this->pos_x;
}
float Arrow::get_arrow_pos_y() {
	return this->pos_y;
}
float Arrow::get_arrow_radians() {
	return this->radians;
}
glm::vec3 Arrow::get_arrow_color() {
	return this->color;
}
float Arrow::get_arrow_velocity() {
	return this->velocity;
}
float Arrow::get_tip_pos_x() {
	float x = this->arrow_length;
	float y = this->rectangle_width / 2;
	return (cos(radians) * x - sin(radians) * y) + this->pos_x;
}
float Arrow::get_tip_pos_y() {
	float x = this->arrow_length;
	float y = this->rectangle_width / 2;
	return (sin(radians) * x + cos(radians) * y) + this->pos_y;
}

void Arrow::set_arrow_mesh(Mesh* m) {
	this->arrow_mesh = m;
}
void Arrow::compute_mesh(glm::vec3 corner) {
	this->arrow_mesh = Object2D::CreateArrow(this->name, corner, this->rectangle_length, this->rectangle_width,
		this->tip_length, this->tip_width, this->color);
}
void Arrow::set_arrow_tip_width(float t) {
	this->tip_width = t;
}
void Arrow::set_arrow_length(float l) {
	this->arrow_length = l;
}
void Arrow::set_arrow_positions(float x, float y) {
	this->pos_x = x;
	this->pos_y = y;
}
void Arrow::set_arrow_pos_x(float x) {
	this->pos_x = x;
}
void Arrow::set_arrow_radians(float r) {
	this->radians = r;
}
void Arrow::set_arrow_pos_y(float y) {
	this->pos_y = y;
}
void Arrow::set_arrow_color(glm::vec3 c) {
	this->color = color;
}
void Arrow::increase_pos_y(float l) {
	this->pos_y += l;
}
void Arrow::set_arrow_velocity(float v) {
	this->velocity = v;
}
void Arrow::arrow_moving(float deltaTimeSeconds, bool g) {
	if (g) {
		/* if gravity enabled */
		this->radians -= deltaTimeSeconds;
	}
	this->pos_x += velocity * cos(radians);
	this->pos_y += velocity * sin(radians);
}