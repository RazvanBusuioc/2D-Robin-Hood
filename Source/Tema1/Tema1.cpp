#include "Tema1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"

using namespace std;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
	resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);
	
	/* Create Meshes */
	CreateMeshes();

	/* Create objects */
	CreateBowObject();
	CreateArrowObject();
	CreatePowerBarObject();
}

void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{	
	/* update the timings: seconds betweekn arrows, ballons, etc */
	update_timings(deltaTimeSeconds);

	/* Check if the player has any lives left and update the level if needed */
	update_game();

	/* check arrow-ballons, arrow-shuriken, shuriken-bow collisions */
	check_collisions();

	/* render the bow */
	render_bow();

	/* render the arrow in the bow if possible */
	render_main_arrow();

	/* render the powerbar */
	render_powerbar();

	/* render the hp of the player */
	render_hearts();

	/* render the score */
	render_score();

	/* render the scorebar strip */
	render_scorebar();

	/* render red ballons, one at SECONDS_BETWEEN_RED_BALLONS  */
	render_red_ballons(deltaTimeSeconds);

	/* render yellow ballons, one at SECONDS_BETWEEN_YELLOW_BALLONS  */
	render_yellow_ballons(deltaTimeSeconds);

	/* render shurikens, one at SECONDS_BETWEEN_SHURIKENS */
	render_shurikens(deltaTimeSeconds);

	/* render shooted arrows */
	render_arrows(deltaTimeSeconds);

}

void Tema1::FrameEnd()
{

}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_W)) {
		/* when w pressed the bow position on y axis grows until it hits the scorebar*/
		if (bow->get_bow_pos_y() + bow->get_bow_height() / 2  + SCOREBAR_HEIGHT < resolution.y) {
			bow->increase_pos_y(BOW_MOVEMENT_TRANSLATE_FACTOR * deltaTime);
			main_arrow->increase_pos_y(BOW_MOVEMENT_TRANSLATE_FACTOR * deltaTime);
		}
	}
	if (window->KeyHold(GLFW_KEY_S)) {
		/* when s pressed the bow position on y axis reduces until it hits the powerbar*/
		if (bow->get_bow_pos_y() - bow->get_bow_height() / 2 - (POWER_BAR_HEIGHT + POWER_BAR_POS_Y)> 0) {
			bow->increase_pos_y(-BOW_MOVEMENT_TRANSLATE_FACTOR * deltaTime);
			main_arrow->increase_pos_y(-BOW_MOVEMENT_TRANSLATE_FACTOR * deltaTime);
		}	
	}
	if (window->MouseHold(GLFW_MOUSE_BUTTON_1)) {
		/* while left clock pressed power for the arrow grows and the powerbar grows aswell*/
		if (time_mouse_pressed < SECONDS_FOR_MAX_POWER && seconds_since_arrow_fired >= SECONDS_BETWEEN_ARROWS) {
			time_mouse_pressed += deltaTime;
			power_bar->increase_scale_x(POWER_BAR_SCALE_FACTOR * deltaTime);
		}
	}
}

void Tema1::OnKeyPress(int key, int mods)
{
	// add key press event
}

void Tema1::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{	
	/* compute the angle of the bow and main arrow based the mouse position */
	if (mouseX < BOW_LEFT_PADDING) {
		return;
	}
	float b = mouseX - BOW_LEFT_PADDING;
	float a = ((float)window->GetResolution().y - (bow->get_bow_pos_y()/(float)resolution.y) * window->GetResolution().y)  - mouseY;
	float radians = atan(a / b);
	bow->set_bow_radians(radians);
	main_arrow->set_arrow_radians(radians);
	
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_2 && seconds_since_arrow_fired >= SECONDS_BETWEEN_ARROWS) {
		/* 
		* if left click was released and enough time has passed in order for an arrow to reload
		* the main arrow will be shoted. All the timing related to the arrow will be updated and
		* all the objects aswell
		*/
		if (time_mouse_pressed > SECONDS_FOR_MAX_POWER) {
			time_mouse_pressed = SECONDS_FOR_MAX_POWER;
		}
		main_arrow->set_arrow_velocity(ARROW_VELOCITY_FACTOR * time_mouse_pressed );
		shooted_arrows.push_back(main_arrow);
		CreateArrowObject();
		time_mouse_pressed = 1;
		seconds_since_arrow_fired = 0;
		power_bar->set_scale_x(1);
	}
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}


