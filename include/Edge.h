#pragma once
#include <vector>
class Node;
class Edge
{
    public:
        Edge(){}
        ~Edge(){}
        virtual std::vector<Node*>  apply() = 0;
};

class AddBackward: public Edge
{
    public:
        AddBackward() {};
        ~AddBackward(){}
        AddBackward(Node* self, Node* other);
        virtual std::vector<Node*> apply() override;
    private:
        Node* m_self;
        Node* m_other;
};