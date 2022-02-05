#include "Autograd.h"
#include <unordered_set>
#include <queue>
AutogradMeta::AutogradMeta(){}
AutogradMeta::AutogradMeta(bool require_grad): m_requireGrad(require_grad){}
AutogradMeta::~AutogradMeta(){}
std::vector<Edge* >& AutogradMeta::getEdgeList()
{
    return m_edges;
}
bool AutogradMeta::require_grad(){
    return m_requireGrad;
}

bool AutogradMeta::isLeaf()
{
    return m_edges.empty();
}
void AutogradMeta::release()
{
    return;
}

void AutogradMeta::insertEdge(Edge* edge){
    m_edges.push_back(edge);
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
            std::vector<Node*> nodes = edge->apply();
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