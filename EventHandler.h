#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H



class SDL_Window;
class b2World;

//Interface to receive events from some kind of controller object.
class EventHandler
{
private:
  bool m_bQuit;
  SDL_Window* m_pWindow;

  //box2d  stuff
  b2World& m_physicsWorld;
  float m_timeStep;
  int m_velocityIterations;
  int m_positionIterations;
public:
  explicit EventHandler(SDL_Window *pWindow, b2World& physicsWorld);

//Main Event Loop
    void EventLoop();

//Destructor
    virtual ~EventHandler();
    
    
};



#endif
