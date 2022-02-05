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
        AddBackward(Node* self, Node* other,Node* out);
        virtual std::vector<Node*> apply() override;
    private:
        Node* m_self;
        Node* m_other;
        Node* m_out;
};

class SigmoidBackward: public Edge
{
    public:
        SigmoidBackward() {};
        ~SigmoidBackward(){}
        SigmoidBackward(Node* input, Node* out);
        virtual std::vector<Node*> apply() override;
    private:
        Node* m_input;
        Node* m_out;
};

class SumBackward: public Edge
{
    public:
        SumBackward() {};
        ~SumBackward(){}
        SumBackward(Node* input, Node* out);
        virtual std::vector<Node*> apply() override;
    private:
        Node* m_input;
        Node* m_out;
};