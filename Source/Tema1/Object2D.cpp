#include "Object2D.h"

#include <Core/Engine.h>
#include <iostream>

Mesh* Object2D::CreateArrow(std::string name, glm::vec3 leftBottomCorner, float rectangle_length, float rectangle_height,
						float tip_length, float tip_height, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color), //0
		VertexFormat(corner + glm::vec3(rectangle_length, 0, 0), color), //1
		VertexFormat(corner + glm::vec3(rectangle_length, rectangle_height, 0), color), //2
		VertexFormat(corner + glm::vec3(0, rectangle_height, 0), color), //3
		VertexFormat(corner + glm::vec3(rectangle_length, -(tip_height/2 - rectangle_height/2), 0), color), //4
		VertexFormat(corner + glm::vec3(rectangle_length, rectangle_height + (tip_height / 2 - rectangle_height / 2), 0), color), //5
		VertexFormat(corner + glm::vec3(rectangle_length + tip_length, rectangle_height/2, 0), color) //6
	};

	Mesh* arrow = new Mesh(name);	
	if (!fill) {
		std::vector<unsigned short> indices = { 0, 1, 4, 6, 5, 2, 3};
		arrow->SetDrawMode(GL_LINE_LOOP);
		arrow->InitFromData(vertices, indices);
	}
	else {
		std::vector<unsigned short> indices = { 0, 1, 2, 3, 0, 2, 4, 6, 5};
		arrow->InitFromData(vertices, indices);
	}
	
	return arrow;
}

Mesh* Object2D::CreateBow(std::string name, glm::vec3 leftBottomCorner, float bow_height, float bow_length, glm::vec3 color)
{
	
	glm::vec3 corner = leftBottomCorner;

	float b = bow_height / 2; // elipse semi-minor axis
	float a = bow_length; // elipse semi-major axis
	
	std::vector<VertexFormat> vertices ;
	for (float i = -b; i <= b ; i++) {
		//elipse parametric representation : (x,y) = (a*cos(t), b*sin(t))
		float y = i; // current y 
		float x = a * cos(asin(y / b)); // curent x in parametric representation 
		vertices.push_back(VertexFormat(corner + glm::vec3(x, y, 0),color)); // add vertex of (x,y)
	}
	Mesh* bow = new Mesh(name);
	std::vector<unsigned short> indices ;
	for (int i = 0; i <= bow_height; i++) {
		indices.push_back(i);
	}
	glLineWidth(BOW_WIDTH);
	bow->SetDrawMode(GL_LINE_LOOP);
	bow->InitFromData(vertices, indices);
	return bow;
}

Mesh* Object2D::CreateShuriken(std::string name, glm::vec3 leftBottomCorner, float shuriken_length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices = {
		VertexFormat(corner, color), //0
		VertexFormat(corner + glm::vec3(shuriken_length/2, shuriken_length/2, 0), color), //1 - center
		VertexFormat(corner + glm::vec3(0, shuriken_length / 2, 0), color), // 2
		VertexFormat(corner + glm::vec3(shuriken_length / 2, 0, 0), color), // 3
		VertexFormat(corner + glm::vec3(shuriken_length, 0, 0), color), // 4
		VertexFormat(corner + glm::vec3(shuriken_length, shuriken_length / 2, 0), color), // 5
		VertexFormat(corner + glm::vec3(shuriken_length, shuriken_length, 0), color), // 6
		VertexFormat(corner + glm::vec3(shuriken_length / 2, shuriken_length, 0), color), // 7
		VertexFormat(corner + glm::vec3(0, shuriken_length, 0), color), // 8

	};
	
	Mesh* shuriken = new Mesh(name);
	std::vector<unsigned short> indices = {0, 1, 2,
										   3, 4, 1,
										   1, 5, 6,
	                                       1, 7, 8};
	
	if (!fill) {
		shuriken->SetDrawMode(GL_LINE_LOOP);
		shuriken->InitFromData(vertices, indices);
	}
	else {
		shuriken->InitFromData(vertices, indices);
	}
	return shuriken;
}

