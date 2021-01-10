#ifndef BLOCK_H
#define BLOCK_H

#include "GLHeader.h"
#include <glm/gtc/matrix_transform.hpp> //glm::mat4 and more


class b2World;
class b2Body;

class Block
{
 public:
  Block(b2World& physicsWorld,
	const glm::mat4& projection,
	const int leftX,
	const int topY,
	const int rightX,
	const int botY,
	const unsigned char r,const unsigned char g, const unsigned char b);

  bool IsExtension(const int row) const ;
  void Extend();
  void Draw();
  void SetupGraphics();
  void SetupPhysicsInfo();
  ~Block();


private:
  void SetupShader();

  int m_startX;
  int m_stopX;
  int m_startY;
  int m_stopY;

  b2World& m_physicsWorld;
  const glm::mat4& m_projection;
  
  GLuint m_shaderProgram;

  GLuint m_VBO;
  GLuint m_EBO;

  GLuint m_aPos;
  GLuint m_mvp;

  b2Body* m_pBody;
};

#endif
