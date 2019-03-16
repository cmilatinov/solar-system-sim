#include "FontLoader.h"
#include "Display.h"

FontLoader::FontLoader(Loader * loader){
	this->loader = loader;
	backgroundTexID = loader->loadTexture("text_background.png");
}


FontLoader::~FontLoader(){
}

Font * FontLoader::loadFont(const char * fontName){

	std::string fontSource;
	std::ifstream fontFile;

	fontFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		fontFile.open(("fonts\\" + std::string(fontName) + ".fnt").c_str());
		std::stringstream fontStringStream;

		fontStringStream << fontFile.rdbuf();
		
		fontFile.close();

		fontSource = fontStringStream.str();
	}catch (std::ifstream::failure e){

		std::cout << "ERROR::FONT::FILE_NOT_SUCCESFULLY_READ" << std::endl;

	}

	std::vector<std::string> lines = StringUtils::split(fontSource, "\n");
	std::map<int, Character*> hashMap;
	int lineHeight, texWidth, texHeight;
	std::string fntName;
	for (std::string line : lines) {
		if (StringUtils::startsWith(line, "char id=")) {
			int id = stoi(line.substr(line.find("id=") + 3, 3));
			int x = stoi(line.substr(line.find("x=") + 2, 3));
			int y = stoi(line.substr(line.find("y=") + 2, 3));
			int width = stoi(line.substr(line.find("width=") + 6, 3));
			int height = stoi(line.substr(line.find("height=") + 7, 3));
			int xOffset = stoi(line.substr(line.find("xoffset=") + 8, 3));
			int yOffset = stoi(line.substr(line.find("yoffset=") + 8, 3));
			int xAdvance = stoi(line.substr(line.find("xadvance=") + 9, 3));
			Character * ch = new Character(id, x, y, width, height, xOffset, yOffset, xAdvance);
			hashMap.insert(std::make_pair(id, ch));
		}
		else if (StringUtils::startsWith(line, "info face=")) {
			fntName = line.substr(line.find("\"") + 1);
			fntName = fntName.substr(0, fntName.find("\""));
		}
		else if (StringUtils::startsWith(line, "common lineHeight=")) {
			lineHeight = stoi(line.substr(line.find("lineHeight=") + 11, 3));
			texWidth = stoi(line.substr(line.find("scaleW=") + 7, 3));
			texHeight = stoi(line.substr(line.find("scaleH=") + 7, 3));
		}
	}

	std::string texName = fontName + std::string(".png");
	unsigned int texID = loader->loadTexture(texName.c_str());
	
	return new Font(fntName, texID, texWidth, texHeight, lineHeight, hashMap);
}

