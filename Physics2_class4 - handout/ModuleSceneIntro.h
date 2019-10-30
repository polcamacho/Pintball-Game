#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "p2DynArray.h"

#include "Globals.h"

#define BOUNCE_TIME 150

class PhysBody;

struct Bounce {

	Bounce() : body(NULL), texture(NULL), hitT(0), fx(0) {}

	PhysBody* body;
	SDL_Texture* texture;
	uint hitT;
	uint fx;

};
struct Light
{
	Light() : body(NULL), texture(NULL), on(false), fx(0)
	{}

	Light(ModuleSceneIntro* physics, int x, int y, light_intensity type);

	light_intensity type;
	PhysBody* body;
	SDL_Texture* texture;
	bool on;
	uint fx;
	int x, y;

};
enum light_intensity {

	LOW,
	MEDIUM,
	HIGH,

};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	bool CollisionMap();

public:
	

	SDL_Texture* graphics;
	PhysBody* background;

	Bounce bounce[10];
	Bounce left_bounce, right_bounce;

	SDL_Texture* TLight_LOW;
	SDL_Texture* TLight_MEDIUM;
	SDL_Texture* TLight_HIGH;

	uint LIGHT_FX;

	p2DynArray<Light> lights;

	PhysBody* lose;
	uint lose_fx;

	Mix_Chunk music;

};
