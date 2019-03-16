#include <string>
#include <map>

#pragma once
class Character {
public:
	unsigned char c;
	int charID;
	int x, y, width, height, xOffset, yOffset, xAdvance;

	Character(int charID, int x, int y, int width, int height, int xOffset, int yOffset, int xAdvance) {
		this->c = unsigned char(charID);
		this->charID = charID;
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->xOffset = xOffset;
		this->yOffset = yOffset;
		this->xAdvance = xAdvance;
	}
};

class Font
{
private:
	int numChars;
	unsigned int texID;
	int texWidth, texHeight;
	int lineHeight;

	std::string name;

	std::map<int, Character*> charHashMap;

public:
	Font(std::string name, unsigned int texID, int texWidth, int texHeight, int lineHeight, std::map<int, Character*> charHashMap);
	~Font();
	
	Character * getCharacter(int charID);

	std::string getFontName();

	int getTextureID();
	int getTextureWidth();
	int getTextureHeight();
	int getCharacterCount();
	int getLineHeight();
};

