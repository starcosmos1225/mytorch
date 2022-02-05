#include "Edge.h"
#include <iostream>
AddBackward::AddBackward(Node* self, Node* other):
m_self(self),
m_other(other)
{}

std::vector<Node*> AddBackward::apply()
{
    std::cout<<"add backward apply"<<std::endl;
    return {m_self,m_other};
}