void Tema1::CreateMeshes() {
	/* Create all the meshed using the functions from Objesc2D class */
	bow_mesh = Object2D::CreateBow("bow", corner, BOW_HEIGHT, BOW_LEN, BOW_COLOR);
	arrow_mesh = Object2D::CreateArrow("arrow", corner, ((float)4 / 5) * ARROW_LEN, (float)ARROW_TIP_WIDTH / 10,
		((float)1 / 5) * ARROW_LEN, ARROW_TIP_WIDTH, ARROW_COLOR);
	shuriken_mesh = Object2D::CreateShuriken("shuriken", corner, SHURIKEN_LEN, SHURIKEN_COLOR);
	red_ballon_mesh = Object2D::CreateBallon("red_ballon", corner, BALLON_HEIGHT, BALLON_WIDTH, RED_BALLON_COLOR);
	yellow_ballon_mesh = Object2D::CreateBallon("yellow_ballon", corner, BALLON_HEIGHT, BALLON_WIDTH, YELLOW_BALLON_COLOR);
	power_bar_mesh = Object2D::CreatePowerBar("power_bar", corner, POWER_BAR_LENGTH, POWER_BAR_HEIGHT, POWER_BAR_COLOR);
	heart_mesh = Object2D::CreateHeart("heart", corner, 40, 40, HEART_COLOR);
	digit0 = Object2D::CreateDigit("digit0", corner, DIGIT_HEIGHT, DIGIT_WIDTH, TEXT_COLOR, 0);
	digit1 = Object2D::CreateDigit("digit1", corner, DIGIT_HEIGHT, DIGIT_WIDTH, TEXT_COLOR, 1);
	digit2 = Object2D::CreateDigit("digit2", corner, DIGIT_HEIGHT, DIGIT_WIDTH, TEXT_COLOR, 2);
	digit3 = Object2D::CreateDigit("digit3", corner, DIGIT_HEIGHT, DIGIT_WIDTH, TEXT_COLOR, 3);
	digit4 = Object2D::CreateDigit("digit4", corner, DIGIT_HEIGHT, DIGIT_WIDTH, TEXT_COLOR, 4);
	digit5 = Object2D::CreateDigit("digit5", corner, DIGIT_HEIGHT, DIGIT_WIDTH, TEXT_COLOR, 5);
	digit6 = Object2D::CreateDigit("digit6", corner, DIGIT_HEIGHT, DIGIT_WIDTH, TEXT_COLOR, 6);
	digit7 = Object2D::CreateDigit("digit7", corner, DIGIT_HEIGHT, DIGIT_WIDTH, TEXT_COLOR, 7);
	digit8 = Object2D::CreateDigit("digit8", corner, DIGIT_HEIGHT, DIGIT_WIDTH, TEXT_COLOR, 8);
	digit9 = Object2D::CreateDigit("digit9", corner, DIGIT_HEIGHT, DIGIT_WIDTH, TEXT_COLOR, 9);
	letter_s = Object2D::CreateLetter("s", corner, DIGIT_HEIGHT, DIGIT_WIDTH, TEXT_COLOR, 0);
	letter_c = Object2D::CreateLetter("c", corner, DIGIT_HEIGHT, DIGIT_WIDTH, TEXT_COLOR, 1);
	letter_o = Object2D::CreateLetter("o", corner, DIGIT_HEIGHT, DIGIT_WIDTH, TEXT_COLOR, 2);
	letter_r = Object2D::CreateLetter("r", corner, DIGIT_HEIGHT, DIGIT_WIDTH, TEXT_COLOR, 3);
	letter_e = Object2D::CreateLetter("e", corner, DIGIT_HEIGHT, DIGIT_WIDTH, TEXT_COLOR, 4);
	equal1 = Object2D::CreateLetter("e1", corner, DIGIT_HEIGHT, DIGIT_WIDTH, TEXT_COLOR, 5);
	equal2 = Object2D::CreateLetter("e2", corner, DIGIT_HEIGHT, DIGIT_WIDTH, TEXT_COLOR, 6);
	square = Object2D::CreateSquare("square", corner, SCOREBAR_LENGTH, SCOREBAR_HEIGHT, SCOREBAR_COLOR);

	/* add meshes to list */
	AddMeshToList(bow_mesh);
	AddMeshToList(arrow_mesh);
	AddMeshToList(shuriken_mesh);
	AddMeshToList(red_ballon_mesh);
	AddMeshToList(yellow_ballon_mesh);
	AddMeshToList(power_bar_mesh);
	AddMeshToList(heart_mesh);
	AddMeshToList(square);
	AddMeshToList(digit0);
	AddMeshToList(digit1); 
	AddMeshToList(digit2); 
	AddMeshToList(digit3);
	AddMeshToList(digit4);
	AddMeshToList(digit5);
	AddMeshToList(digit6);
	AddMeshToList(digit7);
	AddMeshToList(digit8);
	AddMeshToList(digit9);
	AddMeshToList(letter_s);
	AddMeshToList(letter_c);
	AddMeshToList(letter_o);
	AddMeshToList(letter_r);
	AddMeshToList(letter_e);
	AddMeshToList(equal1);
	AddMeshToList(equal2);
}

