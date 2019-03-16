#include "DynamicTextMesh.h"


DynamicTextMesh::DynamicTextMesh(std::string textString, Mesh * mesh, Font * font, Vector2f pos, Vector2f bounds, Gui * background, int size) : TextMesh(textString, mesh, font, pos, bounds, background, size){
	
}

DynamicTextMesh::DynamicTextMesh(TextMesh * textMesh) : TextMesh(*textMesh){

}

DynamicTextMesh::~DynamicTextMesh(){

}

void DynamicTextMesh::setText(std::string text, FontLoader * loader){
	loader->updateMesh(this, text);
}

void DynamicTextMesh::setLowerLeftCornerPos(Vector2f pos){
	this->setScreenPosition(Vector2f(pos.x, pos.y - this->getTextBounds().y));
}

void DynamicTextMesh::setLowerRightCornerPos(Vector2f pos) {
	this->setScreenPosition(Vector2f(pos.x - this->getTextBounds().x, pos.y - this->getTextBounds().y));
}
