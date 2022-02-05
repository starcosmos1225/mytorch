#pragma once
#include "Node.h"
#include "Edge.h"

class Tensor;
class AutogradMeta: public Node
{
    public:
        AutogradMeta();
        AutogradMeta(Tensor* tensor);
        ~AutogradMeta();
        virtual std::vector<Edge* >& getEdgeList();
        virtual bool isLeaf();
        virtual void release();
        virtual void insertEdge(Edge* edge);
        virtual std::vector<double>& grad();
        virtual std::vector<double>& data();
        std::string toString();

    private:
        Tensor* m_pTensor;
        std::vector<double> m_grad;
};

void backward(Node* node);

