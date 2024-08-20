#ifndef NODEDATA_H
#define NODEDATA_H

#include <map>

struct NodeInfo
{
    NodeInfo():
	    m_red(0),
	    m_green(0),
	    m_blue(0),
	    m_linkNodeId(0)
    {
    }
    
    ~NodeInfo()
    {
    }
    
    unsigned int m_red;
    unsigned int m_green;
    unsigned int m_blue;
    unsigned int m_linkNodeId;
};



class NodeData
{
 public:
    NodeData();
    ~NodeData();
    bool getNodeData(unsigned int index, NodeInfo& nodeInfo) const;
private:
    std::map<unsigned int,NodeInfo> m_nodeInfoMap;
};

#endif