/* creates a new main arrow in order to rend it with the bow */
void Tema1::CreateArrowObject() {
	main_arrow = new Arrow("arrow", ARROW_LEN, ARROW_TIP_WIDTH, ARROW_COLOR);
	main_arrow->set_arrow_mesh(arrow_mesh);
	main_arrow->set_arrow_positions(BOW_LEFT_PADDING, bow->get_bow_pos_y());
	main_arrow->set_arrow_radians(bow->get_bow_radians());
}

/* Creates the bow object */
void Tema1::CreateBowObject() {
	bow = new Bow("bow", BOW_HEIGHT, BOW_LEN, BOW_COLOR);
	bow->set_bow_mesh(bow_mesh);
	bow->set_bow_positions(BOW_LEFT_PADDING, (float)resolution.y / 2);
}

/* Creates the power bar object */
void Tema1::CreatePowerBarObject() {
	power_bar = new PowerBar("power_bar", POWER_BAR_HEIGHT, POWER_BAR_LENGTH);
	power_bar->set_mesh(power_bar_mesh);
	power_bar->set_positions(POWER_BAR_POS_X, POWER_BAR_POS_Y);
}

/* Creates the shuriken object and returns it */
Shuriken* Tema1::CreateShurikenObject() {
	shuriken = new Shuriken("shuriken", SHURIKEN_LEN, SHURIKEN_COLOR);
	shuriken->set_shuriken_mesh(shuriken_mesh);
	shuriken->set_shuriken_positions(300, 300);
	return shuriken;
}

/* Creates the red ballon object and returns it*/
Ballon* Tema1::CreateRedBallonObject() {
	red_ballon = new Ballon("red_ballon", BALLON_HEIGHT, BALLON_WIDTH, RED_BALLON_COLOR);
	red_ballon->set_ballon_mesh(red_ballon_mesh);
	return red_ballon;
}

/* Creates the yellow ballon object and returns it*/
Ballon* Tema1::CreateYellowBallonObject() {
	red_ballon = new Ballon("yellow_ballon", BALLON_HEIGHT, BALLON_WIDTH, RED_BALLON_COLOR);
	red_ballon->set_ballon_mesh(red_ballon_mesh);
	return red_ballon;
}




