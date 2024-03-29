#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"
#include "ModuleFonts.h"


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
	lives = 0;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;
	
	lives = 5;

	//TEXTURES
	pin_background = App->textures->Load("pinball/background2.png");
	background_lost = App->textures->Load("pinball/background_lost.png");
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
	bounce_hit_tex = App->textures->Load("pinball/Bounce_when_hit.png");
	lateral_bounce_right_tex_light = App->textures->Load("pinball/lateral_Bounce_right_light.png");
	lateral_bounce_left_tex_light = App->textures->Load("pinball/lateral_Bounce_left_light.png");
	score_tex = App->textures->Load("pinball/score_font.png");
	high_score_tex = App->textures->Load("pinball/high_score_font.png");
	prev_score_tex = App->textures->Load("pinball/previous_score_font.png");
	
	//FONTS
	score=App->fonts->Load("pinball/numbers_font.png", "0123456789", 1);

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
	App->textures->Unload(lateral_bounce_left_tex);
	App->textures->Unload(lateral_bounce_left_tex_light);
	App->textures->Unload(lateral_bounce_right_tex);
	App->textures->Unload(lateral_bounce_right_tex_light);
	App->textures->Unload(score_tex);
	App->textures->Unload(high_score_tex);

	App->fonts->UnLoad(0);

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
		
	//Blits
	App->renderer->Blit(pin_background, 0, 0);
	
	//BALL
	if (ball != nullptr)
	{
		int x, y;
		ball->GetPosition(x, y);
		App->renderer->Blit(ball_tex, x, y);
	}

	App->renderer->Blit(tunnel_tex, 90, 35);
	App->renderer->Blit(lights_ball_throw, 200, 62);
	App->renderer->Blit(title, 9, 364);
	App->renderer->Blit(score_tex, 350, 175);
	App->renderer->Blit(high_score_tex, 283, 47);
	App->renderer->Blit(prev_score_tex, 274, 98);
	
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{

		power_ball = -18.6f;
		
		
	}


	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{

		power_ball = -18.6f;
		

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

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
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
		App->scene_intro->left_flipper->body->ApplyAngularImpulse(-0.15f, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		App->scene_intro->right_flipper->body->ApplyAngularImpulse(0.15f, true);
	}


	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		
		gameover = false;
		RestartBall(true);
		
	}

	

	//SENSORS
	{
		if (sensed == true)
		{
			ball->body->SetTransform({ PIXEL_TO_METERS(242), PIXEL_TO_METERS(355 - 0.2f) }, 0.0f);
			App->audio->PlayFx(loose_bip_fx);
			App->audio->PlayFx(loose_dinosaur_fx);
			previous_score = score;
			sensed = false;
			
			lives -= 1;

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

	//SCORE AND LIVES

	sprintf_s(lives_char, 10, "%d", lives);
	App->fonts->BlitText(360, 213, 0, lives_char);

	sprintf_s(score_char, 10, "%d", score);
	App->fonts->BlitText(257, 174, 0, score_char);

	sprintf_s(highscore_char, 10, "%d", highscore);
	App->fonts->BlitText(274, 67, 0, highscore_char);

	sprintf_s(previous_char, 10, "%d", previous_score);
	App->fonts->BlitText(274, 118, 0, previous_char);

	//BUMPERS
	{

		if (light_bumper1 == true)
		{

			App->renderer->Blit(bounce_hit_tex, 70, 115);
			light_bumper1 = false;
		}
				

		else {
			
			App->renderer->Blit(bounce_tex, 70, 115);	

		}

		if (light_bumper2 == true)
		{
			App->renderer->Blit(bounce_hit_tex, 32, 140);
			light_bumper2 = false;
		}
		else {
			App->renderer->Blit(bounce_tex, 32, 140);
			
		}

		if (light_bumper3 == true)
		{
			App->renderer->Blit(bounce_hit_tex, 190, 120);
			light_bumper3 = false;
		}
		else {
			
			App->renderer->Blit(bounce_tex, 190, 120);
			
		}

		if (light_bumper4 == true)
		{
			App->renderer->Blit(bounce_hit_tex, 190, 245);
			light_bumper4 = false;
		}
		else {
			App->renderer->Blit(bounce_tex, 190, 245);
		}

		if (light_bumper5 == true)
		{
			App->renderer->Blit(bounce_hit_tex, 100, 170);
			light_bumper5 = false;
		}
		else {
			App->renderer->Blit(bounce_tex, 100, 170);
		}

		if (light_bumper6 == true)
		{
			App->renderer->Blit(bounce_hit_tex, 55, 270);
			light_bumper6 = false;
		}
		else {
			
			App->renderer->Blit(bounce_tex, 55, 270);
			
		}
	
		if (light_bumper7 == true)
		{
			App->renderer->Blit(bounce_hit_tex, 185, 165);
			light_bumper7 = false;
		}
		else {
			
			App->renderer->Blit(bounce_tex, 185, 165);
			
		}

		if (light_bumper8 == true)
		{
			App->renderer->Blit(bounce_hit_tex, 90, 210);
			light_bumper8 = false;
		}
		else {
			
			App->renderer->Blit(bounce_tex, 90, 210);
			
		}

		if (light_bumper9 == true)
		{
			App->renderer->Blit(bounce_hit_tex, 140, 190);
			light_bumper9 = false;
		}
		else {
			
			App->renderer->Blit(bounce_tex, 140, 190);
			
		}

		if (light_bumper10 == true)
		{
			App->renderer->Blit(bounce_hit_tex, 125, 90);
			light_bumper10 = false;
		}
		else {
			
			App->renderer->Blit(bounce_tex, 125, 90);
		}

		
	}

	//LATERAL BUMPERS
	{
	
	if(light_lateral_right == true)
	{
		
		App->renderer->Blit(lateral_bounce_right_tex_light, 209, 160);
		
		light_lateral_right = false;

	}
	else {
		App->renderer->Blit(lateral_bounce_right_tex, 209, 160);
	}

	if (light_lateral_left == true)
	{

		App->renderer->Blit(lateral_bounce_left_tex_light, 12, 197);
		
		light_lateral_left = false;
		
	}
	else {
		App->renderer->Blit(lateral_bounce_left_tex, 12, 197);
	}
		
	}

	if (lives < 1 ) {
		gameover = true;
	}

	if (gameover == true)
	{

		App->renderer->Blit(background_lost, 0, 0);
		ball->body->SetTransform(b2Vec2(PIXEL_TO_METERS(-50), PIXEL_TO_METERS(0)), 0);
	}

	LOG("%i", lives);

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	
	if (bodyA->body->GetFixtureList()->IsSensor())
	{
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
		{
			start = true;
		}

		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
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

	

	if (bodyA->body->GetFixtureList()->IsSensor())
	{
		ball->body->ApplyForce({ 0,-power_ball }, ball->body->GetLocalCenter(), true);
	}

	//BUMPERS
	{
	if (bodyA == bumper1)
	{
			
		reboted1 = true;
		light_bumper1 = true;
		
	}

	if (bodyA == bumper2)
	{
		
		reboted2 = true;
		light_bumper2 = true;
		

	}

	if (bodyA == bumper3)
	{
		
		light_bumper3 = true;
		reboted3 = true;
		

	}

	if (bodyA == bumper4)
	{
		light_bumper4 = true;
		reboted4 = true;
		

	}

	if (bodyA == bumper5)
	{
		light_bumper5 = true;
		reboted5 = true;
		

	}

	if (bodyA == bumper6)
	{
		light_bumper6 = true;
		reboted6 = true;
		

	}

	if (bodyA == bumper7)
	{
		light_bumper7 = true;
		reboted7 = true;
		

	}

	if (bodyA == bumper8)
	{
		light_bumper8 = true;
		reboted8 = true;
		

	}

	if (bodyA == bumper9)
	{
		light_bumper9 = true;
		reboted9 = true;
		

	}

	if (bodyA == bumper10)
	{
		light_bumper10 = true;
		reboted10 = true;
		

	}
	}
	//LATERALS BUMPERS
	{

		if (bodyA == lateral_bumper_right) {
			light_lateral_right = true;
			reboted2_1 = true;
		}

		if (bodyA == lateral_bumper_left) {
			light_lateral_left = true;
			reboted2_2 = true;
		}
	}

	if (reboted1)
	{
		
		App->audio->PlayFx(bumper_fx);
		reboted1 = false;
		if(score<5000){
			score += 100;
		}
		else if (score < 10000 && score> 5000) {
			score += 150;
		}
		else {
			score += 200;
		}
	}
	if (reboted2)
	{
		
		App->audio->PlayFx(bumper_fx);
		reboted2 = false;
		if (score < 5000) {
			score += 100;
		}
		else if (score < 10000 && score> 5000) {
			score += 150;
		}
		else {
			score += 200;
		}
	}
	if (reboted3)
	{
		
		App->audio->PlayFx(bumper_fx);
		reboted3 = false;
		if (score < 5000) {
			score += 100;
		}
		else if (score < 10000 && score> 5000) {
			score += 150;
		}
		else {
			score += 200;
		}
	}
	if (reboted4)
	{
		
		App->audio->PlayFx(bumper_fx);
		reboted4 = false;
		if (score < 5000) {
			score += 100;
		}
		else if (score < 10000 && score> 5000) {
			score += 150;
		}
		else {
			score += 200;
		}
	}
	if (reboted5)
	{
		
		App->audio->PlayFx(bumper_fx);
		reboted5 = false;
		if (score < 5000) {
			score += 100;
		}
		else if (score < 10000 && score> 5000) {
			score += 150;
		}
		else {
			score += 200;
		}
	}
	if (reboted6)
	{
		
		App->audio->PlayFx(bumper_fx);
		reboted6 = false;
		if (score < 5000) {
			score += 100;
		}
		else if (score < 10000 && score> 5000) {
			score += 150;
		}
		else {
			score += 200;
		}

	}
	if (reboted7)
	{
		
		App->audio->PlayFx(bumper_fx);
		reboted7 = false;
		if (score < 5000) {
			score += 100;
		}
		else if (score < 10000 && score> 5000) {
			score += 150;
		}
		else {
			score += 200;
		}
	}
	if (reboted8)
	{
		
		App->audio->PlayFx(bumper_fx);
		reboted8 = false;
		if(score<5000){
			score += 100;
		}
		else if (score < 10000 && score> 5000) {
			score += 150;
		}
		else {
			score += 200;
		}
	}
	if (reboted9)
	{
		
		App->audio->PlayFx(bumper_fx);
		reboted9 = false;
		if (score < 5000) {
			score += 100;
		}
		else if (score < 10000 && score> 5000) {
			score += 150;
		}
		else {
			score += 200;
		}
	}
	if (reboted10)
	{
		
		App->audio->PlayFx(bumper_fx);
		reboted10 = false;
		if (score < 5000) {
			score += 100;
		}
		else if (score < 10000 && score> 5000) {
			score += 150;
		}
		else {
			score += 200;
		}
	}

	if (reboted2_1)
	{
		App->audio->PlayFx(lateral_bumper_fx);
		reboted2_1 = false;
		if (score < 5000) {
			score += 200;
		}
		else if (score < 10000 && score> 5000) {
			score += 300;
		}
		else {
			score += 400;
		}
	}

	if (reboted2_2)
	{
		
		App->audio->PlayFx(lateral_bumper_fx);
		reboted2_2 = false;
		if (score < 5000) {
			score += 200;
		}
		else if (score < 10000 && score> 5000) {
			score += 300;
		}
		else {
			score += 400;
		}
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
	
	int laterals_left[10] = {
		26, 316,
		26, 284,
		32, 293,
		32, 318,
		28, 317
	};

	background_chain7 = App->physics->CreateChain(0, 0, laterals_left, false, 10, 0.0f);

	int laterals_right[10] = {
		216, 315,
		216, 288,
		221, 284,
		221, 313,
		217, 315
	};
	
	background_chain8 = App->physics->CreateChain(0, 0, laterals_right, false, 10, 0.0f);

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
	ball = App->physics->CreateCircle(241, 340, 5, true, 0.1f, 1.5f);
	ball->listener = this;
	//live++;

	//SENSORS
	{//Sensor Start
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
	}

	//Bumpers
	{
	bumper1 = App->physics->CreateCircle(80, 125, 8, false, 1.0f, 1.0f);
	bumper1->listener = this;

	bumper2 = App->physics->CreateCircle(42, 150, 8, false, 1.0f, 1.0f);
	bumper2->listener = this;

	bumper3 = App->physics->CreateCircle(200, 130, 8, false, 1.0f, 1.0f);
	bumper3->listener = this;

	bumper4 = App->physics->CreateCircle(200, 255, 8, false, 1.0f, 1.0f);
	bumper4->listener = this;

	bumper5 = App->physics->CreateCircle(110, 180, 8, false, 1.0f, 1.0f);
	bumper5->listener = this;

	bumper6 = App->physics->CreateCircle(65, 280, 8, false, 1.0f, 1.0f);
	bumper6->listener = this;

	bumper7 = App->physics->CreateCircle(195, 175, 8, false, 1.0f, 1.0f);
	bumper7->listener = this;

	bumper8 = App->physics->CreateCircle(100, 220, 8, false, 1.0f, 1.0f);
	bumper8->listener = this;

	bumper9 = App->physics->CreateCircle(150, 200, 8, false, 1.0f, 1.0f);
	bumper9->listener = this;

	bumper10 = App->physics->CreateCircle(135, 100, 8, false, 1.0f, 1.0f);
	bumper10->listener = this;
	}
	
	//Lateral bumpers
	{
	

	int lateral_Bounce_right[10] = {
	26, 1,
	26, 61,
	1, 48,
	1, 19,
	5, 11
	};
	
	lateral_bumper_right = App->physics->CreateChain(209, 160, lateral_Bounce_right, false, 10, 1.1f);
	lateral_bumper_right->listener = this;
	
	int lateral_Bounce_left[10] = {
	0, 1,
	0, 60,
	24, 49,
	24, 18,
	20, 11
	};
	
	lateral_bumper_left = App->physics->CreateChain(12, 197, lateral_Bounce_left, false, 10, 1.1f);
	lateral_bumper_left->listener = this;
	
	}

}

void ModuleSceneIntro::CreateJoints() {

	b2RevoluteJoint* left_flipper_ball_joint = nullptr;
	b2RevoluteJoint* right_flipper_ball_joint = nullptr;
	b2RevoluteJointDef left_flipper_definition;
	b2RevoluteJointDef right_flipper_definition;
	
	left_flipper = App->physics->CreateRectangle(100, 340, 35, 8, true, 0.3f);
	right_flipper = App->physics->CreateRectangle(143, 342, 35, 8, true, 0.3f);
	
	left_flipper_ball = App->physics->CreateCircle(87, 342, 2, false, 0.25f, 1.0f);
	right_flipper_ball = App->physics->CreateCircle(154, 344, 2, false, 0.25f, 1.0f);
	
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

void ModuleSceneIntro::RestartBall(bool reset)
{
	
	if (reset == true)
	{
		ball->body->SetTransform(b2Vec2(PIXEL_TO_METERS(241), PIXEL_TO_METERS(340)), 0);
		if (score > highscore) {
			highscore = score;
		}
		score = 0;
		lives = 5;
		previous_score = 0;
	}
	else
	{
		ball->body->SetTransform(b2Vec2(PIXEL_TO_METERS(241), PIXEL_TO_METERS(340)), 0);
	}
}