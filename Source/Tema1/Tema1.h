#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>
#include <time.h>
#include <stdlib.h>
#include <Tema1/Objects/Bow.h>
#include <Tema1/Objects/Arrow.h>
#include <Tema1/Objects/Ballon.h>
#include <Tema1/Objects/Shuriken.h>
#include <Tema1/Objects/PoawerBar.h>
#include <Tema1/Macros.h>
#include <stack>



class Tema1 : public SimpleScene
{
	public:
		Tema1();
		~Tema1();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

		void CreateMeshes();

		/* Class instantiations methods */
		void CreateArrowObject();
		void CreateBowObject();
		Shuriken* CreateShurikenObject();
		Ballon* CreateRedBallonObject();
		Ballon* CreateYellowBallonObject();
		void CreatePowerBarObject();

		/* rendering methods */
		void render_red_ballons(float time);
		void render_yellow_ballons(float deltaTimeSeconds);
		void render_shurikens(float deltaTimeSeconds);
		void render_arrows(float deltaTimeSeconds);
		void render_score();
		void render_hearts();
		void render_scorebar();
		void render_powerbar();
		void render_main_arrow();
		void render_bow();

		/* collision detection */
		void check_collisions();

		/* gameplay methods*/
		void print_score();
		void update_game();
		void update_timings(float deltaTimeSeconds);

	protected:
		glm::vec3 corner = glm::vec3(0, 0, 0);
		glm::mat3 modelMatrix;

		/* Game Objects */
		Bow* bow;
		Arrow* main_arrow;
		Ballon* red_ballon;
		Ballon* yellow_ballon;
		Shuriken* shuriken;
		PowerBar* power_bar;

		/* list of game objects */
		std::vector<Arrow*> shooted_arrows;
		std::vector<Ballon*> red_ballons;
		std::vector<Ballon*> yellow_ballons;
		std::vector<Shuriken*> shurikens;
		glm::ivec2 resolution;

		/* bow angle */
		float bow_radians = 0;

		/* gameplay timings */
		float time_mouse_pressed = SECONDS_BETWEEN_ARROWS;
		float seconds_since_arrow_fired = SECONDS_BETWEEN_ARROWS;
		float seconds_since_red_ballon_thrown = 0;
		float seconds_since_yellow_ballon_thrown = 0;
		float seconds_since_shuriken_thrown = 0;
		float seconds_between_red_ballons = SECONDS_BETWEEN_RED_BALLONS_L1;
		float seconds_between_yellow_ballons = SECONDS_BETWEEN_YELLOW_BALLONS_L1;
		float seconds_between_shurikens = SECONDS_BETWEEN_SHURIKENS_L1;

		/* velocity for the objects */
		float ballons_velocity_factor = BALLONS_VELOCITY_FACTOR_L1;
		float shuriken_velocity_factor = SHURIKEN_VELOCITY_FACTOR_L1;
		float shuriken_rotation_factor = SHURIKEN_ROTATION_FACTOR_L1;

		/* Gameplay fields */
		int player_score = 0;
		int player_lives = 3;
		int game_level = 1;

		/* Meshes */
		Mesh* power_bar_mesh;
		Mesh* bow_mesh;
		Mesh* arrow_mesh;
		Mesh* shuriken_mesh;
		Mesh* red_ballon_mesh;
		Mesh* yellow_ballon_mesh;
		Mesh* heart_mesh;
		Mesh* digit0, *digit1, *digit2, *digit3, *digit4, *digit5,
			*digit6, *digit7, *digit8, *digit9;
		Mesh* letter_s;
		Mesh* letter_c;
		Mesh* letter_o;
		Mesh* letter_r;
		Mesh* letter_e;
		Mesh* equal1;
		Mesh* equal2;
		Mesh* square;
		
};
