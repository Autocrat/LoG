#include "Block.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <Box2D/Box2D.h>


const char *vertexShaderSource = // "#version 330 core\n"
  "attribute vec3 aPos;\n"
  "uniform mat4 mvp;\n"
  "void main()\n"
  "{\n"
  "   gl_Position = mvp * vec4(aPos.x + mvp.x, aPos.y, aPos.z, 1.0);\n"
  "}\0";

const char *fragmentShaderSource = // "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main()\n"
  "{\n"
  "   gl_FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
  "}\n\0";



Block::Block(b2World& physicsWorld,
	     const glm::mat4& projection,
	     const int leftX,
	     const int topY,
	     const int rightX,
	     const int botY,
	     const unsigned blockIndex):
  m_physicsWorld(physicsWorld),
  m_projection(projection),
  m_startX(leftX),
  m_startY(topY),
  m_stopX(rightX),
  m_stopY(botY),
  m_pBody(0),
  m_index(blockIndex)
{
  SetupShader();
}


void Block::Draw()
{
  glUseProgram(m_shaderProgram);
  //Set up the attribute array for coordinates
  glEnableVertexAttribArray(m_aPos);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glVertexAttribPointer(
			m_aPos, // attribute
			3,                 // number of elements per vertex, here (x,y,z)
			GL_FLOAT,          // the type of each element
			GL_FALSE,          // take our values as-is
			0,                 // no extra data between each position
			0                  // offset of first element
			);

  //pass the transform matrix into the shader
  glUniformMatrix4fv(m_mvp, 1, GL_FALSE, glm::value_ptr(m_projection));
  
  
  // Push each element in buffer_vertices to the vertex shader
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  //TBD don't think this is necessary
  int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
  glDrawElements(GL_TRIANGLE_STRIP, 4 /* number of indices */, GL_UNSIGNED_INT, 0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glUseProgram(0);
}

void Block::SetupShader()
{
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
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
    glLinkProgram(m_shaderProgram);
    // check for linking errors
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    
    //bind attributes/uniforms
    
    m_aPos = glGetAttribLocation(m_shaderProgram, "aPos");
    if (m_aPos == -1) {
      std::cerr << "Could not find attribute " << "aPos" << std::endl;
      return;
    }
    
        
    m_mvp = glGetUniformLocation(m_shaderProgram, "mvp");
    if (m_mvp == -1) {
      std::cerr << "Could not find uniform " << "mvp" << std::endl;
      return;
    }
    

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}


Block::~Block()
{
  glDeleteBuffers(1, &m_VBO);
  glDeleteBuffers(1, &m_EBO);
  glDeleteProgram(m_shaderProgram);
}


//Check to see if the passed in 1 row is 1 after the last row in this block
bool Block::IsExtension(const int row) const
{
  return ((row) == m_stopY);
}

//Stretch the attributes down 1 row
void Block::Extend()
{
  ++m_stopY;
}

void Block::SetupGraphics()
{
  
  float leftX = -96.0 + static_cast<double>(m_startX);
  float rightX = -96.0 + static_cast<double>(m_stopX);
  float topY = 54.0 - static_cast<double>(m_startY);
  float botY = 54.0 - static_cast<double>(m_stopY);	    
  
  float vertices[] = {
		      rightX, topY, 0.0f,  // top right
		      rightX, botY, 0.0f,  // bottom right
		      leftX, botY, 0.0f,  // bottom left
		      leftX, topY, 0.0f   // top left 
  };
  

  unsigned int indices[] = { 3, 0, 2,  // first Triangle
			     1   // second Triangle
  };

  glGenBuffers(1, &m_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
  glGenBuffers(1, &m_EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0); 
  
}


void Block::SetupPhysics()
{
      //Set up the object for Box2D
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    //    bodyDef.position.Set(startX,startY); //trying to start at the center
    m_pBody = m_physicsWorld.CreateBody(&bodyDef);
    b2PolygonShape blockShape;

    //define array of vertices for the shape
    b2Vec2 boxVertices[4]; 

    float leftX = -96.0 + static_cast<double>(m_startX);
    float rightX = -96.0 + static_cast<double>(m_stopX);
    float topY = 54.0 - static_cast<double>(m_startY);
    float botY = 54.0 - static_cast<double>(m_stopY);

    boxVertices[0].x = leftX;
    boxVertices[0].y = topY;

    boxVertices[1].x = rightX;
    boxVertices[1].y = topY;

    boxVertices[2].x = rightX;
    boxVertices[2].y = botY;
    
    boxVertices[3].x = leftX;
    boxVertices[3].y = botY;

    blockShape.Set(boxVertices, 4);
    
    //Box only creates convex polygons for shapes,
    //so 2 triangles to create the shape we want
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &blockShape;
    m_pBody->CreateFixture(&fixtureDef);

}
