#include <GL/glew.h>
#include <IL/il.h>
#include <string>
#include "../Engine/src/texture.h"

int main(int argc, char* argv[])
{	
	ilInit();
	cgj::StarmapGenerator generator;	
	auto data = generator.generate(2048);
	ILuint id[6];
	ilGenImages(6, id);
	for (int i = 0; i < 6; ++i) {
		ilBindImage(id[i]);
		ilLoadL(IL_BMP, data[i], 0);
		std::string name = "cubemap_" + i;
		ilSave(IL_BMP, name.c_str());
	}
}