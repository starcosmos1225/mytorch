#include "Autograd.h"
#include <unordered_set>
#include <queue>
#include "Tensor.h"
#include <iostream>
AutogradMeta::AutogradMeta(){}
AutogradMeta::AutogradMeta(Tensor* tensor): m_pTensor(tensor){}
AutogradMeta::~AutogradMeta(){}
std::vector<Edge* >& AutogradMeta::getEdgeList()
{
    return m_edges;
}
// bool AutogradMeta::require_grad(){
//     return m_requireGrad;
// }

bool AutogradMeta::isLeaf()
{
    return m_edges.empty();
}
void AutogradMeta::release()
{
    // for (unsigned int i=0;i<m_grad.size();i++)
    //     std::cout<<m_grad[i]<<" ";
    // std::cout<<std::endl;
    m_grad.clear();
    m_grad.shrink_to_fit();
    return;
}

void AutogradMeta::insertEdge(Edge* edge){
    m_edges.push_back(edge);
}

std::vector<double>& AutogradMeta::grad()
{
    return m_grad;
}

std::vector<double>& AutogradMeta::data()
{
    return m_pTensor->data();
}
std::string AutogradMeta::toString()
{
    std::string res;
    for (unsigned int i=0;i<m_grad.size();i++)
    {
        if (i!=0)
            res +=", ";
        res += std::to_string(m_grad[i]);
    }
    return res;

}

void backward(Node* node)
{
    std::unordered_set<Node*> visited;
    std::queue<Node*> queue;
    queue.push(node);
    visited.insert(node);
    while (!queue.empty())
    {
        Node* out = queue.front();
        queue.pop();
        std::vector<Edge* > edges = out->getEdgeList();
        for (Edge* edge: edges)
        {
            // std::cout<<"begin apply"<<std::endl;
            std::vector<Node*> nodes = edge->apply();
            // std::cout<<"end apply"<<std::endl;
            for (Node* n:nodes)
            {
                if (n&&visited.find(n)==visited.end())
                {
                    queue.push(n);
                    visited.insert(n);
                }
            }
        }
        if (!out->isLeaf())
        {
            out->release();
        }
        visited.erase(out);
    }
}