// Library includes.
#define GLEW_STATIC
#include "GL/glew.h"

#include "GLFW/glfw3.h"

#define STBI_HEADER_FILE_ONLY
#include "stb_image.c"

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>

#include <stddef.h>


#ifdef WIN32

#define requires _ASSERT
#define promises _ASSERT

#else

#include <cassert>
#define requires assert
#define promises assert

#endif


// Constants:
namespace atc
{
	const float TILE_SCALE = 16.0f; // 24.0f; // pixels
	const float AXIS_LENGTH = 2.0f; // units

	const double TARGET_FRAME_RATE = 60.0; // frames per second
	const double TARGET_FRAME_TIME = ( 1.0 / 60.0 ); // seconds per frame
	const double MAX_ACCUMULATED_FRAME_TIME = ( TARGET_FRAME_TIME * 10.0 ); // seconds
}


// Forward declarations:
namespace atc
{
	class App;
	class Renderer;
	class Window;
	class Actor;
	class Camera;
	class HUD;
	class UnitSelection;
	class FormationBehavior;
	class Formation;
	class World;
	class Unit;
}


// Utility includes.
#include "MathHelper.h"
#include "Color.h"
#include "Angle.h"
#include "Vector.h"


// Header Includes:
#include "App.h"

namespace atc
{
	extern App g_app;
}

#include "Texture.h"
#include "Font.h"
#include "Renderer.h"
#include "Window.h"
#include "Actor.h"
#include "Camera.h"
#include "HUD.h"
#include "Orderable.h"
#include "UnitSelection.h"
#include "FormationBehavior.h"
#include "Formation.h"
#include "Path.h"
#include "Grid.h"
#include "MinHeap.h"
#include "Flowfield.h"
#include "Map.h"
#include "World.h"
#include "Unit.h"

// Inline Includes:
#include "App.inl"
#include "Color.inl"
#include "Angle.inl"
#include "Vector.inl"
#include "Texture.inl"
#include "Font.inl"
#include "Renderer.inl"
#include "Window.inl"
#include "Actor.inl"
#include "Camera.inl"
#include "HUD.inl"
#include "UnitSelection.inl"
#include "Formation.inl"
#include "FormationBehavior.inl"
#include "Path.inl"
#include "Grid.inl"
#include "MinHeap.inl"
#include "Flowfield.inl"
#include "Map.inl"
#include "World.inl"
#include "Unit.inl"