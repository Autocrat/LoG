#ifndef SHADERS_H
#define SHADERS_H

#include "GLHeader.h"
#include <map>
#include <string>

class Shaders
{
 public:
  Shaders();
  ~Shaders();

  GLuint getShader(const std::string& shaderName) const;
 private:
  bool compileShader(const char* vertexShaderSource,
                     const char* fragmentShaderSource,
                     const char* shaderName);
  
  void BlockShader();
  void PortalShader();


  

  std::map<std::string, GLuint> m_shaderPrograms;
};

#endif
