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
	void RestartBall(bool reset);

public:
	
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;
	
	p2List<PhysBody*> bumper;
	p2List<PhysBody*> lateral_bumper_right;
	p2List<PhysBody*> lateral_bumper_left;

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


	bool sensed;
	bool sensed_start_1;
	bool sensed_start_2;
	bool sensed_start_3;
	bool sensed_start_4;
	bool sensed_start_5;
	bool start;
	bool reboted;
	bool reboted2;
	float power_ball;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* pin_background;
	SDL_Texture* ball_tex;
	SDL_Texture* bounce_tex;
	SDL_Texture* bounce_hit_tex;
	SDL_Texture* lateral_bounce_left_tex;
	SDL_Texture* lateral_bounce_right_tex;
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

	

	uint start_fx;
	uint ball_throw_fx;
	uint bumper_fx;
	uint lateral_bumper_fx;
	uint loose_bip_fx;
	uint loose_dinosaur_fx;

	p2Point<int> ray;
	bool ray_on;

	int score = 0;
	
};
