#include "Tema1/Objects/Bow.h"
#include <vector>
#include <iostream>
#include <string>

#include <Core/Engine.h>
#include <Tema1\Object2D.h>

Bow::Bow() {}

Bow::Bow(std::string name, float bow_height, float bow_length, glm::vec3 color) {
	this->bow_height = bow_height;
	this->bow_length = bow_length;
	this->color = color;
	this->name = name;
	this->radians = 0;
}
Mesh* Bow::get_bow_mesh() {
	return this->bow_mesh;
}
float Bow::get_bow_height() {
	return this->bow_height;
}
float Bow::get_bow_length() {
	return this->bow_length;
}
float Bow::get_bow_pos_x() {
	return this->pos_x;
}
float Bow::get_bow_pos_y() {
	return this->pos_y;
}
glm::vec3 Bow::get_bow_color() {
	return this->color;
}
float Bow::get_bow_radians() {
	return this->radians;
}

void Bow::compute_mesh(glm::vec3 corner) {
	this->bow_mesh = Object2D::CreateBow(this-> name, corner, this->bow_height, this->bow_length, this->color);
}
void Bow::set_bow_mesh(Mesh* m) {
	this->bow_mesh = m;
}
void Bow::set_bow_height(float h) {
	this->bow_height = h;
}
void Bow::set_bow_length(float l) {
	this->bow_length = l;
}
void Bow::set_bow_pos_x(float x) {
	this->pos_x = x;
}
void Bow::set_bow_pos_y(float y) {
	this->pos_y = y;
}
void Bow::set_bow_radians(float r) {
	this->radians = r;
}
void Bow::set_bow_positions(float x, float y) {
	this->pos_x = x;
	this->pos_y = y;
}
void Bow::set_bow_color(glm::vec3 c) {
	this->color = c;
}
void Bow::increase_pos_y(float d) {
	this->pos_y += d;
}


bool Bow::check_collision(float x, float y, float radius) {
	/* circle elipse collision */
	float distX = abs(x - this->pos_x );
	float distY = y - this->pos_y;// -radius;

	float distance = sqrt((distX * distX) + (distY * distY));
	float alpha = atan(distY / distX) - radians;

	float elipse_radius;
	if (alpha < 3.14 / 2 && alpha > -3.14 / 2) {
		/* the bow is not horizontal */
		float x1 = this->bow_length * cos(alpha);
		float y1 = this->bow_height / 2 * sin(alpha);
		elipse_radius = sqrt(x1 * x1 + y1 * y1);
		if (distance <= elipse_radius + radius) {
			return true;
		}
	}
	else {
		/* the bow is horizontal */
		if (this->radians > 0 && x - radius < this->pos_x + this->bow_height/2 && y + radius > this->pos_y) {
			return true;
		}
		if (this->radians < 0 && x - radius < this->pos_x + this->bow_height / 2 && y - radius < this->pos_y) {
			return true;
		}
	}
	
	return false;
}