#include "Bow.h"
#include <vector>
#include <iostream>
#include <string>

#include <Core/Engine.h>
#include <Tema1\Object2D.h>
#include <Tema1/Objects/Shuriken.h>


Shuriken::Shuriken() {}
Shuriken::Shuriken(std::string name, float shuriken_length, glm::vec3 color) {
	this->name = name;
	this->shuriken_length = shuriken_length;
	this->color = color;
}

Mesh* Shuriken::get_shuriken_mesh() {
	return this->shuriken_mesh;
}
float Shuriken::get_shuriken_length() {
	return this->shuriken_length;
}
float Shuriken::get_shuriken_pos_x() {
	return this->pos_x;
}
float Shuriken::get_shuriken_pos_y() {
	return this->pos_y;
}
float Shuriken::get_shuriken_radians() {
	return this->radians;
}
float Shuriken::get_shuriken_scale() {
	return this->scale;
}
glm::vec3 Shuriken::get_shuriken_color() {
	return this->color;
}

void Shuriken::set_shuriken_mesh(Mesh* m) {
	this->shuriken_mesh = m;
}
void Shuriken::compute_mesh(glm::vec3 corner) {
	this->shuriken_mesh = Object2D::CreateShuriken(this->name, corner, this->shuriken_length, this->color); 
}
void Shuriken::set_shuriken_length(float l) {
	this->shuriken_length = l;
}
void Shuriken::set_shuriken_positions(float x, float y) {
	this->pos_x = x;
	this->pos_y = y;
}
void Shuriken::set_shuriken_pos_x(float x) {
	this->pos_x = x;
}
void Shuriken::set_shuriken_pos_y(float y) {
	this->pos_y = y;
}
void Shuriken::set_shuriken_color(glm::vec3 c) {
	this->color = c;
}
void Shuriken::decrease_pos_x(float t) {
	this->pos_x -= t;
}
void Shuriken::set_shuriken_radians(float r) {
	this->radians = r;
}
void Shuriken::increase_shuriken_radians(float t) {
	if (radians > atan(1) * 4) {
		radians = 0;
	}
	this->radians += t;
}

void Shuriken::decrease_shuriken_scale(float l) {
	this->scale -= l;
}

bool Shuriken::check_collision(float x, float y) {
	/* point-circle collision */
	x -= this->pos_x + this->shuriken_length/2;
	y -= this->pos_y + this->shuriken_length / 2;
	if ((x * x) / ((this->shuriken_length / 2) * (this->shuriken_length / 2)) +
		(y * y) / ((this->shuriken_length / 2) * (this->shuriken_length / 2)) <= 1) {
		return true;
	}
	return false;
}