#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
	unsigned int ID;


	Shader(const char* vertPath, const char* fragtPath, const char* geomPath)
	{
		//getting source files
		std::string vertCode;
		std::string fragCode;
		std::string geomCode;
		std::ifstream vsFile;
		std::ifstream fsFile;
		std::ifstream gsFile;
		//error handling
		vsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			//open files
			vsFile.open(vertPath);
			fsFile.open(fragtPath);
			gsFile.open(geomPath);
			std::stringstream vsStream, fsStream, gsStream;

			//read into streams
			vsStream << vsFile.rdbuf();
			fsStream << fsFile.rdbuf();
			gsStream << gsFile.rdbuf();
			//close files
			vsFile.close();
			fsFile.close();
			gsFile.close();

			//Convert stream into string
			vertCode = vsStream.str();
			fragCode = fsStream.str();
			geomCode = gsStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertCode.c_str();
		const char* fShaderCode = fragCode.c_str();
		const char* gShaderCode = geomCode.c_str();

		//compile shaders
		unsigned int vertex, fragment, geometry;
		int success;
		char infoLog[512];
		//vertex Shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, nullptr);
		glCompileShader(vertex);

		//print errors if need
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
				infoLog << std::endl;
		};



		//same for fs
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, nullptr);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
				infoLog << std::endl;
		};


		//and geom shader
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, nullptr);
		glCompileShader(geometry);

		glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(geometry, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" <<
				infoLog << std::endl;
		};

		//shader Program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, geometry);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);


		//linking errors
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
				infoLog << std::endl;
		}


		//delete all shaders
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		glDeleteShader(geometry);
	}
	
	void use() {
		glUseProgram(ID);
	}
	

	//uniform methods for convinience
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}

};
#endif
