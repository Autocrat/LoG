#include <algorithm>
#include <iostream>

#include "Ship.h"
#include "ShaderUtil.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

//Setup statics
int Ship::index = 0;
GLuint Ship::m_shaderProgram = 0;

Ship::Ship(b2World& world,const glm::mat4& projMat):
  m_projMat(projMat),
  m_rotateAxis(glm::vec3(0.0f,0.0f,1.0f)),
  m_angle(0.0f),
  m_worldRef(world),
  m_index(index++),
  m_bForceCCW(false),
  m_bForceCW(false),
  m_bForceForward(false)
{
    //calculate a start position
    float startX = 0.0f;
    float startY = 0.0f;

    //Set up the object for box2d
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(startX,startY); //trying to start at the center
    m_pBody = m_worldRef.CreateBody(&bodyDef);
    b2PolygonShape shipShape;

    //define array of vertices for the ship shape
    b2Vec2 boxShipVertices[3]; 

    boxShipVertices[0].x = 0.0f;
    boxShipVertices[0].y = 0.5f;

    boxShipVertices[1].x = -0.4f;
    boxShipVertices[1].y = -0.5f;

    boxShipVertices[2].x = 0.0f;
    boxShipVertices[2].y = 0.0f;

    for(int i=0; i<3; ++i)
	{
	    boxShipVertices[i].x*=2.0;
	    boxShipVertices[i].y*=2.0;
	}
    
    shipShape.Set(boxShipVertices, 3);
    
    //Box only creates convex polygons for shapes,
    //so 2 triangles to create the shape we want
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shipShape;
    fixtureDef.density = 0.5f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.5f;
    m_pBody->CreateFixture(&fixtureDef);

    
    boxShipVertices[1].x = 0.4f;
    boxShipVertices[1].y = -0.5f;
	    boxShipVertices[1].x*=2.0;
	    boxShipVertices[1].y*=2.0;


    shipShape.Set(boxShipVertices, 3);
    m_pBody->CreateFixture(&fixtureDef);

//Base the color of this ship on the index
    GLfloat blue = 0.0f;
    GLfloat red = 0.0f;
    GLfloat green = 0.0f;

    switch(m_index)
    {
    case 0:
    {
        blue = 1.0f;
        green = 0.0f;
        red = 0.7f;
        break;
    }
    case 1:
    {
        red = 1.0f;
        break;
    }
    case 2:
    {
        green = 1.0f;
        break;
    }
    case 3:
    {
        blue = 1.0f;
        break;
    }
    default:
    {
        std::cerr << "Too many ships." << std::endl;
        break;
        exit(1);
    }
    }
    //Vertices definitions for use by OpenGL
    //These are separate from Box coordinates, but 
    //they completely line up with all of the external
    //edges
    TempStruct shipVertices[] =
        {
            {{ boxShipVertices[0].x,boxShipVertices[0].y, 0.0f,1.0f}, //0
             {1.0, 1.0, 1.0, 1.0}},
            {{-0.8f, -1.0f, 0.0f, 1.0f}, //1  //overwrote this one in the array, so use it manually
             {red, green, blue, 1.0}},
            {{boxShipVertices[2].x,boxShipVertices[2].y, 0.0f,1.0f}, //2
             {red, green, blue, 1.0}},
            {{boxShipVertices[1].x,boxShipVertices[1].y, 0.0f,1.0f}, //3
             {red, green, blue, 1.0}}
        };

    if(!m_shaderProgram)
    {
        //TBD try/catch
        //Load and compile the shaders
        ShaderList_T shaderList;
        shaderList.push_back(createShaderFromFile(GL_VERTEX_SHADER, "ShipVertex.glsl"));
        shaderList.push_back(createShaderFromFile(GL_FRAGMENT_SHADER, "ShipFragment.glsl"));
        
        m_shaderProgram = createShaderProgram(shaderList);
        //done with the shaders, so delete them
        //do this here instead of in createShaderProgram so that a shader
        //can be reused without rebuilding it
        std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
    }
    //Setup the vertex buffers.  Should this be its own function?
    //glGenBuffers(Number of buffers to allocate, array to put buffer handlers into);
    glGenBuffers(1, m_vertexBufferHandleArray);
    //select the current buffer to have opengl use
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandleArray[0]);
    //copy the data into it
    glBufferData(GL_ARRAY_BUFFER, sizeof(shipVertices), shipVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //array to give the order of the vertices to draw
    GLubyte tmpIndices[] =
    {
        //front
        0,1,2,
        2,3,0

    };
    
    //store the number of indices in the index buffer
    //this only works because they're GLubytes
    m_numIndices = sizeof(tmpIndices);
    glGenBuffers(1, &m_indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndices, tmpIndices, GL_STATIC_DRAW);

    //release the buffers
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//Main drawing function for this Ship
void Ship::Draw()
{
     glUseProgram(m_shaderProgram);

     //get a handle for the rotation uniform;
     GLuint locationMVP = glGetUniformLocation(m_shaderProgram,"mvpMatrix");
     glm::mat4 modelMatrix(1.0f);

     //glm translation vector
     b2Vec2 position = m_pBody->GetWorldCenter();
     //Angles in Radians
     float angleDeg = m_pBody->GetAngle();

     //Translate then Rotate
     modelMatrix = 
         glm::translate(modelMatrix, glm::vec3(position.x, position.y, 0.0f)); 
     modelMatrix = 
         glm::rotate(modelMatrix, angleDeg , m_rotateAxis);    

     //skipping the view matrix, because we don't need it
     glm::mat4 mvpMat = m_projMat * modelMatrix;
     
     glUniformMatrix4fv(locationMVP, 1, GL_FALSE, glm::value_ptr(mvpMat));
    //get a handle to the vPosition attribute of the shader
    //this can/should be done right after linking the shader and
    //stored in a member variable
    //Should probably do that with all the shader program handles
    GLuint vertexAttribute = glGetAttribLocation(m_shaderProgram,"vPosition");
    //bind the buffer that we're going to get the attributes from
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandleArray[0]);

    glEnableVertexAttribArray(vertexAttribute);
    glVertexAttribPointer(vertexAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(TempStruct), 0);

 
    GLuint colorAttribute = glGetAttribLocation(m_shaderProgram, "vColor");
    glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(TempStruct), (void*)(sizeof(GLfloat)*4));
    glEnableVertexAttribArray(colorAttribute);

   //must bind this before glDrawElements
    //or give glDrawElements an array of indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    
    glDrawElements(GL_TRIANGLES,m_numIndices,GL_UNSIGNED_BYTE,0);

    //Cleanup
    glDisableVertexAttribArray(vertexAttribute);
    glDisableVertexAttribArray(colorAttribute);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);
}


//Destructor
Ship::~Ship()
{
    m_worldRef.DestroyBody(m_pBody);
}

void Ship::stop()
{
    m_pBody->SetLinearVelocity(b2Vec2(0.0f,0.0f));
}


void Ship::ProcessInput(unsigned int commands)
{
  //For now, use the constants from CWIID
  m_bForceCCW = (commands & SHIP_CCW);
  m_bForceCW = (commands & SHIP_CW);
  m_bForceForward = (commands & SHIP_FORWARD);
    
    if(commands & SHIP_SHOOT)
    {
      //shoot ?
    }
}



void Ship::DoCommands()
{
    
    b2Transform xf = m_pBody->GetTransform();
    if(m_bForceForward)
    {
        xf.p.SetZero();
        b2Vec2 force = b2Mul(xf,b2Vec2(0.0f, 15.0f));
        m_pBody->ApplyForceToCenter(force,true);
    }

    if(m_bForceCW)
    {
        m_pBody->ApplyTorque(-1.0f, true);
    }
    else if(m_bForceCCW)
    {
        m_pBody->ApplyTorque(1.0f, true);
    }

}
