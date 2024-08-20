#include "NodeData.h"
#include "tinyxml2.h"

#include <iostream>

NodeData::NodeData()
{
	// Example code for loading a dummy xml file
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError errorCode = doc.LoadFile("NodeInfo.xml");
	if (errorCode != tinyxml2::XML_SUCCESS)
	{
		std::cerr << "Error loading XML file" << std::endl;
	}
	else
	{
		// Note that the root element may not actually have any value of its own
		// it could just be the root of the document
		// use named children to get specific elements
		tinyxml2::XMLNode *pRoot = doc.RootElement();
		if (!pRoot)
		{
			std::cerr << "Error getting root element" << std::endl;
			abort();
		}

		// Using a named child to get an element
		tinyxml2::XMLElement *pNode = pRoot->FirstChildElement("Node");
		while (pNode)
		{
			NodeInfo nodeInfo;
			int nodeId = 0;
			tinyxml2::XMLElement *pNodeId = pNode->FirstChildElement("NodeId");
			if (pNodeId)
			{
				pNodeId->QueryIntText(&nodeId);
				std::cout << "NodeId: " << nodeId << std::endl;
			}
			tinyxml2::XMLElement *pColor = pNode->FirstChildElement("Color");
			if (pColor)
			{
				tinyxml2::XMLElement *pRed = pColor->FirstChildElement("Red");
				if (pRed)
				{
					pRed->QueryUnsignedText(&nodeInfo.m_red);
					std::cout << "Red: " << nodeInfo.m_red << std::endl;
				}
				tinyxml2::XMLElement *pGreen = pColor->FirstChildElement("Green");
				if (pGreen)
				{
					pGreen->QueryUnsignedText(&nodeInfo.m_green);
					std::cout << "Green: " << nodeInfo.m_green << std::endl;
				}
				tinyxml2::XMLElement *pBlue = pColor->FirstChildElement("Blue");
				if (pBlue)
				{
					pBlue->QueryUnsignedText(&nodeInfo.m_blue);
					std::cout << "Blue: " << nodeInfo.m_blue << std::endl;
				}
			}
			tinyxml2::XMLElement *pLinkTargetId = pNode->FirstChildElement("LinkTargetId");
			if (pLinkTargetId)
			{
				pLinkTargetId->QueryUnsignedText(&nodeInfo.m_linkNodeId);
				std::cout << "LinkTarget: " << nodeInfo.m_linkNodeId << std::endl;
			}

			m_nodeInfoMap[nodeId] = nodeInfo;
			pNode = pNode->NextSiblingElement("Node");
		}
	}
}

NodeData::~NodeData()
{
}

bool NodeData::getNodeData(unsigned int index, NodeInfo &nodeInfo) const
{
	std::map<unsigned int, NodeInfo>::const_iterator iter = m_nodeInfoMap.find(index);
	if (iter != m_nodeInfoMap.end())
	{
		nodeInfo = iter->second;
		return true;
	}
	else
	{
		return false;
	}
}
