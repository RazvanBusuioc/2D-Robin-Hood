#include <Tema1/Objects/PoawerBar.h>
#include <vector>
#include <iostream>
#include <string>

#include <Core/Engine.h>
#include <Tema1\Object2D.h>

PowerBar::PowerBar() {}

PowerBar::PowerBar(std::string name, float height, float length) {
	this->name = name;
	this->height = height;
	this->length = length;
	this->scale_x = 1;
}

void PowerBar::set_mesh(Mesh* m) {
	this->powerbar_mesh = m;
}
void PowerBar::set_positions(float x, float y) {
	this->pos_x = x;
	this->pos_y = y;
}
void PowerBar::set_pos_x(float x) {
	this->pos_x = x;
}
void PowerBar::set_pos_y(float y) {
	this->pos_y = y;
}
void PowerBar::set_height(float h) {
	this->height = h;
}
void PowerBar::set_length(float l) {
	this->length = l;
}
void PowerBar::set_scale_x(float s) {
	this->scale_x = s;
}
void PowerBar::increase_scale_x(float t) {
	this->scale_x += t;
}

float PowerBar::get_pos_x() {
	return this->pos_x;
}
float PowerBar::get_pos_y() {
	return this->pos_y;
}
float PowerBar::get_height() {
	return this->height;
}
float PowerBar::get_length() {
	return this->length;
}
float PowerBar::get_scale_x() {
	return this->scale_x;
}
Mesh* PowerBar::get_mesh() {
	return this->powerbar_mesh;
}