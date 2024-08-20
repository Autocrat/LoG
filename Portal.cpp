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
	blockIndex)  
{
}

Portal::~Portal()
{
}


