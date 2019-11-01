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
	p2List<PhysBody*> bumper;
	p2List<PhysBody*> flipper_left;
	p2List<PhysBody*> flipper_right;


	PhysBody* sensor;
	PhysBody* rect_ground;
	PhysBody* background_chain;
	PhysBody* background_chain2;
	PhysBody* background_chain3;
	PhysBody* background_chain4;
	PhysBody* background_chain5;
	PhysBody* background_chain6;
	PhysBody* ball_flipper_left;
	PhysBody* ball_flipper_right;

	PhysBody* Fflipper_left;
	PhysBody* Fflipper_right;



	/*PhysBody* flipper_left;
	PhysBody* flipper_right;*/

	PhysBody* ball;


	bool sensed;
	bool start;

	float power_ball = 17;


	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* pin_background;
	SDL_Texture* ball_tex;
	SDL_Texture* bounce_tex;
	SDL_Texture* flipper_left_tex;
	SDL_Texture* flipper_right_tex;
	SDL_Texture* tunnel_tex;
	
	uint start_fx;
	uint ball_throw_fx;

	p2Point<int> ray;
	bool ray_on;

	
};
