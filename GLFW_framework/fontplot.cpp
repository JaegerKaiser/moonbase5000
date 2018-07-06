#include "fontplot.h"

FontPlot::FontPlot(const char *font, const char *vertexShader, const char *fragmentShader, int width, int height )
{
	screenWidth = width;
	screenHeight = height;
	// load the shaders
	LoadShaders(vertexShader, fragmentShader);
	// load the font
	pFont = new tdogl::Font(font);
	// initialize the vertex data - this will be a quad, 0 to 1 in both directions, and uv.
	// these positions and uvs are transformed by shader uniforms on plotting
	// note - v's are flipped
	float vData[6 * 4];
	vData[0 * 4 + 0] = 0.0f; vData[0 * 4 + 1] = 0.0f; vData[0 * 4 + 2] = 0.0f; vData[0 * 4 + 3] = 1.0f;
	vData[1 * 4 + 0] = 0.0f; vData[1 * 4 + 1] = 1.0f; vData[1 * 4 + 2] = 0.0f; vData[1 * 4 + 3] = 0.0f;
	vData[2 * 4 + 0] = 1.0f; vData[2 * 4 + 1] = 1.0f; vData[2 * 4 + 2] = 1.0f; vData[2 * 4 + 3] = 0.0f;
	vData[3 * 4 + 0] = 0.0f; vData[3 * 4 + 1] = 0.0f; vData[3 * 4 + 2] = 0.0f; vData[3 * 4 + 3] = 1.0f;
	vData[4 * 4 + 0] = 1.0f; vData[4 * 4 + 1] = 1.0f; vData[4 * 4 + 2] = 1.0f; vData[4 * 4 + 3] = 0.0f;
	vData[5 * 4 + 0] = 1.0f; vData[5 * 4 + 1] = 0.0f; vData[5 * 4 + 2] = 1.0f; vData[5 * 4 + 3] = 1.0f;
	SetVertexData(vData, 6, 4);
}

void FontPlot::SetVertexData(float *vData, int numVerts, int vertSize)
{
	drawType = GL_TRIANGLES;
	drawStart = 0;
	drawCount = numVerts * 3;
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	// bind the VAO
	glBindVertexArray(vao);

	// bind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertSize*numVerts, vData, GL_STATIC_DRAW);

	// connect the xyz to the "vert" attribute of the vertex shader
	glEnableVertexAttribArray(shaders->attrib("vert"));
	glVertexAttribPointer(shaders->attrib("vert"), 2, GL_FLOAT, GL_FALSE, vertSize * sizeof(GLfloat), (const GLvoid*)0);

	// connect the uv coords to the "vertTexCoord" attribute of the vertex shader
	glEnableVertexAttribArray(shaders->attrib("vertTexCoord"));
	glVertexAttribPointer(shaders->attrib("vertTexCoord"), 2, GL_FLOAT, GL_FALSE, vertSize * sizeof(GLfloat), (const GLvoid*)(2 * sizeof(GLfloat)));

	// unbind the VAO
	glBindVertexArray(0);
}

void FontPlot::LoadShaders(const char *v_shader, const char *f_shader)
{
	std::vector<tdogl::Shader> vs;
	vs.push_back(tdogl::Shader::shaderFromFile(v_shader, GL_VERTEX_SHADER));
	vs.push_back(tdogl::Shader::shaderFromFile(f_shader, GL_FRAGMENT_SHADER));
	shaders = new tdogl::Program(vs);
}

void FontPlot::DrawChar(float x, float y, char c, const glm::vec4 &colour)
{
	//bind the shaders
	shaders->use();

	//set the shader uniforms
	shaders->setUniform("fontTex", 0); //set to 0 because the texture will be bound to GL_TEXTURE0

	// work out the screen coordinates here
	float w, h, xo, yo;
	// note: screen goes -1 to 1 in both directions (normalized device coordinates). x, y passed in in pixels, width height etc from font also in pixels.
	// both need converting using the screen width and height.
	x = 2.0f*(x - screenWidth*0.5f) / screenWidth;
	y = 2.0f*(y - screenHeight*0.5f) / screenHeight;
	w = 2.0f*(float)pFont->GetW(c)/screenWidth;
	h = 2.0f*(float)pFont->GetH(c)/screenHeight;
	xo = 2.0f*(float)pFont->GetXOffs(c) / screenWidth;
	yo = 2.0f*(float)pFont->GetYOffs(c) / screenHeight;

	float u0, v0, uSize, vSize;
	u0 = (float)pFont->GetX(c) / pFont->GetTexSize();
	v0 = (float)pFont->GetY(c) / pFont->GetTexSize();
	uSize = (float)pFont->GetW(c) / pFont->GetTexSize();
	vSize = (float)pFont->GetH(c) / pFont->GetTexSize();
	// set shader uniforms for transform
	shaders->setUniform("xPos", x+xo);
	shaders->setUniform("yPos", y-h-yo);
	shaders->setUniform("xSize", w);
	shaders->setUniform("ySize", h);
	shaders->setUniform("uPos", u0);
	shaders->setUniform("vPos", v0);
	shaders->setUniform("uSize", uSize);
	shaders->setUniform("vSize", vSize);
	shaders->setUniform("colour", colour);

	//bind the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, pFont->GetTexture()->object());

	//bind VAO and draw
	glBindVertexArray(vao);
	// turn off z test
	glDepthFunc(GL_ALWAYS);
	glDrawArrays(drawType, drawStart, drawCount);
	// reenable z buffer
	glDepthFunc(GL_LESS);

	//unbind everything
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	shaders->stopUsing();
}

void FontPlot::DrawString(int x, int y, const std::string &str, const glm::vec4 &colour, Justify just)
{
	int width = 0;
	for (int i = 0; i < str.length(); i++)
	{
		width += pFont->GetXAdvance(str[i]);
	}
	if (just == CENTRE)
		x -= width / 2;
	else if (just == RIGHT)
		x -= width;
	for (int i = 0; i < str.length(); i++)
	{
		DrawChar(x, y, str[i], colour);
		x += pFont->GetXAdvance(str[i]);
	}
}


