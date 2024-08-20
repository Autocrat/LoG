#ifndef BLOCK_H
#define BLOCK_H

#include "BlockInterface.h"
#include "GLHeader.h"
#include <glm/gtc/matrix_transform.hpp> //glm::mat4 and more


class b2World;
class b2Body;
class Shaders;
class NodeInfo;

class Block : public BlockInterface
{
 public:
  Block(b2World& physicsWorld,
	const glm::mat4& projection,
	const Shaders& shaders,
	const NodeInfo& nodeInfo,
	const int leftX,
	const int topY,
	const int rightX,
	const int botY,
	const unsigned int blockIndex);

  bool IsExtension(const int row) const ;
  void Extend();
  void Draw();
  void Finalize();
  virtual ~Block();


private:
  void SetupShader();
  void SetupGraphics();
  void SetupPhysics();

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

  GLuint m_color;
  b2Body* m_pBody;
  const GLfloat m_red;
  const GLfloat m_blue;
  const GLfloat m_green;

  unsigned int m_index;
};

#endif
