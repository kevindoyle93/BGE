#include "ExamSlerping.h"
#include "Content.h"
#include "VectorDrawer.h"
#include "LazerBeam.h"
#include "FountainEffect.h"
#include "Utils.h"


using namespace BGE;

ExamSlerping::ExamSlerping(void)
{
	elapsed = 10000;
	turnRate = glm::half_pi<float>(); // Turn half_pi radians per second
}


ExamSlerping::~ExamSlerping(void)
{
}

bool ExamSlerping::Initialise()
{
	std::shared_ptr<GameComponent> ground = make_shared<Ground>();
	Attach(ground);

	ship1 = make_shared<GameComponent>(true);
	ship1->Attach(Content::LoadModel("cobramk3", glm::rotate(glm::mat4(1), 180.0f, glm::vec3(0, 1, 0))));
	ship1->transform->position = glm::vec3(-10, 2, -10);
	ship1->Attach(make_shared<VectorDrawer>());
	Attach(ship1);

	ship2 = make_shared<GameComponent>(true);
	ship2->Attach(Content::LoadModel("ferdelance", glm::rotate(glm::mat4(1), 180.0f, glm::vec3(0, 1, 0))));
	ship2->Attach(make_shared<VectorDrawer>());
	ship2->transform->diffuse = glm::vec3(1.0f, 0.0f, 0.0f);
	ship2->transform->specular = glm::vec3(1.2f, 1.2f, 1.2f);

	ship2->transform->position = glm::vec3(10, 2, -10);
	Attach(ship2);

	slerping = false;
	t = 0.0f;

	Game::Initialise();

	camera->transform->position = glm::vec3(0, 4, 20);
	return true;
}

void ExamSlerping::Update(float deltaTime)
{
	// Movement of ship2
	if (keyState[SDL_SCANCODE_UP])
	{
		ship2->transform->Walk(speed * deltaTime);
	}
	if (keyState[SDL_SCANCODE_DOWN])
	{
		ship2->transform->Walk(-speed * deltaTime);
	}
	if (keyState[SDL_SCANCODE_LEFT])
	{
		ship2->transform->Yaw(deltaTime * speed * speed);
	}
	if (keyState[SDL_SCANCODE_RIGHT])
	{
		ship2->transform->Yaw(-deltaTime * speed * speed);
	}

	if (keyState[SDL_SCANCODE_O])
	{
		ship2->transform->Fly(deltaTime * speed);
	}

	if (keyState[SDL_SCANCODE_L])
	{
		ship2->transform->Fly(-deltaTime * speed);
	}

	if (keyState[SDL_SCANCODE_SPACE])
	{
		slerping = true;
		fromQuaternion = ship1->transform->orientation;

		glm::vec3 toShip2 = ship2->transform->position - ship1->transform->position;
		toShip2 = glm::normalize(toShip2);
		glm::vec3 axis = glm::cross(Transform::basisLook, toShip2);
		axis = glm::normalize(axis);
		float theta = glm::acos(glm::dot(toShip2, Transform::basisLook));
		toQuaternion = glm::angleAxis(glm::degrees(theta), axis);

		// Calculate the angle of rotation
		toRotate = glm::acos(glm::dot(toShip2, ship1->transform->look));
	}

	if (slerping)
	{
		ship1->transform->orientation = glm::mix(fromQuaternion, toQuaternion, t);
		float timeRemaining = toRotate / turnRate;
		t += 1.0f * (deltaTime / timeRemaining);
		if (t > 1.0f)
		{
			t = 0.0f;
			slerping = false;
		}
	}

	Game::Update(deltaTime);

}
