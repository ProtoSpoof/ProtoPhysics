#pragma once

#define GLEW_STATIC
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader {
    private:
    unsigned int mRendererID;
    unsigned int getUniformLocation(const std::string& name);
    unsigned int createShader(const std::string vertexShaderFilePath, const std::string fragmentShaderFilePath);
    unsigned int compileShader(unsigned int type, const std::string& source);
    std::string readShaderFile(std::string filepath);
    
    public:
    Shader(const std::string& vertexShader,const std::string& fragmentShader);
    ~Shader();

    void bind() const;
    void unbind() const;
    void setUniform4f(const std::string name, float v0, float v1, float v2, float v3);
    void setUniformMat4f(const std::string name, const glm::mat4& matrix);

};

Shader::Shader(const std::string& vertexShaderFilePath,const std::string& fragmentShaderFilePath) {
    std::string vertexShader;
    std::string fragmentShader;

    try {
        vertexShader = readShaderFile(vertexShaderFilePath);
        fragmentShader = readShaderFile(fragmentShaderFilePath);
    } catch (std::string e) {
        std::cout << e;
    }

    mRendererID = createShader(vertexShader, fragmentShader);
} 

Shader::~Shader() {}

unsigned int Shader::getUniformLocation(const std::string& name) {
    int location = glGetUniformLocation(mRendererID, name.c_str());
    
    if (location == -1) std::cout << "Warning: uniform '" << name << "' does not exist!" << std::endl;

    return location;
}

void Shader::setUniform4f(const std::string name, float v0, float v1, float v2, float v3){
	glUniform4f(getUniformLocation(name),v0,v1,v2,v3);
}

void Shader::setUniformMat4f(const std::string name, const glm::mat4& matrix){
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}





void Shader::bind() const {
	glUseProgram(mRendererID);
}

void Shader::unbind() const {
	glUseProgram(0);

}

unsigned int Shader::createShader(const std::string vertexShader, const std::string fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
};


unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char *src = source.c_str();

	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;

	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char *message = (char *)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}


std::string Shader::readShaderFile(std::string filepath) {
	char curChar;
	std::string shaderSource;
	FILE *file = fopen(filepath.c_str(), "r");

	if (file == NULL) throw("ERROR: File \"" + filepath + "\" does not exist\n");

	while ((curChar = fgetc(file)) != EOF) {
		shaderSource.push_back(curChar);
	}

	std::cout << "SOURCE: \n" << shaderSource << std::endl;

	return shaderSource;
}
