#pragma once
#include <string>

//If TextureName is "none" it will try to find color in model file
//And if it won't find, it will use default color and other values like roughenss etc.;

struct RSMODEL_DESC 
{
	std::string fileName;
	std::string textureFolder;
	std::string diffuseTexName;
	std::string normalTexName;
	std::string roughnessTexName;
	std::string metalnessTexName;
	std::string flag;
};
