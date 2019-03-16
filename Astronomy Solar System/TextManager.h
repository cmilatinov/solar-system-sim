#include "TextMesh.h"

#include <vector>
#pragma once
class TextManager
{
public:
	TextManager();
	~TextManager();

	std::vector<TextMesh*> getAllTexts();

	void addText(TextMesh * text);

	void removeText(TextMesh * text);

};