void Tema1::render_red_ballons(float deltaTimeSeconds) {
	if (seconds_since_red_ballon_thrown >= seconds_between_red_ballons &&
		(this->game_level == 1 || this->game_level == 2 || this->game_level == 4 || this->game_level == 6)) {
		/* if the current level allows for red ballon rendering, create a new balon object with random position on x axis 
		*  and store it in the vector of red ballons that need to be rendered */
		seconds_since_red_ballon_thrown = 0;
		Ballon* aux = CreateRedBallonObject();
		aux->set_ballon_positions((rand() % (resolution.x - BALLONS_FIELD_OFFSET - BALLON_WIDTH)) + BALLONS_FIELD_OFFSET, -BALLON_HEIGHT);
		red_ballons.push_back(aux);
	}
	for (int i = 0; i < red_ballons.size(); i++) {
		/* iterate in the ballons vector */
		if (red_ballons[i]->get_ballon_pos_x() > resolution.x || red_ballons[i]->get_ballon_pos_x() < 0 ||
			red_ballons[i]->get_ballon_pos_y() - BALLON_HEIGHT + 1.5 * SCOREBAR_HEIGHT > resolution.y ||
			red_ballons[i]->get_ballon_pos_y() + BALLON_HEIGHT < 0)
		{
			/* check if a ballon is out of the screen. If true, don`t store it anymore */
			red_ballons.erase(red_ballons.begin() + i);
			i--;
			continue;
		}
		if (red_ballons[i]->get_ballon_scale() <= 0) {
			/* if the scaling process of the ballon that was hit is done, erase it from the vector*/
			red_ballons.erase(red_ballons.begin() + i);
			i--;
			continue;
		}

		/* render the ballon */
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(red_ballons[i]->get_ballon_pos_x(), red_ballons[i]->get_ballon_pos_y());
		modelMatrix *= Transform2D::Scale(red_ballons[i]->get_ballon_scale(), red_ballons[i]->get_ballon_scale());
		RenderMesh2D(meshes["red_ballon"], shaders["VertexColor"], modelMatrix);

		
		if (red_ballons[i]->get_ballon_scale() < 1) {
			/* if the cuurent ballon was hit, scale it down*/
			red_ballons[i]->decrease_ballon_scale(BALLONS_SCALE_FACTOR * deltaTimeSeconds);
		}
		else {
			/* otherwise, let the ballon gow up in the air */
			red_ballons[i]->increase_pos_y(ballons_velocity_factor * deltaTimeSeconds);
		}
	}
}

void Tema1::render_yellow_ballons(float deltaTimeSeconds) {
	if (seconds_since_yellow_ballon_thrown >= seconds_between_yellow_ballons &&
		(this->game_level == 2 || this->game_level == 4 || this->game_level == 6)) {
		/* if the current level allows for yellow ballon rendering, create a new balon object with random position on x axis
		*  and store it in the vector of yellow ballons that need to be rendered */
		seconds_since_yellow_ballon_thrown = 0;
		Ballon* aux = CreateYellowBallonObject();
		aux->set_ballon_positions((rand() % (resolution.x - BALLONS_FIELD_OFFSET - BALLON_WIDTH)) + BALLONS_FIELD_OFFSET, -BALLON_HEIGHT);
		yellow_ballons.push_back(aux);
	}
	for (int i = 0; i < yellow_ballons.size(); i++) {
		/* iterate in the ballons vector */
		if (yellow_ballons[i]->get_ballon_pos_x() > resolution.x || yellow_ballons[i]->get_ballon_pos_x() < 0 ||
			yellow_ballons[i]->get_ballon_pos_y() - BALLON_HEIGHT + 1.5 * SCOREBAR_HEIGHT > resolution.y ||
			yellow_ballons[i]->get_ballon_pos_y() + BALLON_HEIGHT < 0)
		{
			/* check if a ballon is out of the screen. If true, don`t store it anymore */
			yellow_ballons.erase(yellow_ballons.begin() + i);
			i--;
			continue;
		}
		if (yellow_ballons[i]->get_ballon_scale() <= 0) {
			/* if the scaling process of the ballon that was hit is done, erase it from the vector*/
			yellow_ballons.erase(yellow_ballons.begin() + i);
			i--;
			continue;
		}
		/* render the ballon */
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(yellow_ballons[i]->get_ballon_pos_x(), yellow_ballons[i]->get_ballon_pos_y());
		modelMatrix *= Transform2D::Scale(yellow_ballons[i]->get_ballon_scale(), yellow_ballons[i]->get_ballon_scale());
		RenderMesh2D(meshes["yellow_ballon"], shaders["VertexColor"], modelMatrix);

		if (yellow_ballons[i]->get_ballon_scale() < 1) {
			/* if the cuurent ballon was hit, scale it down*/
			yellow_ballons[i]->decrease_ballon_scale(BALLONS_SCALE_FACTOR * deltaTimeSeconds);
		}
		else {
			/* otherwise, let the ballon gow up in the air */
			yellow_ballons[i]->increase_pos_y(ballons_velocity_factor * deltaTimeSeconds);
		}
	}
}

