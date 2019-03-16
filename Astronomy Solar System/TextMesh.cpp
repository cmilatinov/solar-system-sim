#include "TextMesh.h"
#include "Display.h"


TextMesh::TextMesh(std::string textString, Mesh * mesh, Font * font, Vector2f pos, Vector2f bounds, Gui * background, int size){
	this->textString = textString;
	this->mesh = mesh;
	this->font = font;
	this->pos = pos;
	this->bounds = bounds;
	this->background = background;
	this->size = size;
	this->modelMatrix = new Matrix4f();
	setScreenPosition(pos);
}


TextMesh::~TextMesh(){
	delete mesh;
}

Mesh * TextMesh::getMesh(){
	return mesh;
}

Font * TextMesh::getFont(){
	return font;
}

Vector2f TextMesh::getScreenPosition(){
	return pos;
}

Vector2f TextMesh::getTextBounds(){
	return bounds;
}

void TextMesh::setScreenPosition(Vector2f pos){
	this->pos = pos;
	modelMatrix->SetIdentity();
	modelMatrix->Translate(Vector3f(2.0f * pos.x / (float)Display::getWidth(),
									-2.0f * pos.y / (float)Display::getHeight(), -0.1f));
	background->setPosition(pos);
}

void TextMesh::setBounds(Vector2f bounds){
	this->bounds = bounds;
}

Gui * TextMesh::getBackground(){
	return background;
}

Matrix4f * TextMesh::getModelMatrix(){
	return modelMatrix;
}
