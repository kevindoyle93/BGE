#include "ExamTimeInterval.h"
#include "Content.h"
#include "VectorDrawer.h"
#include "LazerBeam.h"

using namespace BGE;

ExamTimeInterval::ExamTimeInterval(void)
{
	elapsed = 10000;
}


bool ExamTimeInterval::Initialise()
{
	std::shared_ptr<GameComponent> ground = make_shared<Ground>();
	Attach(ground);

	ship1 = make_shared<GameComponent>(true);
	ship1->Attach(Content::LoadModel("cobramk3", glm::rotate(glm::mat4(1), 180.0f, glm::vec3(0, 1, 0))));
	ship1->Attach(make_shared<VectorDrawer>(glm::vec3(5, 5, 5)));
	ship1->transform->position = glm::vec3(-10, 2, -10);
	Attach(ship1);

	Game::Initialise();

	camera->transform->position = glm::vec3(0, 4, 20);
	return true;
}

void ExamTimeInterval::Update(float timeDelta)
{
	// fireInterval in seconds between shots
	static float fireInterval = 0.5f;

	if (elapsed > fireInterval)
	{
		// Create lazer
		shared_ptr<LazerBeam> lazer = make_shared<LazerBeam>();
		lazer->transform->position = ship1->transform->position;
		lazer->transform->look = ship1->transform->look;
		Attach(lazer);

		// Reset elapsed to begin counting interval until next shot
		elapsed = 0;
	}

	elapsed += timeDelta;


	Game::Update(timeDelta);
}