Mesh* Object2D::CreateBallon(std::string name, glm::vec3 leftBottomCorner, float ballon_height, float ballon_length, glm::vec3 color, bool fill )
{
	glm::vec3 corner = leftBottomCorner;

	float b = ballon_height / 2; // elipse semi-minor axis
	float a = ballon_length / 2; // elipse semi-major axis

	std::vector<VertexFormat> vertices ;
	float i;
	vertices.push_back(corner);
	for (i = -b; i <= b; i++) {
		//elipse parametric representation : (x,y) = (a*cos(t), b*sin(t))
		float y = i; // current y 
		float x = a * cos(asin(y / b)); // curent x in parametric representation 
		vertices.push_back(VertexFormat(corner + glm::vec3(x, y, 0), color)); // add vertex of (x,y)
	}
	
	for (i = b; i >= -b; i--) {
		//elipse parametric representation : (x,y) = (a*cos(t), b*sin(t))
		float y = i; // current y 
		float x = a * cos(asin(y / b)); // curent x in parametric representation 
		vertices.push_back(VertexFormat(corner + glm::vec3(-x, y, 0), color)); // add vertex of (x,y)
	}
	Mesh* ballon = new Mesh(name);
	std::vector<unsigned short> indices;

	//draw only the elipse
	if (!fill) {
		int i;
		for (i = 1; i < vertices.size(); i++) {
			indices.push_back(i);
		}
		ballon->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		int i;
		for (i = 1; i < vertices.size() - 1; i++) {
			indices.push_back(0);
			indices.push_back(i);
			indices.push_back(i + 1);
		}

		
	}

	//lower triangle
	float triangle_height = 5;
	vertices.push_back(VertexFormat(corner + glm::vec3(0, -b, 0), color));
	vertices.push_back(VertexFormat(corner + glm::vec3(-triangle_height, -b -triangle_height, 0), color));
	vertices.push_back(VertexFormat(corner + glm::vec3(triangle_height, -b -triangle_height, 0), color));

	//draw lower tirangle
	if (!fill) {
		int size = vertices.size();
		indices.push_back(size - 3);
		indices.push_back(size - 2);
		indices.push_back(size - 1);
		ballon->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		int size = vertices.size();
		indices.push_back(size - 3);
		indices.push_back(size - 2);
		indices.push_back(size - 1);
	}
	
	//poli Line
	float line_width = 1;
	float line_height = 10;
	float line_length = 10; 
	vertices.push_back(VertexFormat(corner + glm::vec3(-line_width/2, -b - triangle_height, 0), color));
	vertices.push_back(VertexFormat(corner + glm::vec3(line_width/2, -b - triangle_height, 0), color));

	vertices.push_back(VertexFormat(corner + glm::vec3(-line_length - line_width/2, -b - triangle_height -line_height, 0), color));
	vertices.push_back(VertexFormat(corner + glm::vec3(-line_length + line_width, -b - triangle_height - line_height, 0), color));

	//draw line from bottom of ballon
	if (!fill) {
		int size = vertices.size();
		indices.push_back(size - 4);
		indices.push_back(size - 3);
		indices.push_back(size - 2);
		indices.push_back(size - 1);
		ballon->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		int size = vertices.size();
		indices.push_back(size - 4);
		indices.push_back(size - 2);
		indices.push_back(size - 3);

		indices.push_back(size - 3);
		indices.push_back(size - 2);
		indices.push_back(size - 1);
	}

	ballon->InitFromData(vertices, indices);
	return ballon;	
}

Mesh* Object2D::CreatePowerBar(std::string name, glm::vec3 leftBottomCorner, float length, float height, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, height, 0), color),
		VertexFormat(corner + glm::vec3(0, height, 0), color)
	};

	Mesh* power_bar = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };
	if (!fill) {
		power_bar->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		indices.push_back(0);
		indices.push_back(2);
	}

	power_bar->InitFromData(vertices, indices);
	return power_bar;
}

Mesh* Object2D::CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, float height,  glm::vec3 color, bool fill) {
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, height, 0), color),
		VertexFormat(corner + glm::vec3(0, height, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Object2D::CreateHeart(std::string name, glm::vec3 leftBottomCorner, float height, float length, glm::vec3 color, bool fill) {
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(0, height / 4, 0), color),
		VertexFormat(corner + glm::vec3(length / 4, height / 2, 0), color),
		VertexFormat(corner + glm::vec3(length / 2, height / 4, 0), color),
		VertexFormat(corner + glm::vec3(length / 2, 0, 0), color),
		VertexFormat(corner + glm::vec3(0, -height / 2, 0), color),
		VertexFormat(corner + glm::vec3(-length / 2, 0, 0), color),
		VertexFormat(corner + glm::vec3(-length / 2, height / 4, 0), color),
		VertexFormat(corner + glm::vec3(-length / 4, height / 2, 0), color)
	};

	Mesh* heart = new Mesh(name);
	if (!fill) {
		heart->SetDrawMode(GL_LINE_LOOP);
		std::vector<unsigned short> indices = { 1, 2, 3, 4, 5, 6, 7, 8, 1 };
		heart->InitFromData(vertices, indices);
	}
	else {
		std::vector<unsigned short> indices = { 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 5, 6, 0, 6, 7, 0, 7, 8, 0, 8, 1 };
		heart->InitFromData(vertices, indices);
	}
	return heart;
}

