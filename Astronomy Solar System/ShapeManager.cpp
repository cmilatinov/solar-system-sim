#include "ShapeManager.h"


std::map<Mesh*, std::vector<Shape*>> shapes;

ShapeManager::ShapeManager(Loader * loader){
	this->loader = loader;
}

ShapeManager::~ShapeManager(){

}

std::vector<Shape*> ShapeManager::getAllShapes(){
	std::vector<Shape*> shapeList;
	for (std::map<Mesh*, std::vector<Shape*>>::iterator it = shapes.begin(); it != shapes.end(); ++it) {
		for (Shape * shape : it->second) {
			shapeList.push_back(shape);
		}
	}
	return shapeList;
}

std::map<Mesh*, std::vector<Shape*>> ShapeManager::getShapesHashmap(){
	return shapes;
}

void ShapeManager::addShape(Shape * sh) {
	std::map <Mesh*, std::vector<Shape*>>::iterator it = shapes.find(sh->getModel());
	std::vector<Shape*> shList;
	if (it == shapes.end()) {
		shList.push_back(sh);
		shapes.insert(std::make_pair(sh->getModel(), shList));
	}else {
		it->second.push_back(sh);
	}
}

void ShapeManager::removeShape(Shape * shape) {
	for (std::map<Mesh*, std::vector<Shape*>>::iterator it = shapes.begin(); it != shapes.end(); ++it) {
		if (it->first == shape->getModel()) {
			for (std::vector<Shape*>::iterator i = it->second.begin(); i != it->second.end(); ++i) {
				if (*i == shape) {
					it->second.erase(i);
					return;
				}
			}
		}
	}
}