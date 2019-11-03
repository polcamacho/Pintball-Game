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
	sensed_start_1 = false;
	sensed_start_2 = false;
	sensed_start_3 = false;
	sensed_start_4 = false;
	sensed_start_5 = false;
	ball = NULL;
	ball_tex = nullptr;
	bounce_tex = nullptr;
	
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;
	
	//textures
	pin_background = App->textures->Load("pinball/background2.png");
	ball_tex = App->textures->Load("pinball/ball2.png");
	bounce_tex = App->textures->Load("pinball/Bounce.png");
	lateral_bounce_right_tex = App->textures->Load("pinball/lateral_Bounce_right.png");
	lateral_bounce_left_tex = App->textures->Load("pinball/lateral_Bounce_left.png");
	flipper_left_tex = App->textures->Load("pinball/flipper_left.png");
	flipper_right_tex = App->textures->Load("pinball/flipper_right.png");
	tunnel_tex = App->textures->Load("pinball/tunnel.png");
	title = App->textures->Load("pinball/title.png");
	lights_ball_throw = App->textures->Load("pinball/lights_ball_throw.png");
	light1_ball_throw_on = App->textures->Load("pinball/light_1_2_3.png");
	light2_ball_throw_on = App->textures->Load("pinball/light_1_2_3.png");
	light3_ball_throw_on = App->textures->Load("pinball/light_1_2_3.png");
	light4_ball_throw_on = App->textures->Load("pinball/light_4.png");
	light5_ball_throw_on = App->textures->Load("pinball/light_5.png");
	bounce_hit_tex = App->textures->Load("pinball/bounce_when_hit.png");

	//SFX
	ball_throw_fx = App->audio->LoadFx("pinball/Sound/ball_trow.wav");
	start_fx = App->audio->LoadFx("pinball/Sound/start.wav");
	bumper_fx = App->audio->LoadFx("pinball/Sound/bounce.wav");
	lateral_bumper_fx = App->audio->LoadFx("pinball/Sound/lateral_bounce.wav");
	loose_bip_fx=App->audio->LoadFx("pinball/Sound/loose_pip.wav");
	loose_dinosaur_fx = App->audio->LoadFx("pinball/Sound/dinosaur.wav");

	App->audio->PlayFx(start_fx);

	CreateJoints();
	AddBodies();
	SetChain();
	
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
	App->textures->Unload(tunnel_tex);
	App->textures->Unload(title);
	App->textures->Unload(lights_ball_throw);
	App->textures->Unload(light1_ball_throw_on);
	App->textures->Unload(light2_ball_throw_on);
	App->textures->Unload(light3_ball_throw_on);
	App->textures->Unload(light4_ball_throw_on);
	App->textures->Unload(light5_ball_throw_on);
	App->textures->Unload(bounce_hit_tex);

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
		
	//blits
	App->renderer->Blit(pin_background, 0, 0);
	
	if (ball != nullptr)
	{//ball
		int x, y;
		ball->GetPosition(x, y);
		App->renderer->Blit(ball_tex, x, y);
	}

	App->renderer->Blit(tunnel_tex, 90, 35);
	//blits
	App->renderer->Blit(lights_ball_throw, 200, 62);
	App->renderer->Blit(title, 9, 364);

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		power_ball = -21.5f;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
	{

		if (start == true)
		{
			ball->body->SetLinearVelocity(b2Vec2(0, power_ball));
			App->audio->PlayFx(ball_throw_fx);
			start = false;
		}

		power_ball = 0;

	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		App->scene_intro->left_flipper->body->ApplyAngularImpulse(-0.5f, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		App->scene_intro->right_flipper->body->ApplyAngularImpulse(0.5f, true);
	}

	if (sensed == true)
	{
		ball->body->SetTransform({ PIXEL_TO_METERS(242), PIXEL_TO_METERS(355 - 0.2f) }, 0.0f);
		App->audio->PlayFx(loose_bip_fx);
		App->audio->PlayFx(loose_dinosaur_fx);
		sensed = false;
	}

	if (sensed_start_1 == true)
	{
		App->renderer->Blit(light1_ball_throw_on, 235, 265);
		     
		sensed_start_1 = false;
	}

	if (sensed_start_2 == true)
	{
		App->renderer->Blit(light2_ball_throw_on, 235, 206);
		sensed_start_2 = false;
	}

	if (sensed_start_3 == true)
	{
		App->renderer->Blit(light3_ball_throw_on, 235, 149);
		sensed_start_3 = false;
	}

	if (sensed_start_4 == true)
	{
		App->renderer->Blit(light4_ball_throw_on, 225, 100);
		sensed_start_4 = false;
	}

	if (sensed_start_5 == true)
	{
		App->renderer->Blit(light5_ball_throw_on, 202, 63);
		sensed_start_5 = false;
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 5, true, 0.0f));

		circles.getLast()->data->listener = this;
	}


	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

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

	

	if (left_flipper != NULL)
	{
		int x, y;
		left_flipper->GetPosition(x, y);
		App->renderer->Blit(flipper_left_tex, x, y, NULL, 1.0f, left_flipper->GetRotation());
	}

	if (right_flipper != NULL)
	{
		int x, y;
		right_flipper->GetPosition(x, y);
		App->renderer->Blit(flipper_right_tex, x, y, NULL, 1.0f, right_flipper->GetRotation());
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

	p2List_item<PhysBody*>* bounce = bumper.getFirst();

	while (bounce != nullptr)
	{
		int x, y;
		bounce->data->GetPosition(x, y);
		App->renderer->Blit(bounce_tex, x, y);
		bounce = bounce->next;
	}

	p2List_item<PhysBody*>* lateral_bounce_right = lateral_bumper_right.getFirst();

	while (lateral_bounce_right != nullptr)
	{
		int x, y;
		lateral_bounce_right->data->GetPosition(x, y);
		App->renderer->Blit(lateral_bounce_right_tex, x, y);
		lateral_bounce_right = lateral_bounce_right->next;
	}

	p2List_item<PhysBody*>* lateral_bounce_left = lateral_bumper_left.getFirst();

	while (lateral_bounce_left != nullptr)
	{
		int x, y;
		lateral_bounce_left->data->GetPosition(x, y);
		App->renderer->Blit(lateral_bounce_left_tex, x, y);
		lateral_bounce_left = lateral_bounce_left->next;
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	if (bodyA->body->GetFixtureList()->IsSensor())
	{
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			start = true;
		}
		if (bodyA == sensor_death)
		{
			sensed = true;
		}
		if (bodyA == sensor_start_1)
		{
			sensed_start_1 = true;
		}
		if (bodyA == sensor_start_2)
		{
			sensed_start_2 = true;
		}
		if (bodyA == sensor_start_3)
		{
			sensed_start_3 = true;
		}
		if (bodyA == sensor_start_4)
		{
			sensed_start_4 = true;
		}
		if (bodyA == sensor_start_5)
		{
			sensed_start_5 = true;
		}

	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		App->scene_intro->left_flipper->body->ApplyAngularImpulse(-0.5f, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		App->scene_intro->right_flipper->body->ApplyAngularImpulse(0.5f, true);
	}

	if (bodyA->body->GetFixtureList()->IsSensor())
	{
		ball->body->ApplyForce({ 0,-power_ball }, ball->body->GetLocalCenter(), true);
	}


	p2List_item<PhysBody*>* item = bumper.getFirst();

	while (item != nullptr)
	{
		if (bodyB == item->data)
		{
			reboted = true;
		}
		item = item->next;
	}

	p2List_item<PhysBody*>* item2 = lateral_bumper_right.getFirst();

	while (item2 != nullptr)
	{
		if (bodyB == item2->data)
		{
			reboted2 = true;
		}
		item2 = item2->next;
	}

	p2List_item<PhysBody*>* item3 = lateral_bumper_left.getFirst();

	while (item3 != nullptr)
	{
		if (bodyB == item3->data)
		{
			reboted2 = true;
		}
		item3 = item3->next;
	}

	if (reboted)
	{
		//App->player->score += 100;

		App->audio->PlayFx(bumper_fx);

		reboted = false;
	}

	if (reboted2)
	{
		//App->player->score += 100;
		App->audio->PlayFx(lateral_bumper_fx);
		reboted2 = false;
	}

}

void ModuleSceneIntro::SetChain(){
	
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

	background_chain = App->physics->CreateChain(0, 0, background, false, 74, 0.0f);

	int laterals_balldrop1[8] = {
	10, 331,
	10, 362,
	68, 363,
	69, 353
	};

	background_chain2 = App->physics->CreateChain(0, 0, laterals_balldrop1, false, 8, 0.0f);

	int laterals_balldrop2[8] = {
	234, 330,
	173, 351,
	173, 360,
	234, 362
	};

	background_chain3 = App->physics->CreateChain(0, 0, laterals_balldrop2, false, 8, 0.0f);

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


	background_chain4 = App->physics->CreateChain(0, 0, ball_tube, false, 64, 0.0f);
		
	// Pivot 0, 0
	int lateral_down_left[10] = {
		25, 313,
		79, 332,
		79, 333,
		25, 315,
		25, 314
	};
	
	background_chain5 = App->physics->CreateChain(0, 0, lateral_down_left, false, 10, 0.0f);


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

	background_chain6 = App->physics->CreateChain(0, 0, lateral_down_right, false, 16, 0.0f);
	
	int laterals_left[8] = {
	26, 297,
	26, 318,
	31, 318,
	31, 304
	};

	background_chain7 = App->physics->CreateChain(0, 0, laterals_left, false, 8, 0.0f);

	int laterals_right[8] = {
	221, 297,
	221, 316,
	216, 316,
	216, 304
	};
	
	background_chain8 = App->physics->CreateChain(0, 0, laterals_right, false, 8, 0.0f);

	int start_wall[12] = {
	59, 57,
	59, 66,
	61, 73,
	62, 72,
	60, 66,
	60, 57
	};

	background_chain9 = App->physics->CreateChain(0, 0, start_wall, false, 12, 0.0f);

}

void ModuleSceneIntro::AddBodies() {
	
	//BAll
	ball = App->physics->CreateCircle(241, 340, 5, true, 0.1f);
	ball->listener = this;
	//live++;

	//Sensor Start
	rect_ground = App->physics->CreateRectangleSensor(241, 363, 10, 5);
	rect_ground->listener = this;

	//Death Sensor
	sensor_death = App->physics->CreateRectangleSensor(123, SCREEN_HEIGHT, SCREEN_WIDTH / 4, 24);
	sensor_death->listener = this;

	//Sensors tube start 
	sensor_start_1 = App->physics->CreateRectangleSensor(240, 273, 12, 5);
	sensor_start_1->listener = this;

	sensor_start_2 = App->physics->CreateRectangleSensor(240, 217, 12, 5);
	sensor_start_2->listener = this;

	sensor_start_3 = App->physics->CreateRectangleSensor(240, 158, 12, 5);
	sensor_start_3->listener = this;

	sensor_start_4 = App->physics->CreateRectangleSensor(230, 113, 12, 5);
	sensor_start_4->listener = this;

	sensor_start_5 = App->physics->CreateRectangleSensor(208, 78, 12, 5);
	sensor_start_5->listener = this;

	//bumpers
	p2List_item<PhysBody*>* item;

	bumper.add(App->physics->CreateCircle(80, 125, 8, false, 1.2f));
	bumper.add(App->physics->CreateCircle(40, 120, 8, false, 1.2f));
	bumper.add(App->physics->CreateCircle(200, 130, 8, false, 1.2f));
	bumper.add(App->physics->CreateCircle(60, 195, 8, false, 1.2f));
	bumper.add(App->physics->CreateCircle(110, 180, 8, false, 1.2f));
	bumper.add(App->physics->CreateCircle(65, 280, 8, false, 1.2f));
	bumper.add(App->physics->CreateCircle(195, 175, 8, false, 1.2f));
	bumper.add(App->physics->CreateCircle(100, 220, 8, false, 1.2f));
	bumper.add(App->physics->CreateCircle(150, 200, 8, false, 1.2f));
	bumper.add(App->physics->CreateCircle(135, 100, 8, false, 1.2f));

	item = bumper.getFirst();

	while (item != nullptr)
	{
		item->data->listener = this;
		item = item->next;
	}

	//Lateral bumpers

	p2List_item<PhysBody*>* item2;

	int lateral_Bounce_right[10] = {
	26, 1,
	26, 61,
	1, 48,
	1, 19,
	5, 11
	};
	
	lateral_bumper_right.add(App->physics->CreateChain(209, 160, lateral_Bounce_right, false, 10, 1.1f));
	
	item2 = lateral_bumper_right.getFirst();

	while (item2 != nullptr)
	{
		item2->data->listener = this;
		item2 = item2->next;
	}
	
	p2List_item<PhysBody*>* item3;

	int lateral_Bounce_left[10] = {
	0, 1,
	0, 60,
	24, 49,
	24, 18,
	20, 11
	};
	
	lateral_bumper_left.add(App->physics->CreateChain(12, 197, lateral_Bounce_left, false, 10, 1.1f));
		
	item3 = lateral_bumper_left.getFirst();

	while (item3 != nullptr)
	{
		item3->data->listener = this;
		item3 = item3->next;
	}


}

void ModuleSceneIntro::CreateJoints() {

	b2RevoluteJoint* left_flipper_ball_joint;
	b2RevoluteJoint* right_flipper_ball_joint;
	b2RevoluteJointDef left_flipper_definition;
	b2RevoluteJointDef right_flipper_definition;
	
	left_flipper = App->physics->CreateRectangle(100, 340, 35, 8, true, 0.0f);
	right_flipper = App->physics->CreateRectangle(140, 340, 35, 8, true, 0.0f);
	
	left_flipper_ball = App->physics->CreateCircle(87, 342, 2, false, 0.25f);
	right_flipper_ball = App->physics->CreateCircle(153, 342, 2, false, 0.25f);
	
	left_flipper_definition.Initialize(left_flipper->body, left_flipper_ball->body, left_flipper_ball->body->GetWorldCenter());
	right_flipper_definition.Initialize(right_flipper_ball->body, right_flipper->body, right_flipper_ball->body->GetWorldCenter());
	
	left_flipper_definition.enableLimit = true;
	right_flipper_definition.enableLimit = true;
	
	left_flipper_definition.lowerAngle = -40 * DEGTORAD;
	left_flipper_definition.upperAngle = 40 * DEGTORAD;
	right_flipper_definition.lowerAngle = -40 * DEGTORAD;
	right_flipper_definition.upperAngle = 40 * DEGTORAD;

	left_flipper_ball_joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&left_flipper_definition);
	left_flipper_ball_joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&right_flipper_definition);
	

	
}

/*void ModulePlayer::RestartBall(bool reset)
{
	if (++live >= 5 && reset == false)
		gameover = true;
	else if (reset == true)
	{
		ball->body->SetLinearVelocity(b2Vec2(0, 0));
		ball->body->SetAngularVelocity(0);
		ball->body->SetTransform(b2Vec2(PIXEL_TO_METERS(320), PIXEL_TO_METERS(485)), 0);
		score = 0;
		live = 1;
	}
	else
	{
		ball->body->SetLinearVelocity(b2Vec2(0, 0));
		ball->body->SetAngularVelocity(0);
		ball->body->SetTransform(b2Vec2(PIXEL_TO_METERS(320), PIXEL_TO_METERS(485)), 0);
	}
}*/