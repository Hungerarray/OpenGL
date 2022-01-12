#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <string_view>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
	public:
		// the program ID
		uint32_t ID;

		// Constructor reads and builds the shader
		Shader(std::filesystem::path vertexPath, std::filesystem::path fragmentPath);
		// use\activate the shader
		void Use();
		// utility uniform functions
		void setBool(std::string_view name, bool value) const;
		void setInt(std::string_view name, int value) const;
		void setFloat(std::string_view name, float value) const;
		void setVec3(std::string_view name, float x, float y, float z) const {
			int32_t uniLocation = glGetUniformLocation(ID, name.data());
			glUniform3f(uniLocation, x, y, z);
		}
		void setVec3(std::string_view name, int x, int y, int z) const {
			int32_t uniLocation = glGetUniformLocation(ID, name.data());
			glUniform3f(uniLocation, x, y, z);
		}
		void setVec3(std::string_view name, glm::vec3 vec) const {
			int32_t uniLocation = glGetUniformLocation(ID, name.data());
			glUniform3fv(uniLocation, 1, glm::value_ptr(vec));
		}
		void setMat4(std::string_view name, glm::mat4 matrix) const {
			int32_t uniLocation = glGetUniformLocation(ID, name.data());
			glUniformMatrix4fv(uniLocation, 1, GL_FALSE, glm::value_ptr(matrix));
		}
};

inline Shader::Shader(std::filesystem::path vertexPath, std::filesystem::path fragmentPath) {
	//1. retrieve the vertex/fragment source code form filepath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// open files
	vShaderFile.open(vertexPath);
	if(!vShaderFile.is_open()) {
		std::cerr << "ERROR::Failed to open vertex shader file" << std::endl;
	}
	fShaderFile.open(fragmentPath);
	if(!fShaderFile.is_open()) {
		std::cerr << "ERROR::Failed to open fragment shader file" << std::endl;
	}
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

	const char *vShaderCode = vertexCode.c_str();
	const char *fShaderCode = fragmentCode.c_str();


	// 2. compile shaders
	uint32_t vertex, fragment;
	int success;
	char infoLog[512];

	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);
	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	// fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);
	// print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	// shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	// print linking errors if any
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(ID, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
			infoLog << std::endl;
	}

	// delete shaders
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

inline void Shader::Use() {
	glUseProgram(ID);
}

inline void Shader::setBool(std::string_view name, bool value) const {
	int32_t location = glGetUniformLocation(ID, name.data());
	glUniform1i(location, (int)value);
}

inline void Shader::setInt(std::string_view name, int value) const {
	int32_t location = glGetUniformLocation(ID, name.data());
	glUniform1i(location, value);
}

inline void Shader::setFloat(std::string_view name, float value) const {
	int32_t location = glGetUniformLocation(ID, name.data());
	glUniform1f(location, value);
}
#endif
