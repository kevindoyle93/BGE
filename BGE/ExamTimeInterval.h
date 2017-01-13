#pragma once
#include "Game.h"
#include "GameComponent.h"

using namespace std;

namespace BGE
{
	class ExamTimeInterval :
		public Game
	{
	public:
		ExamTimeInterval(void);

		shared_ptr<GameComponent> ship1;
		float elapsed;
		bool Initialise();
		void Update(float timeDelta);
	};
}

