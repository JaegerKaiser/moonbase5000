#pragma once
#include "glew.h"
#include "Bitmap.h"

namespace tdogl {

	/**
	Represents a  CubeMap
	*/
	class CubeMap {
	public:
		/**
		Creates a cubemap from 6 bitmaps.

		The textures will be loaded upside down because tdogl::Bitmap pixel data
		is ordered from the top row down, but OpenGL expects the data to
		be from the bottom row up.

		@param *bitmap  The array of six bitmaps to load the cubemap from
		*/
		CubeMap(const Bitmap* bitmap);

		/**
		Deletes the texture object with glDeleteTextures
		*/
		~CubeMap();

		/**
		@result The texure object, as created by glGenTextures
		*/
		GLuint object() const;

		/**
		@result The original width (in pixels) of the bitmap this texture was made from
		*/
		GLfloat originalWidth() const;

		/**
		@result The original height (in pixels) of the bitmap this texture was made from
		*/
		GLfloat originalHeight() const;

	private:
		GLuint _object;
		GLfloat _originalWidth;
		GLfloat _originalHeight;

		//copying disabled
		CubeMap(const CubeMap&);
		const CubeMap& operator=(const CubeMap&);
	};

}
