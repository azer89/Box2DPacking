
// ---------- BOX2D packing ----------

#ifndef __System_Params__
#define __System_Params__

#include <string>

class SystemParams
{
public:
	SystemParams();
	~SystemParams();

	static void LoadParameters();

public:
	// because square
	static float _upscaleFactor;
	static float _downscaleFactor;

	static float _box2DUpScaling;
	static float _box2DDownScaling;

	static int _function_length; // radius function length

	static std::string _image_folder;
	static std::string _artName;		// Art name
	static std::string _ornament_dir;	// ornaments

};

#endif