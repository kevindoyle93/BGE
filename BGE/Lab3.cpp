#include "Lab3.h"
#include "Content.h"
#include "VectorDrawer.h"
#include "LazerBeam.h"

using namespace BGE;

Lab3::Lab3(void)
{
	elapsed = 10000;
}


bool Lab3::Initialise()
{
	std::shared_ptr<GameComponent> ground = make_shared<Ground>();
	Attach(ground);

	ship1 = make_shared<GameComponent>(true);
	ship1->Attach(Content::LoadModel("cobramk3", glm::rotate(glm::mat4(1), 180.0f, glm::vec3(0, 1, 0))));
	ship1->Attach(make_shared<VectorDrawer>(glm::vec3(5, 5, 5)));
	ship1->transform->position = glm::vec3(-10, 2, -10);
	Attach(ship1);

	ship2 = make_shared<GameComponent>(true);
	ship2->Attach(Content::LoadModel("python", glm::rotate(glm::mat4(1), 180.0f, glm::vec3(0, 1, 0))));
	ship2->Attach(make_shared<VectorDrawer>(glm::vec3(5, 5, 5)));
	ship2->transform->position = glm::vec3(10, 2, -10);
	Attach(ship2);
	Game::Initialise();

	camera->transform->position = glm::vec3(0, 4, 20);
	return true;
}

void Lab3::Update(float timeDelta)
{
	static float timeToFire = 1.0f / 2.0f;
	bool inRange = false, inFov = false;

	// Check range
	if (glm::length(ship1->transform->position - ship2->transform->position) < 15)
	{
		PrintText("In range");
		inRange = true;
	}

	// Check if in front
	glm::vec3 vec = ship2->transform->position - ship1->transform->position;
	float dotProduct = glm::dot(ship1->transform->look, vec);
	if (dotProduct > 0)
	{
		PrintText("In front");
	}

	// Check if in FOV
	int fov = 45;
	float ship1Len = glm::length(ship1->transform->position);
	float angle = glm::degrees(glm::acos(dotProduct / glm::length(vec)));

	if (angle < fov / 2)
	{
		PrintText("In FOV");
		inFov = true;
	}

	if (inRange && inFov && elapsed > timeToFire)
	{
		PrintText("Pew Pew");
		shared_ptr<LazerBeam> lazer = make_shared<LazerBeam>();
		lazer->transform->position = ship1->transform->position;
		lazer->transform->look = ship1->transform->look;
		Attach(lazer);
		elapsed = 0;
	}

	// Movement of ship1
	if (keyState[SDL_SCANCODE_U])
	{
		ship1->transform->position += ship1->transform->look * speed * timeDelta;
	}
	if (keyState[SDL_SCANCODE_J])
	{
		ship1->transform->position -= ship1->transform->look * speed * timeDelta;
	}
	if (keyState[SDL_SCANCODE_H])
	{
		ship1->transform->Yaw(speed / 5.0);
	}
	if (keyState[SDL_SCANCODE_K])
	{
		ship1->transform->Yaw(-speed / 5.0);
	}
	if (keyState[SDL_SCANCODE_Y])
	{
		ship1->transform->Roll(speed / 5.0);
	}
	if (keyState[SDL_SCANCODE_I])
	{
		ship1->transform->Roll(-speed / 5.0);
	}
	if (keyState[SDL_SCANCODE_G])
	{
		ship1->transform->Pitch(speed / 5.0);
	}
	if (keyState[SDL_SCANCODE_L])
	{
		ship1->transform->Pitch(-speed / 5.0);
	}

	// Movement of ship2
	if (keyState[SDL_SCANCODE_UP])
	{
		ship2->transform->position += ship2->transform->look * speed * timeDelta;
	}
	if (keyState[SDL_SCANCODE_DOWN])
	{
		ship2->transform->position -= ship2->transform->look * speed * timeDelta;
	}
	if (keyState[SDL_SCANCODE_LEFT])
	{
		ship2->transform->Yaw(speed / 5.0);
	}
	if (keyState[SDL_SCANCODE_RIGHT])
	{
		ship2->transform->Yaw(-speed / 5.0);
	}
	elapsed += timeDelta;


	Game::Update(timeDelta);
}