void Tema1::render_shurikens(float deltaTimeSeconds) {
	if (seconds_since_shuriken_thrown >= seconds_between_shurikens &&
		(this->game_level == 3 || this->game_level == 5 || this->game_level == 6)) {
		/* if the current level allows for shuriken rendering, create a new shuriken object with random position on y axis
		*  and store it in the vector of shurikens that need to be rendered */
		seconds_since_shuriken_thrown = 0;
		Shuriken* aux = CreateShurikenObject();
		aux->set_shuriken_positions(resolution.x + SHURIKEN_LEN, (rand() % (resolution.y - SHURIKEN_DOWN_PADDING - SHURIKEN_UP_PADDING - 2 * SHURIKEN_LEN)) + SHURIKEN_DOWN_PADDING);
		shurikens.push_back(aux);
	}
	for (int i = 0; i < shurikens.size(); i++) {
		/* iterate in the ballons vector */
		if (shurikens[i]->get_shuriken_pos_x() < -SHURIKEN_LEN || shurikens[i]->get_shuriken_pos_x() > resolution.x + SHURIKEN_LEN ||
			shurikens[i]->get_shuriken_pos_y() < 0 || shurikens[i]->get_shuriken_pos_y() > resolution.y) {
			/* check if a shuriken is out of the screen. If true, don`t store it anymore */
			shurikens.erase(shurikens.begin() + i);
			i--;
			continue;
		}
		if (shurikens[i]->get_shuriken_scale() <= 0) {
			/* if the scaling process of the shuriken that was hit is done, erase it from the vector*/
			shurikens.erase(shurikens.begin() + i);
			i--;
			continue;
		}
		/* render the shuriken */
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(shurikens[i]->get_shuriken_pos_x() + SHURIKEN_LEN / 2,
											  shurikens[i]->get_shuriken_pos_y() + SHURIKEN_LEN / 2);
		modelMatrix *= Transform2D::Rotate(shurikens[i]->get_shuriken_radians());
		modelMatrix *= Transform2D::Scale(shurikens[i]->get_shuriken_scale(), shurikens[i]->get_shuriken_scale());
		modelMatrix *= Transform2D::Translate(-SHURIKEN_LEN / 2, -SHURIKEN_LEN / 2);
		RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrix);

		if (shurikens[i]->get_shuriken_scale() < 1) {
			/* if the cuurent shuriken was hit, scale it down*/
			shurikens[i]->decrease_shuriken_scale(BALLONS_SCALE_FACTOR * deltaTimeSeconds);
		}
		else {
			/* otherwise, the shuriken will advance to the left */
			shurikens[i]->decrease_pos_x(shuriken_velocity_factor * deltaTimeSeconds);
			shurikens[i]->increase_shuriken_radians(shuriken_rotation_factor * deltaTimeSeconds);
		}
	}
}

void Tema1::render_arrows(float deltaTimeSeconds) {
	for (int i = 0; i < shooted_arrows.size(); i++) {
		/* iterte in the shooted arrows vector */
		if (shooted_arrows[i]->get_arrow_pos_x() > resolution.x || shooted_arrows[i]->get_arrow_pos_y() > resolution.y ||
			shooted_arrows[i]->get_arrow_pos_x() < 0 || shooted_arrows[i]->get_arrow_pos_y() < 0)
		{
			/* check if an arrow is out of the screen. If true, don`t store it anymore */
			shooted_arrows.erase(shooted_arrows.begin() + i);
			i--;
			continue;
		}

		/* Create the model matrix */
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(shooted_arrows[i]->get_arrow_pos_x(), shooted_arrows[i]->get_arrow_pos_y());
		modelMatrix *= Transform2D::Rotate(shooted_arrows[i]->get_arrow_radians());

		if (this->game_level >= GRAVITY_ARROW_LVL) {
			/* the current level allows for arrow that use the gravity concept */
			shooted_arrows[i]->arrow_moving(ARROW_ANGLE_FACTOR * deltaTimeSeconds, true);
		}
		else {
			/* the current level doesn t allow for gravity usage */
			shooted_arrows[i]->arrow_moving(-1, false);
		}

		/* render the arrow */
		RenderMesh2D(meshes["arrow"], shaders["VertexColor"], modelMatrix);
	}
}

