#include "Bow.h"
#include <vector>
#include <iostream>
#include <string>

#include <Core/Engine.h>
#include <Tema1\Object2D.h>
#include <Tema1/Objects/Ballon.h>

Ballon::Ballon() {}

Ballon::Ballon(std::string name, float bow_height, float bow_length, glm::vec3 color) {
	this->ballon_height = bow_height;
	this->ballon_length = bow_length;
	this->color = color;
	this->name = name;
}

Mesh* Ballon::get_ballon_mesh(){
	return this->ballon_mesh;
}
float Ballon:: get_ballon_height(){
	return this->ballon_height;
}
float Ballon::get_ballon_length(){
	return this->ballon_length;
}
float Ballon::get_ballon_pos_x(){
	return this->pos_x;
}
float Ballon::get_ballon_pos_y(){
	return this->pos_y;
}
float Ballon::get_ballon_scale() {
	return this->scale;
}
glm::vec3 Ballon::get_ballon_color(){
	return this->color;
}

void Ballon::set_ballon_mesh(Mesh* m){
	this->ballon_mesh = m;
}
void Ballon::compute_mesh(glm::vec3 corner){
	this->ballon_mesh = Object2D::CreateBallon(this->name, corner, this->ballon_height, this->ballon_length, this->color);
}
void Ballon::set_ballon_height(float h){
	this->ballon_height = h;
}
void Ballon::set_ballon_length(float l){
	this->ballon_length = l;
}
void Ballon::set_ballon_positions(float x, float y){
	this->pos_x = x;
	this->pos_y = y;
}
void Ballon::set_ballon_pos_x(float x){
	this->pos_x = x;
}
void Ballon::set_ballon_pos_y(float y){
	this->pos_y = y;
}
void Ballon::set_ballon_color(glm::vec3 c){
	this->color = c;
}
void Ballon::increase_pos_y(float l) {
	this->pos_y += l;
}

void Ballon::decrease_ballon_scale(float s) {
	this->scale -= s;
}


bool Ballon::check_collision(float x, float y) {
	/* check if the position given by x and y inside the elipse of the ballon */
	x -= this->pos_x ;
	y -= this->pos_y ;
	if ((x * x) / ((this->ballon_length / 2) * (this->ballon_length / 2)) +
		(y * y) / ((this->ballon_height / 2) * (this->ballon_height / 2)) <= 1) {
		return true;
	}
	return false;
}