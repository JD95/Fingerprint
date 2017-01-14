#include "Texture.h"
#include "SOIL.h"


Texture::Texture(std::string texture_name)
{
	texture_id = SOIL_load_OGL_texture(
		texture_name.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	/* check for an error during the load process */
	if (0 == texture_id)
	{
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}
}


Texture::~Texture()
{
	glDeleteTextures(1, &texture_id);
}
