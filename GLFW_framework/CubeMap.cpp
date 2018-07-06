/*
tdogl::CubeMap

added by HL
*/

#include "CubeMap.h"
#include <stdexcept>

using namespace tdogl;

static GLenum TextureFormatForBitmapFormat(Bitmap::Format format)
{
	switch (format) {
	case Bitmap::Format_Grayscale: return GL_LUMINANCE;
	case Bitmap::Format_GrayscaleAlpha: return GL_LUMINANCE_ALPHA;
	case Bitmap::Format_RGB: return GL_RGB;
	case Bitmap::Format_RGBA: return GL_RGBA;
	default: throw std::runtime_error("Unrecognised Bitmap::Format");
	}
}

CubeMap::CubeMap(const Bitmap* bitmap) :
	_originalWidth((GLfloat)bitmap[0].width()),
	_originalHeight((GLfloat)bitmap[0].height())
{
	glGenTextures(1, &_object);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _object);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// expect the maps to be in the following order
	// top, bottom, front, back, left, right
	GLint faces[6] = { GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
					GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
					GL_TEXTURE_CUBE_MAP_POSITIVE_X,
					GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
					GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
					GL_TEXTURE_CUBE_MAP_POSITIVE_Z };

	for (int i = 0; i < 6; i++)
	{
		glTexImage2D(faces[i],
			0,
			TextureFormatForBitmapFormat(bitmap[i].format()),
			(GLsizei)bitmap[i].width(),
			(GLsizei)bitmap[i].height(),
			0,
			TextureFormatForBitmapFormat(bitmap[i].format()),
			GL_UNSIGNED_BYTE,
			bitmap[i].pixelBuffer());
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

CubeMap::~CubeMap()
{
	glDeleteTextures(1, &_object);
}

GLuint CubeMap::object() const
{
	return _object;
}

GLfloat CubeMap::originalWidth() const
{
	return _originalWidth;
}

GLfloat CubeMap::originalHeight() const
{
	return _originalHeight;
}
