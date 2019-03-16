#include "TextManager.h"


static std::vector<TextMesh*> texts;

TextManager::TextManager(){
}


TextManager::~TextManager(){
	for (TextMesh * text : texts)
		delete text;
}

std::vector<TextMesh*> TextManager::getAllTexts(){
	return texts;
}

void TextManager::addText(TextMesh * text){
	texts.push_back(text);
}

void TextManager::removeText(TextMesh * text){
	for (std::vector<TextMesh*>::iterator it = texts.begin(); it != texts.end(); ++it) {
		if (*it == text) {
			texts.erase(it);
			return;
		}
	}
}