void Tema1::render_hearts() {
	/* render the hp of the player */
	for (int i = 0; i < player_lives; i++) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(HEARTS_POS_X + 4.5*i * HEART_LEN, HEARTS_POS_Y);
		RenderMesh2D(meshes["heart"], shaders["VertexColor"], modelMatrix);
	}
}

void Tema1::render_score() {
	/* render the score of the player using 7 segment digits */
	int score = this->player_score;
	int digits = 0;
	do {
		int digit = score % 10;
		score /= 10;
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(SCORE_POS_X - 2*digits*DIGIT_WIDTH, SCORE_POS_Y);
		if (digit == 0) {
			RenderMesh2D(meshes["digit0"], shaders["VertexColor"], modelMatrix);
		}
		if (digit == 1) {
			RenderMesh2D(meshes["digit1"], shaders["VertexColor"], modelMatrix);
		}
		if (digit == 2) {
			RenderMesh2D(meshes["digit2"], shaders["VertexColor"], modelMatrix);
		}
		if (digit == 3) {
			RenderMesh2D(meshes["digit3"], shaders["VertexColor"], modelMatrix);
		}
		if (digit == 4) {
			RenderMesh2D(meshes["digit4"], shaders["VertexColor"], modelMatrix);
		}
		if (digit == 5) {
			RenderMesh2D(meshes["digit5"], shaders["VertexColor"], modelMatrix);
		}
		if (digit == 6) {
			RenderMesh2D(meshes["digit6"], shaders["VertexColor"], modelMatrix);
		}
		if (digit == 7) {
			RenderMesh2D(meshes["digit7"], shaders["VertexColor"], modelMatrix);
		}
		if (digit == 8) {
			RenderMesh2D(meshes["digit8"], shaders["VertexColor"], modelMatrix);
		}
		if (digit == 9) {
			RenderMesh2D(meshes["digit9"], shaders["VertexColor"], modelMatrix);
		}
		digits++;
	} while (score > 0);

	/*  render "score=" using 7 segment letters created previously*/
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(SCORE_POS_X - 2 * digits * DIGIT_WIDTH, SCORE_POS_Y);
	RenderMesh2D(meshes["e1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(SCORE_POS_X - 2 * digits++ * DIGIT_WIDTH, SCORE_POS_Y);
	RenderMesh2D(meshes["e2"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(SCORE_POS_X - 2 * digits++ * DIGIT_WIDTH, SCORE_POS_Y);
	RenderMesh2D(meshes["e"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(SCORE_POS_X - 2 * digits++ * DIGIT_WIDTH, SCORE_POS_Y);
	RenderMesh2D(meshes["r"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(SCORE_POS_X - 2 * digits++ * DIGIT_WIDTH, SCORE_POS_Y);
	RenderMesh2D(meshes["o"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(SCORE_POS_X - 2 * digits++ * DIGIT_WIDTH, SCORE_POS_Y);
	RenderMesh2D(meshes["c"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(SCORE_POS_X - 2 * digits++ * DIGIT_WIDTH, SCORE_POS_Y);
	RenderMesh2D(meshes["s"], shaders["VertexColor"], modelMatrix);
}

void Tema1::render_bow() {
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(bow->get_bow_pos_x(), bow->get_bow_pos_y());
	modelMatrix *= Transform2D::Rotate(bow->get_bow_radians());
	RenderMesh2D(meshes["bow"], shaders["VertexColor"], modelMatrix);}

void Tema1::render_scorebar() {
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(0, 720- SCOREBAR_HEIGHT);
	RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);
}

void Tema1::render_powerbar() {
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(power_bar->get_pos_x(), power_bar->get_pos_y());
	modelMatrix *= Transform2D::Scale(power_bar->get_scale_x(), 1);
	RenderMesh2D(meshes["power_bar"], shaders["VertexColor"], modelMatrix);
}

void Tema1::render_main_arrow() {
	/* render the main arrow only if enough seconds have passsed */
	if (seconds_since_arrow_fired >= SECONDS_BETWEEN_ARROWS) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(main_arrow->get_arrow_pos_x(), main_arrow->get_arrow_pos_y());
		modelMatrix *= Transform2D::Rotate(main_arrow->get_arrow_radians());
		RenderMesh2D(meshes["arrow"], shaders["VertexColor"], modelMatrix);
	}
}


void Tema1::check_collisions() {
	for (int i = 0; i < shooted_arrows.size(); i++) {
		/* for every arrow that was shooted */
		for (int j = 0; j < red_ballons.size(); j++) {
			/* for every red ballon in the screen */
			if (red_ballons[j]->check_collision(shooted_arrows[i]->get_tip_pos_x(), shooted_arrows[i]->get_tip_pos_y()) &&
				red_ballons[j]->get_ballon_scale() == 1) {
				/* arrow - red ballon collision detected, start scaling down the ballon and update the score*/
				red_ballons[j]->decrease_ballon_scale(0.01);
				player_score += RED_BALLON_SCORE;
				print_score();
			}
		}
		for (int j = 0; j < yellow_ballons.size(); j++) {
			/* for every yellow ballon in the screen */
			if (yellow_ballons[j]->check_collision(shooted_arrows[i]->get_tip_pos_x(), shooted_arrows[i]->get_tip_pos_y()) &&
				yellow_ballons[j]->get_ballon_scale() == 1) {
				/* arrow - yellow ballon collision detected, start scaling down the ballon and update the score*/
				yellow_ballons[j]->decrease_ballon_scale(0.01);
				player_score += YELLOW_BALLON_SCORE;
				print_score();
			}
		}
		for (int j = 0; j < shurikens.size(); j++) {
			/* for every shuriken in the screen */
			if (shurikens[j]->check_collision(shooted_arrows[i]->get_tip_pos_x(), shooted_arrows[i]->get_tip_pos_y()) &&
				shurikens[j]->get_shuriken_scale() == 1) {
				/* arrow - shuriken collision was detected, start scaling down the shuriken and update the score */
				shurikens[j]->decrease_shuriken_scale(0.01);
				player_score += SHURIKEN_SCORE;
				print_score();
			}
		}
	}
	/* check for bow - shuriken collisions*/
	for (int i = 0; i < shurikens.size(); i++) {
		if (bow->check_collision(shurikens[i]->get_shuriken_pos_x() + SHURIKEN_LEN / 2, shurikens[i]->get_shuriken_pos_y() + SHURIKEN_LEN / 2,
			SHURIKEN_LEN * sqrt(2) / 2) && shurikens[i]->get_shuriken_scale() == 1) {
			/* collision detected, decrease the hp of the player and scale down the shuriken*/
			this->player_lives -= 1;
			cout << "Remaining lives: " << this->player_lives << "\n";
			shurikens[i]->decrease_shuriken_scale(0.01);
		}
	}
}

void Tema1::print_score() {
	cout << "Score: " << this->player_score << "\n";
}

/* check if the players has any hp left and update the current level*/
void Tema1::update_game() {
	if (this->player_lives == 0) {
		cout << "GAME OVER!\n";
		exit(0);
	}
	if (this->player_score < LVL1_MAX_SCORE) { // lvl 1
		this->game_level = 1;
		this->seconds_between_red_ballons = SECONDS_BETWEEN_RED_BALLONS_L1;
		this->seconds_between_yellow_ballons = SECONDS_BETWEEN_YELLOW_BALLONS_L1;
		this->seconds_between_shurikens = SECONDS_BETWEEN_SHURIKENS_L1;
		this->ballons_velocity_factor = BALLONS_VELOCITY_FACTOR_L1;
		this->shuriken_velocity_factor = SHURIKEN_VELOCITY_FACTOR_L1;
		this->shuriken_rotation_factor = SHURIKEN_ROTATION_FACTOR_L1;
	}
	else if (this->player_score > LVL1_MAX_SCORE && this->player_score < LVL2_MAX_SCORE) { // lvl 2
		this->game_level = 2;
		this->seconds_between_red_ballons = SECONDS_BETWEEN_RED_BALLONS_L2;
		this->seconds_between_yellow_ballons = SECONDS_BETWEEN_YELLOW_BALLONS_L2;
		this->seconds_between_shurikens = SECONDS_BETWEEN_SHURIKENS_L2;
		this->ballons_velocity_factor = BALLONS_VELOCITY_FACTOR_L2;
		this->shuriken_velocity_factor = SHURIKEN_VELOCITY_FACTOR_L2;
		this->shuriken_rotation_factor = SHURIKEN_ROTATION_FACTOR_L2;
	}
	else if (this->player_score >= LVL2_MAX_SCORE && this->player_score < LVL3_MAX_SCORE) { // lvl 3
		this->game_level = 3;
		this->seconds_between_red_ballons = SECONDS_BETWEEN_RED_BALLONS_L3;
		this->seconds_between_yellow_ballons = SECONDS_BETWEEN_YELLOW_BALLONS_L3;
		this->seconds_between_shurikens = SECONDS_BETWEEN_SHURIKENS_L3;
		this->ballons_velocity_factor = BALLONS_VELOCITY_FACTOR_L3;
		this->shuriken_velocity_factor = SHURIKEN_VELOCITY_FACTOR_L3;
		this->shuriken_rotation_factor = SHURIKEN_ROTATION_FACTOR_L3;
	}
	else if (this->player_score >= LVL3_MAX_SCORE && this->player_score < LVL4_MAX_SCORE) { // lvl 4
		this->game_level = 4;
		this->seconds_between_red_ballons = SECONDS_BETWEEN_RED_BALLONS_L4;
		this->seconds_between_yellow_ballons = SECONDS_BETWEEN_YELLOW_BALLONS_L4;
		this->seconds_between_shurikens = SECONDS_BETWEEN_SHURIKENS_L4;
		this->ballons_velocity_factor = BALLONS_VELOCITY_FACTOR_L4;
		this->shuriken_velocity_factor = SHURIKEN_VELOCITY_FACTOR_L4;
		this->shuriken_rotation_factor = SHURIKEN_ROTATION_FACTOR_L4;
	}
	else if (this->player_score >= LVL4_MAX_SCORE && this->player_score < LVL5_MAX_SCORE) { // lvl 5
		this->game_level = 5;
		this->seconds_between_red_ballons = SECONDS_BETWEEN_RED_BALLONS_L5;
		this->seconds_between_yellow_ballons = SECONDS_BETWEEN_YELLOW_BALLONS_L5;
		this->seconds_between_shurikens = SECONDS_BETWEEN_SHURIKENS_L5;
		this->ballons_velocity_factor = BALLONS_VELOCITY_FACTOR_L5;
		this->shuriken_velocity_factor = SHURIKEN_VELOCITY_FACTOR_L5;
		this->shuriken_rotation_factor = SHURIKEN_ROTATION_FACTOR_L5;
	}
	else if (this->player_score >= LVL5_MAX_SCORE) { // lvl 6
		this->game_level = 6;
		this->seconds_between_red_ballons = SECONDS_BETWEEN_RED_BALLONS_L6;
		this->seconds_between_yellow_ballons = SECONDS_BETWEEN_YELLOW_BALLONS_L6;
		this->seconds_between_shurikens = SECONDS_BETWEEN_SHURIKENS_L6;
		this->ballons_velocity_factor = BALLONS_VELOCITY_FACTOR_L6;
		this->shuriken_velocity_factor = SHURIKEN_VELOCITY_FACTOR_L6;
		this->shuriken_rotation_factor = SHURIKEN_ROTATION_FACTOR_L6;
	}
}

/* update the game timings */
void Tema1::update_timings(float deltaTimeSeconds) {
	seconds_since_arrow_fired += deltaTimeSeconds;
	seconds_since_red_ballon_thrown += deltaTimeSeconds;
	seconds_since_yellow_ballon_thrown += deltaTimeSeconds;
	seconds_since_shuriken_thrown += deltaTimeSeconds;
}