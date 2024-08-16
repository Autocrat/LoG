#include "GLHeader.h"
#include "EventHandler.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <box2d/box2d.h>

#include "Room.h"
#include "Ship.h"
#include "Shaders.h"
#include "NodeData.h"

EventHandler::EventHandler(SDL_Window *pWindow, b2World& physicsWorld):
    m_bQuit(false),
    m_pWindow(pWindow),
    m_physicsWorld(physicsWorld),
    m_timeStep(1.0f / 60.0f),
    m_velocityIterations(6),
    m_positionIterations(2)
{

}

//Main Event Loop
void EventHandler::EventLoop()
{

    NodeData nodeData;
    Shaders shaders;
  
  //TBD - This should probably be either passed in or created dynamically
  //Rooms are 192x108 grid
  Room room(m_physicsWorld,
	    glm::ortho(-96.0,95.0,-54.0,53.0),
	    shaders,
	    nodeData,
	    "grid.bmp");

  Ship myShip(m_physicsWorld,
	      glm::ortho(-96.0,95.0,-54.0,53.0));
  unsigned int command = 0;
  bool bShipCommand = false;
  SDL_Event event;
  while(!m_bQuit)
    {
      bShipCommand = false;
      if(SDL_PollEvent(&event))
        {          
	  switch(event.type)
            {
	    case SDL_QUIT:
	      m_bQuit=true;
	      break;
	    case SDL_KEYDOWN:
	      {
		bShipCommand = true;
		switch(event.key.keysym.scancode)
		  {
		  case SDL_SCANCODE_X:
		    m_bQuit=true;
		    break;
		  case SDL_SCANCODE_A: //left
		    command |= SHIP_CCW;
		    break;
		  case SDL_SCANCODE_D: //right
		    command |= SHIP_CW;
		    break;
		  case SDL_SCANCODE_W: //up
		    command |= SHIP_FORWARD;
		    break;
		  case SDL_SCANCODE_S: //shoot
		    //		    command |= SHIP_SHOOT;
		    break;
		  }
		break;
	      }
	      
	    case SDL_KEYUP:
	      {
		bShipCommand = true;
		switch(event.key.keysym.scancode)
		  {
		  case SDL_SCANCODE_A: //left
		    command &= ~SHIP_CCW;
		    break;
		  case SDL_SCANCODE_D: //right
		    command &= ~SHIP_CW;
		    break;
		  case SDL_SCANCODE_W: //up
		    command &= ~SHIP_FORWARD;
		    break;
		  case SDL_SCANCODE_S: //shoot
		    //		    command &= ~SHIP_SHOOT;
		    break;
		  }
		break;
	      }
	    default:
                    break;
            }
        }


      if(bShipCommand)
	{
	  myShip.ProcessInput(command);
	}
      
      m_physicsWorld.Step(m_timeStep, m_velocityIterations, m_positionIterations);
      
      //clear the screen
      glClear( GL_COLOR_BUFFER_BIT );
      room.Draw();
      myShip.DoCommands();
      myShip.Draw();
      //Display the back buffer
      SDL_GL_SwapWindow(m_pWindow);
      
      //Sleep to not eat the machine
      SDL_Delay(60);
    }
    
}

//Destructor
EventHandler::~EventHandler()
{
};
    

