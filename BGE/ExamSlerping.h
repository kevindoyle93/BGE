#pragma once
#include "Game.h"
#include "GameComponent.h"
#include "FountainEffect.h"

using namespace std;

namespace BGE
{
	class ExamSlerping :
		public Game
	{
	public:
		ExamSlerping(void);
		~ExamSlerping(void);

		shared_ptr<GameComponent> ship1;
		shared_ptr<GameComponent> ship2;
		float turnRate;
		float toRotate;
		float elapsed;
		bool Initialise();
		void Update(float deltaTime);
		bool slerping;
		glm::quat fromQuaternion;
		glm::quat toQuaternion;
		float t;
	};
}

