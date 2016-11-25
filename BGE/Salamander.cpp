#include "Salamander.h"
#include "Content.h"
using namespace BGE;


Salamander::Salamander(shared_ptr<PhysicsFactory> _physicsFactory)
{
	physicsFactory = _physicsFactory;
	force = glm::vec3(0);
}

Salamander::Salamander()
{

}

Salamander::~Salamander(void)
{

}

bool Salamander::Initialise()
{
	return true;
}

shared_ptr<PhysicsController> Salamander::CreateSalamander(glm::vec3 position, int numSections, float sectionWidth, float sectionHeight, float sectionDepth)
{
	head = physicsFactory->CreateBox(sectionWidth, sectionHeight, sectionDepth, position, glm::quat());

	glm::vec3 gap = glm::vec3(0, 0, sectionDepth + sectionDepth / 4);

	shared_ptr<PhysicsController> sectionToConnect = head;
	for (int i = 0; i < numSections; i++)
	{
		position += gap;
		sectionToConnect = CreateBodySection(position, sectionToConnect, sectionWidth, sectionHeight, sectionDepth);
		bodySections.push_back(sectionToConnect);
	}

	CreateLegs(
		bodySections[0],
		sectionWidth,
		sectionHeight,
		sectionDepth / 2
		);

	CreateLegs(
		bodySections[bodySections.size() - 1],
		sectionWidth,
		sectionHeight,
		sectionDepth / 2
		);

	return head;
}

shared_ptr<PhysicsController> Salamander::CreateBodySection(glm::vec3 position, shared_ptr<PhysicsController> sectionToConnect, float w, float h, float d)
{
	shared_ptr<PhysicsController> section = physicsFactory->CreateBox(w, h, d, position, glm::quat());
	bodySections.push_back(section);

	btHingeConstraint * hinge = new btHingeConstraint(*sectionToConnect->rigidBody, *section->rigidBody, btVector3(0, 0, 2.5f), btVector3(0, 0, -2.5f), btVector3(0, 1, 0), btVector3(0, 1, 0), true);
	hinge->setLimit(btScalar(-0.20f), btScalar(0.20f));

	physicsFactory->dynamicsWorld->addConstraint(hinge);

	return section;
}

void Salamander::CreateLegs(shared_ptr<PhysicsController> bodySection, float w, float h, float d)
{
	int rightLeftMupltiplier[] = { -1, 1 };

	for each (int side in rightLeftMupltiplier)
	{
		float angle = -90.0f * side;
		glm::vec3 offset = glm::vec3(d / 2, 0, 0) * float(side);

		// Create upper leg section
		glm::vec3 position = bodySection->transform->position + offset;
		shared_ptr<PhysicsController> upperLeg = physicsFactory->CreateBox(w, h, d, position, glm::angleAxis(angle, glm::vec3(0, 1, 0)));
		btHingeConstraint * hinge = new btHingeConstraint(
			*bodySection->rigidBody,
			*upperLeg->rigidBody,
			btVector3(w * side * 0.5f, 0, 0),
			btVector3(0, 0, d * 0.625),
			btVector3(0, 1, 0),
			btVector3(0, 1, 0),
			true
			);
		physicsFactory->dynamicsWorld->addConstraint(hinge);

		// Create lower leg section
		offset = glm::vec3(d * 0.75f * side, -d, 0);
		position += offset;
		shared_ptr<PhysicsController> lowerLeg = physicsFactory->CreateBox(w, h, d, position, glm::angleAxis(angle, glm::vec3(1, 0, 0)));
		hinge = new btHingeConstraint(
			*upperLeg->rigidBody,
			*lowerLeg->rigidBody,
			btVector3(0, -h / 2, -d / 2),
			btVector3(0, 0, d * 0.625),
			btVector3(1, 0, 0),
			btVector3(1, 0, 0)
			);
		hinge->setLimit(glm::quarter_pi<float>(), glm::quarter_pi<float>());
		physicsFactory->dynamicsWorld->addConstraint(hinge);

		legs.push_back(upperLeg);
	}
}

void Salamander::Update(float timeDelta)
{
	const Uint8 * keyState = Game::Instance()->GetKeyState();

	float scale = 10000.0f;

	if (keyState[SDL_SCANCODE_UP])
	{
		force += bodySections[bodySections.size() - 1]->transform->right * scale * timeDelta;
	}
	if (keyState[SDL_SCANCODE_DOWN])
	{
		force += bodySections[bodySections.size() - 1]->transform->right * scale * timeDelta;
	}

	velocity += force * timeDelta;
	bodySections[bodySections.size() - 1]->transform->position += velocity * timeDelta;

	if (glm::length(velocity) > 0.0001f)
	{
		bodySections[bodySections.size() - 1]->transform->look = glm::normalize(velocity);
		bodySections[bodySections.size() - 1]->transform->right = glm::cross(transform->look, transform->up);
		velocity *= 0.99f;
	}

	// Reset the force
	force = glm::vec3(0);

	GameComponent::Update(timeDelta);
}

/*

void Steerable2DController::Update(float timeDelta)
{
const Uint8 * keyState = Game::Instance()->GetKeyState();

float scale = 10000.0f;

if (keyState[keyForward])
{
force += transform->look * scale * timeDelta;
}
if (keyState[keyBackwards])
{
force += -transform->look * scale * timeDelta;
}
if (keyState[keyRight])
{
force += transform->right * scale * timeDelta;
}
if (keyState[keyLeft])
{
force += -transform->right * scale * timeDelta;
}

velocity += force * timeDelta;
transform->position += velocity * timeDelta;

if (glm::length(velocity) > 0.0001f)
{
transform->look = glm::normalize(velocity);
transform->right = glm::cross(transform->look, transform->up);
velocity *= 0.99f;
}

// Reset the force
force = glm::vec3(0);

GameComponent::Update(timeDelta);
}

*/
