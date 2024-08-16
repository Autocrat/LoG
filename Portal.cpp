#include "Portal.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <box2d/box2d.h>
#include "Shaders.h"
#include "NodeData.h"

Portal::Portal(b2World& physicsWorld,
	       const glm::mat4& projection,
	       const Shaders& shaders,
	       const NodeInfo& nodeInfo,
	       const int leftX,
	       const int topY,
	       const int rightX,
	       const int botY,
	       const unsigned int blockIndex):
  Block(physicsWorld,
	projection,
	shaders,
	nodeInfo,
	leftX,
	topY,
	rightX,
	botY,
	blockIndex),
  vertexShaderSource ( // "#version 330 core\n"
		      "attribute vec3 aPos;\n"
		      "uniform mat4 mvp;\n"
		      "void main()\n"
		      "{\n"
		      "   gl_Position = mvp * vec4(aPos.x + mvp.x, aPos.y, aPos.z, 1.0);\n"
		      "}\0"),
  fragmentShaderSource( // "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main()\n"
  "{\n"
  "   gl_FragColor = vec4(0.0f, 0.5f, 1.0f, 1.0f);\n"
  "}\n\0")
  
  
{
}

Portal::~Portal()
{
}


