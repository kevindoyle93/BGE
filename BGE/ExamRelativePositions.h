#pragma once
#include "Game.h"
#include "GameComponent.h"

using namespace std;

namespace BGE
{
	class ExamRelativePositions :
		public Game
	{
	public:
		ExamRelativePositions(void);

		shared_ptr<GameComponent> ship1;
		shared_ptr<GameComponent> ship2;
		float elapsed;
		bool Initialise();
		void Update(float timeDelta);
	};
}

