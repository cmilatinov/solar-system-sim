#include "Shader.h"
#include "StaticShader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath){
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(("shaders\\" + std::string(vertexPath)).c_str());
		fShaderFile.open(("shaders\\" + std::string(fragmentPath)).c_str());
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		//std::cout << vertexCode << std::endl;
		//std::cout << fragmentCode << std::endl;
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char *vShaderCode = vertexCode.c_str();
	const char *fShaderCode = fragmentCode.c_str();
	// 2. compile shaders
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	if (!checkCompileErrors(vertex, "VERTEX"));// exit(-1);
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	if (!checkCompileErrors(fragment, "FRAGMENT"));// exit(-2);
	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
}

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath) {
	// retrieve the vertex/geometry/fragment source code from filePath
	std::string vertexCode;
	std::string geometryCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream gShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		gShaderFile.open(geometryPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, gShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		gShaderStream << gShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		gShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		geometryCode = gShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char *vShaderCode = vertexCode.c_str();
	const char *gShaderCode = geometryCode.c_str();
	const char *fShaderCode = fragmentCode.c_str();
	// compile shaders

	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	if (!checkCompileErrors(vertex, "VERTEX"));// exit(-1);
	// geometry shader
	geometry = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry, 1, &gShaderCode, NULL);
	glCompileShader(geometry);
	if (!checkCompileErrors(geometry, "GEOMETRY")) exit(-4);
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	if (!checkCompileErrors(fragment, "FRAGMENT"));// exit(-2);
	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, geometry);
	glAttachShader(ID, fragment);
}


Shader::~Shader(){
	glDetachShader(ID, vertex);
	glDetachShader(ID, fragment);
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteProgram(ID);
}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::stop() {
	glUseProgram(0);
}

bool Shader::checkCompileErrors(unsigned int shader, std::string type) {
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			return false;
		}
		return true;
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			return false;
		}
		return true;
	}
}

void Shader::bindAttribute(int attribute, char varName[]) {
	glBindAttribLocation(ID, attribute, varName);
}

void Shader::loadMatrix(unsigned int location, Matrix4f *mat) {
	const GLfloat input[16] = {mat->matrix[0][0], mat->matrix[0][1], mat->matrix[0][2], mat->matrix[0][3], 
								mat->matrix[1][0], mat->matrix[1][1], mat->matrix[1][2], mat->matrix[1][3], 
								mat->matrix[2][0], mat->matrix[2][1], mat->matrix[2][2], mat->matrix[2][3],
								mat->matrix[3][0], mat->matrix[3][1], mat->matrix[3][2], mat->matrix[3][3]};
	glUniformMatrix4fv(location, 1, false, input);
}

void Shader::loadVector(unsigned int location, Vector3f *vec) {
	glUniform3f(location, vec->x, vec->y, vec->z);
}

void Shader::loadVector(unsigned int location, Vector4f *vec) {
	glUniform4f(location, vec->x, vec->y, vec->z, vec->w);
}

void Shader::loadBoolean(unsigned int location, bool boolean) {
	glUniform1i(location, boolean == true ? 1 : 0);
}

void Shader::loadInt(unsigned int location, int integer) {
	glUniform1i(location, integer);
}

void Shader::loadFloat(unsigned int location, float f) {
	glUniform1f(location, f);
}

void Shader::loadDouble(unsigned int location, double d) {
	glUniform1d(location, d);
}

unsigned int Shader::getUniformLocation(char name[]) {
	return glGetUniformLocation(ID, name);
}