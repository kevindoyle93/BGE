
#ifdef _WIN32
 #include <windows.h>
#endif 
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include<glm.hpp>
#include "Game.h"
#include "TriangleTest.h"
#include "TexturedCube.h"
#include "CubeTest.h"
#include "PhysicsGame1.h"
#include "ModelTest.h"
#include "SteeringGame.h"
#include "SceneGraphGame.h"
//#include "Lab4.h"
//#include "Lab5.h"
#include "Lab3.h"
//#include "Lab6.h"
//#include "Lab7.h"
#include "Buddha.h"
#include "Game.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <ctime>

#include <OVR.h>
#include <OVR_CAPI_GL.h>

#include "Content.h"
#include "FPSController.h"
#include "XBoxController.h"
#include "Steerable3DController.h"
#include "Utils.h"
#include "VRGame2.h"
#include "SDL_syswm.h"
#include "ExamRelativePositions.h"
#include "ExamSlerping.h"

using namespace BGE;

int main(int argc, char *argv[])
{
	// http://www.youtube.com/watch?v=EEbVHxOkTxw
	Params::Load("default");

	// Run game with components in front/behind each other, distance between components, and angle/FOV
	// shared_ptr<Game> game = make_shared<ExamRelativePositions>();

	// Run game with quaternion slerping
	shared_ptr<Game> game = make_shared<ExamSlerping>();
	game->Run();
	
	return 0;
}