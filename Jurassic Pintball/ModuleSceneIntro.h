#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void SetChain();
	void AddBodies();
	void CreateJoints();
	void RestartBall(bool reset = false);

public:
	
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;
	
	
	PhysBody* lateral_bumper_right;
	PhysBody* lateral_bumper_left;

	PhysBody* background_chain;
	PhysBody* background_chain2;
	PhysBody* background_chain3;
	PhysBody* background_chain4;
	PhysBody* background_chain5;
	PhysBody* background_chain6;
	PhysBody* background_chain7;
	PhysBody* background_chain8;
	PhysBody* background_chain9;

	PhysBody* left_flipper;
	PhysBody* right_flipper;
	PhysBody* left_flipper_ball;
	PhysBody* right_flipper_ball;
	PhysBody* ball;
	PhysBody* rect_ground;
	PhysBody* sensor_death;
	PhysBody* sensor_start_1;
	PhysBody* sensor_start_2;
	PhysBody* sensor_start_3;
	PhysBody* sensor_start_4;
	PhysBody* sensor_start_5;
	
	PhysBody* bumper1;
	PhysBody* bumper2;
	PhysBody* bumper3;
	PhysBody* bumper4;
	PhysBody* bumper5;
	PhysBody* bumper6;
	PhysBody* bumper7;
	PhysBody* bumper8;
	PhysBody* bumper9;
	PhysBody* bumper10;
	
	bool sensed;
	bool sensed_start_1;
	bool sensed_start_2;
	bool sensed_start_3;
	bool sensed_start_4;
	bool sensed_start_5;
	bool start;
		
	float power_ball;
	
	bool light_bumper1 = false;
	bool light_bumper2 = false;
	bool light_bumper3 = false;
	bool light_bumper4 = false;
	bool light_bumper5 = false;
	bool light_bumper6 = false;
	bool light_bumper7 = false;
	bool light_bumper8 = false;
	bool light_bumper9 = false;
	bool light_bumper10 = false;
	
	bool reboted1 = false;
	bool reboted2 = false;
	bool reboted3 = false;
	bool reboted4 = false;
	bool reboted5 = false;
	bool reboted6 = false;
	bool reboted7 = false;
	bool reboted8 = false;
	bool reboted9 = false;
	bool reboted10 = false;

	bool reboted2_1 = false;
	bool reboted2_2 = false;

	bool light_lateral_left = false;
	bool light_lateral_right = false;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* pin_background;
	SDL_Texture* background_lost;
	SDL_Texture* ball_tex;
	SDL_Texture* bounce_tex;
	SDL_Texture* bounce_hit_tex;
	SDL_Texture* flipper_left_tex;
	SDL_Texture* flipper_right_tex;
	SDL_Texture* tunnel_tex;
	SDL_Texture* title;
	SDL_Texture* lights_ball_throw;
	SDL_Texture* light1_ball_throw_on;
	SDL_Texture* light2_ball_throw_on;
	SDL_Texture* light3_ball_throw_on;
	SDL_Texture* light4_ball_throw_on;
	SDL_Texture* light5_ball_throw_on;
	SDL_Texture* lateral_bounce_left_tex;
	SDL_Texture* lateral_bounce_left_tex1;
	SDL_Texture* lateral_bounce_left_tex2;
	SDL_Texture* lateral_bounce_left_tex3;
	SDL_Texture* lateral_bounce_left_tex4;
	SDL_Texture* lateral_bounce_left_tex5;
	SDL_Texture* lateral_bounce_right_tex;
	SDL_Texture* lateral_bounce_right_tex1;
	SDL_Texture* lateral_bounce_right_tex2;
	SDL_Texture* lateral_bounce_right_tex3;
	SDL_Texture* lateral_bounce_right_tex4;
	SDL_Texture* lateral_bounce_right_tex5;


	uint start_fx;
	uint ball_throw_fx;
	uint bumper_fx;
	uint lateral_bumper_fx;
	uint loose_bip_fx;
	uint loose_dinosaur_fx;

	p2Point<int> ray;
	bool ray_on;

	
	int score = 0;
	int previous_score = 0;
	int highscore = 0;

	char score_char[10];
	char previous_char[10];
	char highscore_char[10];
	
	
	int lives;
	char lives_char[10];
	bool gameover;


};
