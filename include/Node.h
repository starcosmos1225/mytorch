#pragma once
#include "Edge.h"
class Node
{
    public:
        Node(){}
        ~Node(){}
        virtual std::vector<Edge* >& getEdgeList()=0;
        // virtual bool require_grad()=0;
        virtual bool isLeaf()=0;
        virtual void release()=0;
        virtual void insertEdge(Edge* edge)=0;
        virtual std::vector<double>& grad()=0;
        virtual std::vector<double>& data()=0;
    protected:
        std::vector<Edge* > m_edges;
};