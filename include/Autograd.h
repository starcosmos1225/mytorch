#pragma once
#include "Node.h"
#include "Edge.h"
// class A
// {
//     public:
//         A(){}
//         int v;
// };
class AutogradMeta: public Node
{
    public:
        AutogradMeta();
        AutogradMeta(bool require_grad);
        ~AutogradMeta();
        virtual std::vector<Edge* >& getEdgeList();
        virtual bool require_grad();
        virtual bool isLeaf();
        virtual void release();
        virtual void insertEdge(Edge* edge);
    private:
        bool m_requireGrad;
        // std::vector<Edge*> m_edges;
};

void backward(Node* node);

