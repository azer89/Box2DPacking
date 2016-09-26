

// ---------- BOX2D packing ----------

#include "Box2DSystemParams.h"

#include "LuaScript.h"

void Box2DSystemParams::LoadParameters()
{
	std::cout << "BOX2D packing SystemParams::LoadParameters\n";

	LuaScript script("C:\\Users\\azer\\workspace\\Box2DPacking\\Box2DPacking\\box2d_params.lua");

	//Box2DSystemParams::_upscaleFactor = script.get<float>("_upscaleFactor");
	//Box2DSystemParams::_downscaleFactor = script.get<float>("_downscaleFactor");

	Box2DSystemParams::_box2DUpScaling = script.get<float>("_box2DUpScaling");
	Box2DSystemParams::_box2DDownScaling = script.get<float>("_box2DDownScaling");

	//Box2DSystemParams::_function_length = script.get<int>("_function_length");

	//Box2DSystemParams::_image_folder = script.get<std::string>("_image_folder");
	//Box2DSystemParams::_artName = script.get<std::string>("_artName");
	//Box2DSystemParams::_ornament_dir = script.get<std::string>("_ornament_dir");

	//std::cout << "SystemParams::_upscaleFactor    : " << Box2DSystemParams::_upscaleFactor << "\n";
	//std::cout << "SystemParams::_downscaleFactor  : " << Box2DSystemParams::_downscaleFactor << "\n";
	std::cout << "SystemParams::_box2DUpScaling   : " << Box2DSystemParams::_box2DUpScaling << "\n";
	std::cout << "SystemParams::_box2DDownScaling : " << Box2DSystemParams::_box2DDownScaling << "\n";
	//std::cout << "SystemParams::_function_length  : " << Box2DSystemParams::_function_length << "\n";
	//std::cout << "SystemParams::_image_folder     : " << Box2DSystemParams::_image_folder << "\n";
	//std::cout << "SystemParams::_artName          : " << Box2DSystemParams::_artName << "\n";
	//std::cout << "SystemParams::_ornament_dir     : " << Box2DSystemParams::_ornament_dir << "\n";

	std::cout << "SystemParams::LoadParameters OK\n";
}


//float Box2DSystemParams::_upscaleFactor = 0.0f;
//float Box2DSystemParams::_downscaleFactor = 0.0f;

float Box2DSystemParams::_box2DUpScaling = 0.0f;
float Box2DSystemParams::_box2DDownScaling = 0.0f;

//int Box2DSystemParams::_function_length = 0;

//std::string Box2DSystemParams::_image_folder = "";
//std::string Box2DSystemParams::_artName = "";
//std::string Box2DSystemParams::_ornament_dir = "";