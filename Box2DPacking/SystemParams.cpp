

// ---------- BOX2D packing ----------

#include "SystemParams.h"

#include "LuaScript.h"

void SystemParams::LoadParameters()
{
	std::cout << "BOX2D packing SystemParams::LoadParameters\n";

	LuaScript script("C:\\Users\\azer\\workspace\\Box2DPacking\\Box2DPacking\\params.lua");

	SystemParams::_upscaleFactor    = script.get<float>("_upscaleFactor");
	SystemParams::_downscaleFactor  = script.get<float>("_downscaleFactor");

	SystemParams::_box2DUpScaling   = script.get<float>("_box2DUpScaling");
	SystemParams::_box2DDownScaling = script.get<float>("_box2DDownScaling");

	SystemParams::_function_length  = script.get<int>("_function_length");

	SystemParams::_image_folder     = script.get<std::string>("_image_folder");
	SystemParams::_artName          = script.get<std::string>("_artName");
	SystemParams::_ornament_dir     = script.get<std::string>("_ornament_dir");

	std::cout << "SystemParams::LoadParameters OK\n";
}


float SystemParams::_upscaleFactor    = 0.0f;
float SystemParams::_downscaleFactor  = 0.0f;

float SystemParams::_box2DUpScaling   = 0.0f;
float SystemParams::_box2DDownScaling = 0.0f;

int SystemParams::_function_length = 0;

std::string SystemParams::_image_folder = "";
std::string SystemParams::_artName      = "";
std::string SystemParams::_ornament_dir = "";