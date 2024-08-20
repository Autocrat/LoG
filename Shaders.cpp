#include "Shaders.h"
#include <iostream>


Shaders::Shaders()
{
    const char *vertexShaderSource =  // "#version 330 core\n"
      "attribute vec3 aPos;\n"
      "uniform vec3 uColor;\n"
      "uniform mat4 mvp;\n"
      "varying vec3 vColor;\n"
      "void main()\n"
      "{\n"
      "   vColor=uColor;\n"
      "   gl_Position = mvp * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"   
      "}\0";
  
    const char *fragmentShaderSource =  // "#version 330 core\n"
      "varying mediump vec3 vColor;\n"
      "void main()\n"
      "{\n"
      "   gl_FragColor = vec4(vColor, 1.0);\n"
      "}\n\0";
  
    /********************************************************/
    //temp setup shader info
    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
// fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    m_shaderPrograms["basic"] = shaderProgram;

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

}

Shaders::~Shaders()
{
}


GLuint Shaders::getShader(const std::string& shaderName) const
{
  std::map<std::string, GLuint>::const_iterator iter = m_shaderPrograms.find(shaderName);
  if(iter == m_shaderPrograms.end())
  {
    std::cerr << "Error finding shader: " << shaderName << std::endl; 
    throw 1;
  }
  else
  {
    return iter->second;
  }

}
