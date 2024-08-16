//SDL2
#include "GLHeader.h"
#include <SDL2/SDL.h>

#include <iostream>
#include "EventHandler.h"

//Box 2D
#include <box2d/box2d.h>

//function prototype
//1920x1080 for full HD
void setupDisplay(SDL_Window *&pWindow, SDL_GLContext& context, int screenx=1024, int screeny=768);



int main(int /*argc*/, char ** /*argv*/)
{

    //World for use with box2d with no gravity
    //positive 10.0 is up
    //negative 10.0 is down
    //positive is right,
    //negative is left
    b2World World(b2Vec2(0.0f,0.0f));
  
    //variables to handle the SDL display
    SDL_Window *pWindow = 0;
    SDL_GLContext glContext;

    setupDisplay(pWindow,glContext);

    //Instantiate main event handler/control loop object
    EventHandler eventHandler(pWindow,World);

    //Run the main loop
    eventHandler.EventLoop();

    //clean up everything set up by the display
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(pWindow);
    pWindow = 0;
    SDL_Quit();

    return 0;
}




//****************************************************************
//Basic SDL2 setup of a screen for OpenGL
void setupDisplay(SDL_Window*& pWindow, SDL_GLContext& context, int screenx, int screeny)
{


    //Initialize SDL video and sound
    if ( SDL_Init ( SDL_INIT_VIDEO  ) < 0 ) {
        std::cerr << "Unable to initialize SDL " << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        return ;
    }


    
    Uint32 FLAGS = SDL_WINDOW_OPENGL;
    //    SDL_GL_SetAttribute ( SDL_GL_CONTEXT_MAJOR_VERSION , 3 ) ;
    //    SDL_GL_SetAttribute ( SDL_GL_CONTEXT_MINOR_VERSION , 0 ) ;

    SDL_GL_SetAttribute ( SDL_GL_CONTEXT_EGL , 1 ) ;
    SDL_GL_SetAttribute ( SDL_GL_CONTEXT_MAJOR_VERSION , 2 );
    SDL_GL_SetAttribute ( SDL_GL_CONTEXT_MINOR_VERSION , 0 ) ;

    
    int majorVersion;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &majorVersion);
    std::cout << "Major: " <<  majorVersion << std::endl;

    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &majorVersion);
    std::cout << "Minor: " <<  majorVersion << std::endl;
    SDL_GL_SetAttribute ( SDL_GL_DOUBLEBUFFER , 1 ) ;
    //    SDL_GL_SetAttribute ( SDL_GL_DEPTH_SIZE , 24 ) ;

    if ( ( pWindow = SDL_CreateWindow ( "Grid " ,
					SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED ,
					screenx , screeny,
					FLAGS ) ) == NULL ) {
        std::cerr << "Unable to open window" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        return ;
    }
    SDL_DisableScreenSaver();
    
    context = SDL_GL_CreateContext ( pWindow ) ;
    SDL_GL_SetSwapInterval ( 1 ) ;

#ifdef GLEW
    glewExperimental = true;
    GLenum err = glewInit () ;
    if(err != GLEW_OK)
    {
      std::cerr << "Error initializing GLEW: " << glewGetErrorString(err) << std::endl;
        return;
    }
#endif
    std::cerr << glGetString(GL_VERSION) << std::endl;


    glViewport(0,0,screenx,screeny);

    // glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_LESS);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);

    glEnable(GL_BLEND);
    //    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //clear the double buffers in hopes that 
    //it will clear EVERYTHING
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


}

