#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

#define BOW_WIDTH 3

namespace Object2D
{

	/* Meshes for all the object in the scene */
	Mesh* CreateArrow(std::string name, glm::vec3 leftBottomCorner, float rectangle_length, float rectangle_height,
					  float tip_height, float tip_width, glm::vec3 color, bool fill = true);
	Mesh* CreateBow(std::string name, glm::vec3 leftBottomCorner, float bow_height, float bow_length, glm::vec3 color);

	Mesh* CreateShuriken(std::string name, glm::vec3 leftBottomCorner, float shuriken_length, glm::vec3 color, bool fill = true);

	Mesh* CreateBallon(std::string name, glm::vec3 leftBottomCorner, float ballon_height, float ballon_length, glm::vec3 color, bool fill = true);

	Mesh* CreatePowerBar(std::string name, glm::vec3 leftBottomCorner, float length, float height,  glm::vec3 color, bool fill = true);

	Mesh* CreateHeart(std::string name, glm::vec3 leftBottomCorner, float height, float length, glm::vec3 color, bool fill = true);

	/* creates digits */
	Mesh* CreateDigit(std::string name, glm::vec3 leftBottomCorner, float height, float length, glm::vec3 color, int digit);

	/* creates letters */
	Mesh* CreateLetter(std::string name, glm::vec3 leftBottomCorner, float height, float length, glm::vec3 color, int letter);

	Mesh* CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, float height, glm::vec3 color, bool fill = true);
}


