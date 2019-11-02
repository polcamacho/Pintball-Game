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


public:
	
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;
	
	

	PhysBody* sensor;
	PhysBody* rect_ground;
	p2List<PhysBody*> bumper;
	p2List<PhysBody*> lateral_bumper_right;
	p2List<PhysBody*> lateral_bumper_left;

	PhysBody* background_chain;
	PhysBody* background_chain2;
	PhysBody* background_chain3;
	PhysBody* background_chain4;
	PhysBody* background_chain5;
	PhysBody* background_chain6;

	
	
	PhysBody* left_flipper;
	PhysBody* right_flipper;
	PhysBody* left_flipper_ball;
	PhysBody* right_flipper_ball;
	
	PhysBody* ball;


	bool sensed;
	bool start;
	bool reboted;

	float power_ball;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* pin_background;
	SDL_Texture* ball_tex;
	SDL_Texture* bounce_tex;
	SDL_Texture* lateral_bounce_left_tex;
	SDL_Texture* lateral_bounce_right_tex;
	SDL_Texture* flipper_left_tex;
	SDL_Texture* flipper_right_tex;
	SDL_Texture* tunnel_tex;
	SDL_Texture* title;
	
	uint start_fx;
	uint ball_throw_fx;

	p2Point<int> ray;
	bool ray_on;

	
};
