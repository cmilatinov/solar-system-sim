#include "Font.h"



Font::Font(std::string name, unsigned int texID, int texWidth, int texHeight, int lineHeight, std::map<int, Character*> charHashMap){
	this->name = name;
	this->texID = texID;
	this->texWidth = texWidth;
	this->texHeight = texHeight;
	this->lineHeight = lineHeight;
	this->charHashMap = charHashMap;
	this->numChars = charHashMap.size();
}

Font::~Font(){
	for (std::map<int, Character*>::iterator it = charHashMap.begin(); it != charHashMap.end(); ++it)
		delete it->second;
}

Character * Font::getCharacter(int charID){
	return charHashMap.find(charID)->second;
}

std::string Font::getFontName(){
	return this->name;
}

int Font::getTextureID(){
	return this->texID;
}

int Font::getTextureWidth(){
	return this->texWidth;
}

int Font::getTextureHeight()
{
	return this->texHeight;
}

int Font::getCharacterCount(){
	return this->numChars;
}

int Font::getLineHeight(){
	return lineHeight;
}
