#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP


#include "../include/glad/glad.h"
#include <SDL2/SDL.h>
#include <fstream>
#include <iostream>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
class ShaderProgram
{

    public:
        
        // program object for our shaders
        // has control of our vertex shader and fragment shader
        // to define how our vertices and triangles are used

        GLuint GraphicsPipelineShaderProgram = 0;
        
        // ---------- HELPERS TO LOAD SHADER FILES, COMPILE SHADERS AND THEN ATTACH AND CREATE THEM TO OUR GRAPHICS PIPELINE PROGRAM -----------
        std::string LoadShaderAsString(const std::string& filename)
        {
            std::string res = "";

            std::string line = "";
            std::ifstream myFile(filename.c_str());

            if(myFile.is_open())
            {
                while(std::getline(myFile, line))
                {
                    res+=line + '\n';
                }
                myFile.close();
            }
            return res;
        }

        GLuint CompileShader(GLuint type, const std::string &sourceCode)
        {
            GLuint shaderObject = 0;
            if(type == GL_VERTEX_SHADER)
            {
                shaderObject = glCreateShader(GL_VERTEX_SHADER);
            }
            else if(type == GL_FRAGMENT_SHADER)
            {
                shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
            }

            if (shaderObject == 0)
            {
                return 0;
            }

            const char* src = sourceCode.c_str();
            glShaderSource(shaderObject, 1, &src, nullptr);
            glCompileShader(shaderObject);

            GLint compileOk = GL_FALSE;
            glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &compileOk);
            if (compileOk != GL_TRUE)
            {
                GLint logLength = 0;
                glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &logLength);
                if (logLength > 1)
                {
                    std::string log(static_cast<size_t>(logLength), '\0');
                    glGetShaderInfoLog(shaderObject, logLength, nullptr, log.data());
                    std::cout << "Shader compilation failed: " << log << std::endl;
                }
                glDeleteShader(shaderObject);
                return 0;
            }

            return shaderObject;
        }
        // takes in vertex and fragment shader source codes

        GLuint CreateShaderProgram(const std::string &vs, const std::string &fs)
        {
            GLuint programObject = glCreateProgram();

            GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, vs);
            GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fs);

            if (myVertexShader == 0 || myFragmentShader == 0)
            {
                if (myVertexShader != 0)
                {
                    glDeleteShader(myVertexShader);
                }
                if (myFragmentShader != 0)
                {
                    glDeleteShader(myFragmentShader);
                }
                glDeleteProgram(programObject);
                return 0;
            }

            // linking our two shader programs(vertex and fragment) into one executable
            // file, for our graphicsPipelne
            glAttachShader(programObject, myVertexShader);
            glAttachShader(programObject, myFragmentShader);
            glLinkProgram(programObject);

            GLint linkOk = GL_FALSE;
            glGetProgramiv(programObject, GL_LINK_STATUS, &linkOk);
            if (linkOk != GL_TRUE)
            {
                GLint logLength = 0;
                glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &logLength);
                if (logLength > 1)
                {
                    std::string log(static_cast<size_t>(logLength), '\0');
                    glGetProgramInfoLog(programObject, logLength, nullptr, log.data());
                    std::cout << "Shader program link failed: " << log << std::endl;
                }
                glDeleteShader(myVertexShader);
                glDeleteShader(myFragmentShader);
                glDeleteProgram(programObject);
                return 0;
            }

            // validate our progam
            glValidateProgram(programObject);
            //detach shader

            glDeleteShader(myVertexShader);
            glDeleteShader(myFragmentShader);
            return programObject;
        }

        //------- HELPERS END HERE ----------
    


        // constructor should read the src code for the shaders and then compile them, attaching it to our 
        // graphicspipleline shaderID;
        ShaderProgram(const std::string &vertPath, const std::string &fragPath)
        {
            std::string vertexShaderSrc     = LoadShaderAsString(vertPath);
            std::string fragmentShaderSrc   = LoadShaderAsString(fragPath);
            GraphicsPipelineShaderProgram = CreateShaderProgram(vertexShaderSrc, fragmentShaderSrc);
        }


        // --- METHODS FOR VERTEX AND MATRIX STUFF ---


        // activate the shader
        // ------------------------------------------------------------------------
        void use() const
        { 
            glUseProgram(GraphicsPipelineShaderProgram); 
        }
        // utility uniform functions
        // ------------------------------------------------------------------------
        void setBool(const std::string &name, bool value) const
        {         
            glUniform1i(glGetUniformLocation(GraphicsPipelineShaderProgram, name.c_str()), (int)value); 
        }
        // ------------------------------------------------------------------------
        void setInt(const std::string &name, int value) const
        { 
            glUniform1i(glGetUniformLocation(GraphicsPipelineShaderProgram, name.c_str()), value); 
        }
        // ------------------------------------------------------------------------
        void setFloat(const std::string &name, float value) const
        { 
            glUniform1f(glGetUniformLocation(GraphicsPipelineShaderProgram, name.c_str()), value); 
        }
        // ------------------------------------------------------------------------
        void setVec2(const std::string &name, const glm::vec2 &value) const
        { 
            glUniform2fv(glGetUniformLocation(GraphicsPipelineShaderProgram, name.c_str()), 1, &value[0]); 
        }
        void setVec2(const std::string &name, float x, float y) const
        { 
            glUniform2f(glGetUniformLocation(GraphicsPipelineShaderProgram, name.c_str()), x, y); 
        }
        // ------------------------------------------------------------------------
        void setVec3(const std::string &name, const glm::vec3 &value) const
        { 
            glUniform3fv(glGetUniformLocation(GraphicsPipelineShaderProgram, name.c_str()), 1, &value[0]); 
        }
        void setVec3(const std::string &name, float x, float y, float z) const
        { 
            glUniform3f(glGetUniformLocation(GraphicsPipelineShaderProgram, name.c_str()), x, y, z); 
        }
        // ------------------------------------------------------------------------
        void setVec4(const std::string &name, const glm::vec4 &value) const
        { 
            glUniform4fv(glGetUniformLocation(GraphicsPipelineShaderProgram, name.c_str()), 1, &value[0]); 
        }
        void setVec4(const std::string &name, float x, float y, float z, float w) const
        { 
            glUniform4f(glGetUniformLocation(GraphicsPipelineShaderProgram, name.c_str()), x, y, z, w); 
        }
        // ------------------------------------------------------------------------
        void setMat2(const std::string &name, const glm::mat2 &mat) const
        {
            glUniformMatrix2fv(glGetUniformLocation(GraphicsPipelineShaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        // ------------------------------------------------------------------------
        void setMat3(const std::string &name, const glm::mat3 &mat) const
        {
            glUniformMatrix3fv(glGetUniformLocation(GraphicsPipelineShaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        // ------------------------------------------------------------------------
        void setMat4(const std::string &name, const glm::mat4 &mat) const
        {
            glUniformMatrix4fv(glGetUniformLocation(GraphicsPipelineShaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }

};

#endif

