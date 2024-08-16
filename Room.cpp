//https://youtu.be/N614k7LVuMU -wtf is this ?

#include "Room.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "ShaderUtil.h"
#include "Block.h"
#include "BlockMapKey.h"
#include "Shaders.h"
#include "NodeData.h"

Room::Room(b2World& physicsWorld,
	   const glm::mat4& projection,
	   const Shaders& shaders,
	   const NodeData& nodeData,
	   const std::string& mapFileName):
    m_physicsWorld(physicsWorld),
    m_projection(projection),
    m_shaders(shaders),
    m_nodeData(nodeData)
{
    /********************************************************/
  
    //Using SDL_Image to load the bmp into a SDL_Surface
    SDL_Surface* map_image_surface = IMG_Load(mapFileName.c_str());
    if (map_image_surface == NULL) {
	std::cerr << "IMG_Load: " << SDL_GetError() << std::endl;
	return;
    }

    //Currently, the image size should be 192x108x24bpp (1/10 of a 1920x1080 image)
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


    //Look at each row of pixels in the bitmap.
    //Use the color to determine
    // a. If this color (index) is defined in the XML
    // b. If the previous pixel is of the same color
    //   1. Contiguous rows of pixels of the same color can be used to make the the OpenGL objects
    //   2. Contiguous blocks can be used for the same thing.
    //     xxxxxxx  can be 1 OpenGL object
    //
    //     xxxxxxxx
    //     xxxxxxxx
    //     xxxxxxxx
    //              can be 1 OpenGL object
    //
    //     xxxxxxxx
    //     xxxxxxxxx
    //              will be 2 OpenGL objects
    //
    
    unsigned char* pixels = static_cast<unsigned char*>(map_image_surface->pixels);

    //Build an index based on the color of the pixel
    //This will be used to check the XML for what type of object this is
    unsigned int lastIndex = (pixels[2] << 16) | (pixels[1] << 8) | (pixels[0]);
    int pixelRunCount = 0;
    int xStart=0;
    int pitch = map_image_surface->pitch;
    int pixelNum = 0;
    for(int j = 0; j < map_image_surface->h; ++j)
    {
	int offset = j * pitch; //offset into the byte array of pixels
	lastIndex = (pixels[offset+2] << 16) | (pixels[offset+1] << 8) | (pixels[offset+0]);
	pixelRunCount = 0;
	xStart = 0;

	for(int i = 0; i < pitch;)
	{
	    unsigned char b = pixels[offset + i++];
	    unsigned char g = pixels[offset + i++];
	    unsigned char r = pixels[offset + i++];
	    ++pixelNum;
	    unsigned int blockIndex = (r << 16) | (g << 8) | (b);
	    //Check to see if it's still the same texture
	    if(blockIndex!=lastIndex || pixelNum == map_image_surface->w) //we've reached the end.  create the block object that fits this count.
	    {
		int xEnd = xStart + pixelRunCount;
		//Block creation
		HandleBlockCreation(xStart,xEnd,lastIndex,j);
   
		//reset
		if(pixelNum == map_image_surface->w) //reset when we reach the end of the row
		{
		    xStart = 0;
		    pixelNum = 0;
		}
		else //reset at the end of the run
		{
		    xStart += pixelRunCount;
		    lastIndex = blockIndex;
		}
		pixelRunCount = 1;
	      
	    } // last in run or row
	    else
	    {
		++pixelRunCount;
	    }

	}
    }
    SDL_UnlockSurface(map_image_surface);    
    SDL_FreeSurface(map_image_surface);

    BlockMap::iterator iter = m_blockMap.begin();
    for(;iter != m_blockMap.end(); ++iter)
    {
	iter->second->Finalize();
    }

}

//Destructor
Room::~Room()
{
    BlockMap::const_iterator iter = m_blockMap.begin();
    for(;iter != m_blockMap.end(); ++iter)
    {
	delete  iter->second;
    }
}


//Walk through all of the blocks in the room and put it on the screen
void Room::Draw()
{
    BlockMap::const_iterator iter = m_blockMap.begin();
    for(;iter != m_blockMap.end(); ++iter)
    {
	iter->second->Draw();
    }
}


void Room::AddBlock(const BlockMapKey& blockMapKey,
		    const int leftX,
		    const int topY,
		    const int rightX,
		    const int botY,
		    const unsigned int blockIndex)
{
    NodeInfo nodeInfo;
    m_nodeData.getNodeData(blockIndex,nodeInfo);
    Block * pBlock = new Block(m_physicsWorld,
			       m_projection,
			       m_shaders,
			       nodeInfo,
			       leftX,
			       topY,
			       rightX,
			       botY,
			       blockIndex);

    m_blockMap[blockMapKey]= pBlock;
}



//Figure out the extents of the block, see if we need to extend the existing block or create a new one
void Room::HandleBlockCreation(int xStart,
			       int xEnd,
			       unsigned int blockIndex,
			       const int j)
{
    //check to see if the block index is defined in the XML
    NodeInfo nodeInfo;
    bool nodeFound = m_nodeData.getNodeData(blockIndex,nodeInfo);
    if(nodeFound)
	//    if(blockIndex == 0x00FF0000) //Only create objects when the Red byte is 255 - TBD: Look up index to determine what to do
    {
	//make sure the block isn't already in the map
	BlockMapKey tempBlockMapKey(xStart, xEnd, blockIndex);
    
	BlockMap::iterator iter = m_blockMap.find(tempBlockMapKey);
    
	if(iter == m_blockMap.end())
	{
	    AddBlock(tempBlockMapKey,
		     xStart,
		     j,
		     xEnd,
		     j+1,
		     blockIndex);
	}
	else
	{
	    //check to see if an existing block just needs to be stretched downward
	    Block *pBlock = iter->second;
	    if(pBlock->IsExtension(j))
	    {
		pBlock->Extend();
	    }
	    else
	    {
		AddBlock(tempBlockMapKey,
			 xStart,
			 j,
			 xEnd,
			 j+1,
			 blockIndex);
	    }
	}
    }
}