TextMesh * FontLoader::loadString(std::string str, Font * font, int size, Vector2f screenPos) {
	float scaleFactor = (float)size / (float)font->getLineHeight();
	
	float screenWidth = Display::getWidth();
	float screenHeight = Display::getHeight();

	float maxWidth = 0.0f;

	std::vector<std::string> lines = StringUtils::split(str, "\n");
	int totalSpaces = 0;
	int totalCharCount = 0;
	for (std::string line : lines) {
		totalSpaces += StringUtils::numSpaces(line);
		totalCharCount += line.length();
	}
	 
	int * indices = new int[(totalCharCount - totalSpaces) * 6];
	float * vertices = new float[(totalCharCount - totalSpaces) * 12];
	float * uvs = new float[(totalCharCount - totalSpaces) * 8];

	int line = 0;
	Vector2f virtualCursor = Vector2f();
	int a = 0;
	int c = 0;
	for (int j = 0; j < lines.size(); j++) {
		virtualCursor = Vector2f(0, font->getLineHeight() * scaleFactor * j);
		for (int i = 0; i < lines.at(j).length(); i++) {
			int charID = StringUtils::getCharacterAt(lines.at(j), a);
			Character * ch = font->getCharacter(charID);
			if (charID != 32) {
				uvs[(8 * c)] = (float)ch->x / (float)font->getTextureWidth();
				uvs[(8 * c) + 1] = 1.0f - ((float)ch->y / (float)font->getTextureHeight());

				uvs[(8 * c) + 2] = (float)ch->x / (float)font->getTextureWidth();
				uvs[(8 * c) + 3] = 1.0f - ((float)(ch->y + ch->height) / (float)font->getTextureHeight());

				uvs[(8 * c) + 4] = (float)(ch->x + ch->width) / (float)font->getTextureWidth();
				uvs[(8 * c) + 5] = 1.0f - ((float)(ch->y + ch->height) / (float)font->getTextureHeight());

				uvs[(8 * c) + 6] = (float)(ch->x + ch->width) / (float)font->getTextureWidth();
				uvs[(8 * c) + 7] = 1.0f - ((float)ch->y / (float)font->getTextureHeight());

				vertices[(12 * c)] = (2.0f * (virtualCursor.x + scaleFactor * ch->xOffset) / screenWidth) - 1.0f;
				vertices[(12 * c) + 1] = (2.0f * (screenHeight - (virtualCursor.y + scaleFactor * ch->yOffset)) / screenHeight) - 1.0f;
				vertices[(12 * c) + 2] = -0.5f;

				vertices[(12 * c) + 3] = (2.0f * (virtualCursor.x + scaleFactor * ch->xOffset) / screenWidth) - 1.0f;
				vertices[(12 * c) + 4] = (2.0f * (screenHeight - (virtualCursor.y + scaleFactor * (ch->yOffset + ch->height))) / screenHeight) - 1.0f;
				vertices[(12 * c) + 5] = -0.5f;

				vertices[(12 * c) + 6] = (2.0f * (virtualCursor.x + scaleFactor * (ch->xOffset + ch->width)) / screenWidth) - 1.0f;
				vertices[(12 * c) + 7] = (2.0f * (screenHeight - (virtualCursor.y + scaleFactor * (ch->yOffset + ch->height))) / screenHeight) - 1.0f;
				vertices[(12 * c) + 8] = -0.5f;

				vertices[(12 * c) + 9] = (2.0f * (virtualCursor.x + scaleFactor * (ch->xOffset + ch->width)) / screenWidth) - 1.0f;
				vertices[(12 * c) + 10] = (2.0f * (screenHeight - (virtualCursor.y + scaleFactor * ch->yOffset)) / screenHeight) - 1.0f;
				vertices[(12 * c) + 11] = -0.5f;

				indices[(6 * c)] = (4 * c);
				indices[(6 * c) + 1] = (4 * c) + 1;
				indices[(6 * c) + 2] = (4 * c) + 3;
				indices[(6 * c) + 3] = (4 * c) + 3;
				indices[(6 * c) + 4] = (4 * c) + 1;
				indices[(6 * c) + 5] = (4 * c) + 2;

				c++;
			}
			a++;
			virtualCursor.x += scaleFactor * ch->xAdvance;
		}
		if (virtualCursor.x > maxWidth)
			maxWidth = virtualCursor.x;
		a = 0;
	}

	Mesh * generatedMesh = loader->loadToVAO(indices, (totalCharCount - totalSpaces) * 6 * sizeof(int), vertices, (totalCharCount - totalSpaces) * 12 * sizeof(float) , uvs, (totalCharCount - totalSpaces) * 8 * sizeof(float));
	
	Vector2f bounds = Vector2f(maxWidth, lines.size() * font->getLineHeight() * scaleFactor);
	
	Gui * background = new Gui(backgroundTexID, screenPos.x, screenPos.y, bounds.x, bounds.y);

	TextMesh * tMesh = new TextMesh(str, generatedMesh, font, screenPos, bounds, background, size);
	
	delete indices;
	delete vertices;
	delete uvs;

	return tMesh;
}