Mesh* Object2D::CreateDigit(std::string name, glm::vec3 leftBottomCorner, float height, float length, glm::vec3 color, int digit) {
	glm::vec3 corner = leftBottomCorner;
	Mesh* mesh = new Mesh(name);
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, height / 2, 0), color),
		VertexFormat(corner + glm::vec3(length, height, 0), color),
		VertexFormat(corner + glm::vec3(0, height , 0), color),
		VertexFormat(corner + glm::vec3(0, height/2 , 0), color)
	};
	if (digit == 0) {
		std::vector<unsigned short> indices = { 0, 1, 3, 4 };
		mesh->SetDrawMode(GL_LINE_LOOP);
		mesh->InitFromData(vertices, indices);
	}
	if (digit == 1) {
		std::vector<unsigned short> indices = { 1, 3 };
		mesh->SetDrawMode(GL_LINE_STRIP);
		mesh->InitFromData(vertices, indices);
	}
	if (digit == 2) {
		std::vector<unsigned short> indices = { 1, 0, 5, 2, 3, 4 };
		mesh->SetDrawMode(GL_LINE_STRIP);
		mesh->InitFromData(vertices, indices);
	}
	if (digit == 3) {
		std::vector<unsigned short> indices = { 0, 1, 2, 5, 2, 3, 4 };
		mesh->SetDrawMode(GL_LINE_STRIP);
		mesh->InitFromData(vertices, indices);
	}
	if (digit == 4) {
		std::vector<unsigned short> indices = { 1, 3, 2, 5, 4 };
		mesh->SetDrawMode(GL_LINE_STRIP);
		mesh->InitFromData(vertices, indices);
	}
	if (digit == 5) {
		std::vector<unsigned short> indices = { 0, 1, 2, 5, 4, 3 };
		mesh->SetDrawMode(GL_LINE_STRIP);
		mesh->InitFromData(vertices, indices);
	}
	if (digit == 6) {
		std::vector<unsigned short> indices = { 5, 0, 1, 2, 5, 4, 3 };
		mesh->SetDrawMode(GL_LINE_STRIP);
		mesh->InitFromData(vertices, indices);
	}
	if (digit == 7) {
		std::vector<unsigned short> indices = { 1, 3, 4 };
		mesh->SetDrawMode(GL_LINE_STRIP);
		mesh->InitFromData(vertices, indices);
	}
	if (digit == 8) {
		std::vector<unsigned short> indices = { 2, 5, 0, 1, 3, 4, 5 };
		mesh->SetDrawMode(GL_LINE_STRIP);
		mesh->InitFromData(vertices, indices);
	}
	if (digit == 9) {
		std::vector<unsigned short> indices = { 2, 5, 4, 3, 1, 0 };
		mesh->SetDrawMode(GL_LINE_STRIP);
		mesh->InitFromData(vertices, indices);
	}
	return mesh;

}

Mesh* Object2D::CreateLetter(std::string name, glm::vec3 leftBottomCorner, float height, float length, glm::vec3 color, int letter) {
	glm::vec3 corner = leftBottomCorner;
	Mesh* mesh = new Mesh(name);
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, height / 2, 0), color),
		VertexFormat(corner + glm::vec3(length, height, 0), color),
		VertexFormat(corner + glm::vec3(0, height , 0), color),
		VertexFormat(corner + glm::vec3(0, height / 2 , 0), color)
	};
	if ( letter == 0) { // 0 code for s
		std::vector<unsigned short> indices = { 0, 1, 2, 5, 4, 3 };
		mesh->SetDrawMode(GL_LINE_STRIP);
		mesh->InitFromData(vertices, indices);
	}
	if (letter == 1) { // 1 code for c
		std::vector<unsigned short> indices = { 3, 4, 0, 1 };
		mesh->SetDrawMode(GL_LINE_STRIP);
		mesh->InitFromData(vertices, indices);
	}
	if (letter == 2) { // 2 code for o
		std::vector<unsigned short> indices = { 4, 0, 1, 3 };
		mesh->SetDrawMode(GL_LINE_LOOP);
		mesh->InitFromData(vertices, indices);
	}
	if (letter == 3) { // 3 code for r
		std::vector<unsigned short> indices = { 1, 5, 2, 3, 4, 0 };
		mesh->SetDrawMode(GL_LINE_STRIP);
		mesh->InitFromData(vertices, indices);
	}
	if (letter == 4) { // 4 code for e
		std::vector<unsigned short> indices = { 3, 4, 5, 2, 5, 0, 1 };
		mesh->SetDrawMode(GL_LINE_STRIP);
		mesh->InitFromData(vertices, indices);
	}
	if (letter == 5) { // 5 code for =
		std::vector<VertexFormat> vertices1 =
		{
			VertexFormat(corner + glm::vec3(0, height / 4, 0), color),
			VertexFormat(corner + glm::vec3(length, height / 4, 0), color)
		};
		std::vector<unsigned short> indices1 = { 0, 1 };
		mesh->SetDrawMode(GL_LINE_STRIP);
		mesh->InitFromData(vertices1, indices1);

	}
	if (letter == 6) { // 6 code for =
		std::vector<VertexFormat> vertices1 =
		{
			VertexFormat(corner + glm::vec3(0, 3 * height / 4, 0), color),
			VertexFormat(corner + glm::vec3(length, 3 * height / 4, 0), color)
		};
		std::vector<unsigned short> indices1 = { 0, 1 };
		mesh->SetDrawMode(GL_LINE_STRIP);
		mesh->InitFromData(vertices1, indices1);

	}
	return mesh;
}