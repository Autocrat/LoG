#ifndef SHIP_H
#define SHIP_H

#include "GLHeader.h"
#include <box2d/box2d.h>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <vector>


const unsigned int SHIP_FORWARD= 0x1;
const unsigned int SHIP_CCW    = 0x2;
const unsigned int SHIP_CW     = 0x4;
const unsigned int SHIP_SHOOT  = 0x8;

class Ship
{
    struct TempStruct
    {
        GLfloat vertices[4];
        GLfloat colors[4];
    };

public:
    //Default constructor
  Ship(b2World& world,const glm::mat4& projMat);
    ~Ship();

    //The function that actually puts the object on the screen
    void Draw();

    //called every frame before drawing to do whatever was commanded
    //in ProcessInput()
    void DoCommands();
    
    //turn a bit mapped field into a set of commands to be handled
    //during DoCommands
    void ProcessInput(unsigned int commands);
    void stop();
private:
    const glm::mat4& m_projMat;
    glm::vec3 m_rotateAxis;

    //only using 1 vertex buffer for this object
    GLuint m_vertexBufferHandleArray[1];    
    GLuint m_colorBufferHandleArray[1];    

    static GLuint m_shaderProgram;
    GLuint m_indexBuffer;
    GLuint m_numIndices;
    GLfloat m_angle;

    //box2d parameters

    //reference to the world to use for Box
    b2World& m_worldRef;
    
    //should destroy this body in the destructor
    b2Body *m_pBody;

    //ship creation stuff
    int m_index;
    static int index;


    //Flags to indicate button states
    bool m_bForceCCW; //left
    bool m_bForceCW; //right
    bool m_bForceForward; // Forward thrust
};

#endif //SHIP_H