void FontLoader::updateMesh(TextMesh * mesh, std::string newText){
	Font * font = mesh->getFont();
	float scaleFactor = (float)mesh->size / (float)font->getLineHeight();

	float screenWidth = Display::getWidth();
	float screenHeight = Display::getHeight();

	float maxWidth = 0.0f;

	std::vector<std::string> lines = StringUtils::split(newText, "\n");
	int totalSpaces = 0;
	int totalCharCount = 0;
	for (std::string line : lines) {
		totalSpaces += StringUtils::numSpaces(line);
		totalCharCount += line.length();
	}

	int * indices = new int[(totalCharCount - totalSpaces) * 6];
	float * vertices = new float[(totalCharCount - totalSpaces) * 12];
	float * uvs = new float[(totalCharCount - totalSpaces) * 8];

	int line = 0;
	Vector2f virtualCursor = Vector2f();
	int a = 0;
	int c = 0;
	for (int j = 0; j < lines.size(); j++) {
		virtualCursor = Vector2f(0, font->getLineHeight() * scaleFactor * j);
		for (int i = 0; i < lines.at(j).length(); i++) {
			int charID = StringUtils::getCharacterAt(lines.at(j), a);
			Character * ch = font->getCharacter(charID);
			if (charID != 32) {
				uvs[(8 * c)] = (float)ch->x / (float)font->getTextureWidth();
				uvs[(8 * c) + 1] = 1.0f - ((float)ch->y / (float)font->getTextureHeight());

				uvs[(8 * c) + 2] = (float)ch->x / (float)font->getTextureWidth();
				uvs[(8 * c) + 3] = 1.0f - ((float)(ch->y + ch->height) / (float)font->getTextureHeight());

				uvs[(8 * c) + 4] = (float)(ch->x + ch->width) / (float)font->getTextureWidth();
				uvs[(8 * c) + 5] = 1.0f - ((float)(ch->y + ch->height) / (float)font->getTextureHeight());

				uvs[(8 * c) + 6] = (float)(ch->x + ch->width) / (float)font->getTextureWidth();
				uvs[(8 * c) + 7] = 1.0f - ((float)ch->y / (float)font->getTextureHeight());

				vertices[(12 * c)] = (2.0f * (virtualCursor.x + scaleFactor * ch->xOffset) / screenWidth) - 1.0f;
				vertices[(12 * c) + 1] = (2.0f * (screenHeight - (virtualCursor.y + scaleFactor * ch->yOffset)) / screenHeight) - 1.0f;
				vertices[(12 * c) + 2] = -0.5f;

				vertices[(12 * c) + 3] = (2.0f * (virtualCursor.x + scaleFactor * ch->xOffset) / screenWidth) - 1.0f;
				vertices[(12 * c) + 4] = (2.0f * (screenHeight - (virtualCursor.y + scaleFactor * (ch->yOffset + ch->height))) / screenHeight) - 1.0f;
				vertices[(12 * c) + 5] = -0.5f;

				vertices[(12 * c) + 6] = (2.0f * (virtualCursor.x + scaleFactor * (ch->xOffset + ch->width)) / screenWidth) - 1.0f;
				vertices[(12 * c) + 7] = (2.0f * (screenHeight - (virtualCursor.y + scaleFactor * (ch->yOffset + ch->height))) / screenHeight) - 1.0f;
				vertices[(12 * c) + 8] = -0.5f;

				vertices[(12 * c) + 9] = (2.0f * (virtualCursor.x + scaleFactor * (ch->xOffset + ch->width)) / screenWidth) - 1.0f;
				vertices[(12 * c) + 10] = (2.0f * (screenHeight - (virtualCursor.y + scaleFactor * ch->yOffset)) / screenHeight) - 1.0f;
				vertices[(12 * c) + 11] = -0.5f;

				indices[(6 * c)] = (4 * c);
				indices[(6 * c) + 1] = (4 * c) + 1;
				indices[(6 * c) + 2] = (4 * c) + 3;
				indices[(6 * c) + 3] = (4 * c) + 3;
				indices[(6 * c) + 4] = (4 * c) + 1;
				indices[(6 * c) + 5] = (4 * c) + 2;

				c++;
			}
			a++;
			virtualCursor.x += scaleFactor * ch->xAdvance;
		}
		if (virtualCursor.x > maxWidth)
			maxWidth = virtualCursor.x;
		a = 0;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getMesh()->vbos[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (totalCharCount - totalSpaces) * 6 * sizeof(int), indices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->getMesh()->vbos[1]);
	glBufferData(GL_ARRAY_BUFFER, (totalCharCount - totalSpaces) * 12 * sizeof(float), vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->getMesh()->vbos[2]);
	glBufferData(GL_ARRAY_BUFFER, (totalCharCount - totalSpaces) * 8 * sizeof(float), uvs, GL_DYNAMIC_DRAW);

	mesh->textString = newText;
	
	Vector2f bounds = Vector2f(maxWidth, lines.size() * font->getLineHeight() * scaleFactor);
	mesh->setBounds(bounds);
	mesh->getBackground()->setBounds(bounds);
	
	mesh->getMesh()->SetVertexCount((totalCharCount - totalSpaces) * 6);
	
	delete indices;
	delete vertices;
	delete uvs;
}
