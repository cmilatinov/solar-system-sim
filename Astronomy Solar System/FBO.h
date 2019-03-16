#include <GLAD/glad.h>
#include <string>
#include <vector>

#pragma once
class FBO
{
private:
	int width, height;
	int samples;
	unsigned int fboID;
	unsigned int depthBuffer;
	unsigned int colorBuffer;
	std::vector<unsigned int> textures;
	
	unsigned int createFBO();

	unsigned int createTextureAttachment(std::string type);

	unsigned int createDepthBufferAttachment();

	unsigned int createColorBufferAttachment();

	void addDepthAttachment();

	void addColorAttachment();

	void addDepthBuffer();

	void addColorBuffer();

public:
	FBO(int width, int height, bool needsDepthTexture);
	FBO(int width, int height, int samples);
	~FBO();

	void resolve(FBO * fbo);

	void bind();

	void unbind();

	int getTextureWidth();
	int getTextureHeight();

	unsigned int getID();

	std::vector<unsigned int> getTextureIDs();
};

