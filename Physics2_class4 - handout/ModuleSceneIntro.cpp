#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
	ray_on = false;
	sensed = false;
	ball = NULL;
	ball_tex = nullptr;
	bounce_tex = nullptr;
	flipper_left_tex = nullptr;
	flipper_right_tex = nullptr;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;
	
	pin_background = App->textures->Load("pinball/background.png");
	ball_tex = App->textures->Load("pinball/ball2.png");
	bounce_tex = App->textures->Load("pinball/Bounce.png");
	flipper_left_tex = App->textures->Load("pinball/flipper_left.png");
	flipper_right_tex = App->textures->Load("pinball/flipper_right.png");
	tunnel_tex = App->textures->Load("pinball/tunnel.png");
	
	ball = App->physics->CreateCircle(241, 340, 5, true);

	rect_ground = App->physics->CreateRectangleSensor(241, 363, 10, 5);
	rect_ground->listener = this;

	sensor = App->physics->CreateRectangleSensor(123, SCREEN_HEIGHT, SCREEN_WIDTH/4, 24);
	sensor->listener = this;


	SetChain();
	AddBodies();


	power_ball = 0;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	App->textures->Unload(pin_background);
	App->textures->Unload(ball_tex);
	App->textures->Unload(bounce_tex);
	App->textures->Unload(flipper_left_tex);
	App->textures->Unload(flipper_right_tex);
	App->textures->Unload(flipper_left_tex);
	App->textures->Unload(tunnel_tex);


	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	App->renderer->Blit(pin_background, 0, 0);


	
	//ball
	int x, y;
	ball->GetPosition(x, y);

	App->renderer->Blit(ball_tex, x, y);

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		power_ball = 18;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
	{


		if (start == true)
		{
			
			ball->body->ApplyForce({ 0,-power_ball }, ball->body->GetLocalCenter(), true);
			start = false;
		}

		power_ball = 0;

	}

	if (sensed == true)
	{
		ball->body->SetTransform({ PIXEL_TO_METERS(242), PIXEL_TO_METERS(355 - 0.2f) }, 0.0f);
		sensed = false;
	}

	/*{
		int x, y;

		flipper_left->GetPosition(x,y);
		App->renderer->Blit(flipper_left_tex, x, y, NULL, 1.0f, RADTODEG * flipper_left->GetRotation());
	}*/

	/*angle = flipper_right->body->GetAngle();
	App->renderer->Blit(App->player->imgflipperright, 171, 530, NULL, 1.0f, RADTODEG *angle + 18, 45, 10);*/

	
	App->renderer->Blit(tunnel_tex, 90, 35);

	

	/*if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}*/

	/*if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 5, true));

		circles.getLast()->data->listener = this;
	}*/

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = bumper.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);

		App->renderer->Blit(bounce_tex, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}
	
	c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		
		App->renderer->Blit(ball_tex, x, y, NULL, 1.0f);

		c = c->next;
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	c = ricks.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = flipper_left.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(flipper_left_tex, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = flipper_right.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(flipper_right_tex, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}


	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

	if (bodyA->body->GetFixtureList()->IsSensor())
	{
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			start = true;
		}
		if (bodyA == sensor)
		{
			sensed = true;
		}

	}
}

void ModuleSceneIntro::SetChain(){
	// Pivot 0, 0
	int background[74] = {
	9, 362,
	11, 197,
	8, 180,
	6, 163,
	5, 148,
	8, 132,
	13, 116,
	20, 101,
	29, 87,
	41, 74,
	56, 61,
	68, 53,
	80, 47,
	94, 42,
	109, 39,
	124, 38,
	140, 39,
	151, 40,
	162, 43,
	175, 48,
	184, 52,
	192, 58,
	202, 65,
	209, 71,
	218, 81,
	225, 90,
	231, 100,
	238, 114,
	243, 128,
	246, 141,
	247, 156,
	247, 224,
	247, 362,
	174, 362,
	172, 389,
	76, 389,
	69, 363
	};

	background_chain = App->physics->CreateChain(0, 0, background, 74);

	int laterals_balldrop1[8] = {
	10, 331,
	10, 362,
	68, 363,
	69, 353
	};

	background_chain2 = App->physics->CreateChain(0, 0, laterals_balldrop1, 8);

	int laterals_balldrop2[8] = {
	234, 330,
	173, 351,
	173, 360,
	234, 362
	};

	background_chain3 = App->physics->CreateChain(0, 0, laterals_balldrop2, 8);

	int ball_tube[64] = {
	86, 59,
	98, 55,
	111, 52,
	128, 50,
	152, 53,
	168, 59,
	180, 64,
	189, 70,
	196, 75,
	205, 84,
	216, 99,
	228, 119,
	233, 136,
	234, 153,
	234, 173,
	235, 362,
	232, 362,
	232, 173,
	232, 153,
	231, 136,
	226, 119,
	214, 99,
	203, 84,
	194, 75,
	187, 70,
	178, 64,
	166, 59,
	149, 54,
	133, 52,
	121, 52,
	106, 54,
	89, 59
};

	background_chain4 = App->physics->CreateChain(0, 0, ball_tube, 64);

	int lateral_down_left[20] = {
	31, 314,
	43, 319,
	58, 325,
	71, 330,
	84, 335,
	84, 337,
	71, 332,
	58, 327,
	43, 321,
	31, 316
	};
	
	background_chain5 = App->physics->CreateChain(0, 0, lateral_down_left, 20);

	int lateral_down_right[16] = {
	217, 313,
	195, 321,
	175, 328,
	162, 334,
	162, 335,
	175, 329,
	195, 322,
	217, 314
	};

	background_chain6 = App->physics->CreateChain(0, 0, lateral_down_right, 16);


}

void ModuleSceneIntro::AddBodies() {

	//bumpers
	bumper.add(App->physics->CreateCircle(80, 125, 8, false));
	bumper.add(App->physics->CreateCircle(40, 120, 8, false));
	bumper.add(App->physics->CreateCircle(200, 130, 8, false));
	bumper.add(App->physics->CreateCircle(60, 195, 8, false));
	bumper.add(App->physics->CreateCircle(110, 180, 8, false));
	bumper.add(App->physics->CreateCircle(65, 280, 8, false));
	bumper.add(App->physics->CreateCircle(195, 175, 8, false));
	bumper.add(App->physics->CreateCircle(100, 220, 8, false));
	bumper.add(App->physics->CreateCircle(150, 200, 8, false));
	bumper.add(App->physics->CreateCircle(135, 100, 8, false));

	//flippers
	flipper_left.add(App->physics->CreateRectangle(97, 340, 40, 10));
}
