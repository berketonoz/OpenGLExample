#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Shader {
public:
    GLuint Program;
    //Constructor
    Shader(const GLchar *vertexPath, const GLchar *fragmentPath){
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        //ensures ifstream objects can throw exceptions
        vShaderFile.exceptions(std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::badbit);
        try{
            //Open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            //Read file's buffer
            vShaderStream << vShaderFile.rdbuf( );
            fShaderStream << fShaderFile.rdbuf( );
            //Clode file handlers
            vShaderFile.close();
            fShaderFile.close();
            //Convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }catch(std::ifstream::failure e){
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const GLchar *vShaderCode = vertexCode.c_str();
        const GLchar *fShaderCode = fragmentCode.c_str();
        //2. Compile shaders
        GLuint vertex, fragment;
        GLint success;
        GLchar infoLog[512];
        //Vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        //Print compile errors
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        //Fragment shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1 , &fShaderCode, NULL);
        glCompileShader(fragment);
        //Print errors
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        //Shader program
        this->Program = glCreateProgram();
        glAttachShader(this->Program, vertex);
        glAttachShader(this->Program, fragment);
        glLinkProgram(this->Program);
        //Print linking errors if any
        glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
        if(!success){
            glGetProgramInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        //Delete the shaders as they're linked
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    void use(){
        glUseProgram(this->Program);
    }
};

#endif
