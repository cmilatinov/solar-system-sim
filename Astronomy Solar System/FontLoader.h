#include "Font.h"
#include "utils.h"
#include "Loader.h"
#include "TextMesh.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#pragma once
class FontLoader
{
private:
	Loader * loader;

	unsigned int backgroundTexID;

public:
	FontLoader(Loader * loader);
	~FontLoader();

	Font * loadFont(const char * fontName);
	
	TextMesh * loadString(std::string str, Font * font, int size, Vector2f screenPos);

	void updateMesh(TextMesh * mesh, std::string newText);
};

