//https://youtu.be/N614k7LVuMU -wtf is this ?

#include "Room.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "ShaderUtil.h"
#include "Block.h"

Room::Room(const glm::mat4& projection, const std::string& mapFileName):
  m_projection(projection)
{
 
  /********************************************************/
  
  //Using SDL_Image to load the bmp into a SDL_Surface
  SDL_Surface* map_image_surface = IMG_Load(mapFileName.c_str());
  if (map_image_surface == NULL) {
    std::cerr << "IMG_Load: " << SDL_GetError() << std::endl;
    return;
  }

  SDL_LockSurface(map_image_surface);
  std::cout << (int)map_image_surface->format->BitsPerPixel << "   " << (int)map_image_surface->format->BytesPerPixel  << std::endl;
  std::cout << "Format enum: " <<  SDL_GetPixelFormatName(map_image_surface->format->format) << std::endl;
  if(map_image_surface->format->format != SDL_PIXELFORMAT_BGR24)
    {
      std::cerr << "Incorrect pixel format.  Need SDL_PIXELFORMAT_BGR24" << std::endl;
      return;
    }

  //Don't need this for the above pixel format
  /*
  SDL_Palette *pPalette = map_image_surface->format->palette;
  if(!pPalette)
    {
      std::cout << "Null palette" << std::endl;
    }
  */

  
  unsigned char* pixels = static_cast<unsigned char*>(map_image_surface->pixels);

  //get the red attribute of the first pixel in the row
  unsigned char rLast = pixels[2];
  int pixelRunCount = 0;
  float xPositionStart = -96.0; //TBD remove magic numbers
  float xPositionStop = -96.0;
  float yPositionStart = 54.0;
  float yPositionStop = 53.0; //TBD this will eventually be counted down

  int pitch = map_image_surface->pitch;
  int pixelNum = 0;
  for(int j = 0; j < map_image_surface->h; ++j)

    {
      int offset = j * pitch; //offset into the byte array of pixels
      rLast = pixels[offset + 2]; //use the first red byte to start
      pixelRunCount = 0;
      xPositionStart = -96.0; //TBD remove magic numbers
      xPositionStop = -96.0;
    
      for(int i = 0; i < pitch;)
	{
	  unsigned char b = pixels[offset + i++];
	  unsigned char g = pixels[offset + i++];
	  unsigned char r = pixels[offset + i++];
	  ++pixelNum;

	  //Check to see if it's still the same texture
	  if(r!=rLast || pixelNum == map_image_surface->w) //we've reached the end.  create the block object that fits this count.
	    {
	      //	  std::cout << "Finished a run of " << pixelRunCount << " " << static_cast<unsigned int>(rLast) << " pixels" << std::endl;
	      xPositionStop = xPositionStart + static_cast<double>(pixelRunCount);

	      //Block creation
	      if(rLast == 255) //Only create objects when the Red byte is 255
		{
		  Block * block = new Block(m_projection,
					    xPositionStart,
					    yPositionStart,
					    xPositionStop,
					    yPositionStop,
					    r,g,b );
		  
		  m_blocks.push_back(block);

		}
	      //END GL Code
    
	      //reset
	      if(pixelNum == map_image_surface->w) //reset when we reach the end of the row
		{
		  xPositionStart = -96.0; // TBD - remove magic numbers
		  xPositionStop = xPositionStart;
		  pixelNum = 0;
		}
	      else //reset at the end of the run
		{
		  xPositionStart = xPositionStop;
		  rLast = r;
		}
	      pixelRunCount = 1;

	    } // last in run or row
	  else
	    {
	      ++pixelRunCount;
	    }

	}
      yPositionStop--;
      yPositionStart--;
    }
  SDL_UnlockSurface(map_image_surface);    
  SDL_FreeSurface(map_image_surface);

}

Room::~Room()
{
  for(int i = 0; i < m_blocks.size(); ++i)
    {
      delete m_blocks[i];
    }
}



void Room::Draw()
{
  for(int i = 0; i < m_blocks.size(); ++i)
    {
      m_blocks[i]->Draw();
    }
}