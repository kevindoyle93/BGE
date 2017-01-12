#include "RelativePositions.h"
#include "Content.h"
#include "VectorDrawer.h"
#include "LazerBeam.h"

using namespace BGE;

RelativePositions::RelativePositions(void)
{
	elapsed = 10000;
}


bool RelativePositions::Initialise()
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

void RelativePositions::Update(float timeDelta)
{
	bool inRange = false, inFov = false;

	// Check distance between components
	if (glm::length(ship1->transform->position - ship2->transform->position) < 15)
	{
		PrintText("In range");
		inRange = true;
	}

	// Check if component in front of other component
	glm::vec3 vec = ship2->transform->position - ship1->transform->position;
	float dotProduct = glm::dot(ship1->transform->look, vec);
	if (dotProduct > 0)
	{
		PrintText("In front");
	}

	// Check angle between components/if component in FOV
	int fov = 45;
	float angle = glm::degrees(glm::acos(dotProduct / glm::length(vec)));

	if (angle < fov / 2)
	{
		PrintText("In FOV");
		inFov = true;
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
