#include "Edge.h"
#include <iostream>
#include "Node.h"
#include<algorithm>
AddBackward::AddBackward(Node* self, Node* other, Node* out):
m_self(self),
m_other(other),
m_out(out)
{}

std::vector<Node*> AddBackward::apply()
{
    auto& self_grad = m_self->grad();
    auto& other_grad = m_other->grad();
    auto& out_grad = m_out->grad();
    // std::cout<<"begin apply:"<<self_grad.size()<<" "<<other_grad.size()<<" "<<out_grad.size()<<std::endl;
    if (self_grad.size()==0)
        self_grad = out_grad;
    else{
        assert(self_grad.size()==out_grad.size());
        for (unsigned int i=0;i<out_grad.size();i++)
            self_grad[i] += out_grad[i];
    }
    if (other_grad.size()==0)
        other_grad = out_grad;
    else{
        assert(other_grad.size()==out_grad.size());
        for (unsigned int i=0;i<out_grad.size();i++)
            other_grad[i] += out_grad[i];
    }
    return {m_self,m_other};
}

SigmoidBackward::SigmoidBackward(Node* input, Node* out):
m_input(input),
m_out(out)
{}
std::vector<Node*> SigmoidBackward::apply()
{
    // backward dx = out*(1-out)
    auto& input_grad = m_input->grad();
    auto& out_grad = m_out->grad();
    auto& out_data = m_out->data();
    if (input_grad.size()==0)
        input_grad = std::vector<double>(out_grad.size(),0.0);  
    else
        assert(input_grad.size()==out_grad.size());
    for (unsigned int i=0;i<input_grad.size();i++)
    {
        input_grad[i] += out_grad[i]*out_data[i]*(1-out_data[i]);
    }
    return {m_input};
}

SumBackward::SumBackward(Node* input, Node* out):
m_input(input),
m_out(out)
{}
std::vector<Node*> SumBackward::apply()
{
    // std::cout<<"begin sum back"<<std::endl;
    auto& input_grad = m_input->grad();
    auto& out_grad = m_out->grad();
    auto& input_data = m_input->data();
    // std::cout<<"out grad size:"<<out_grad.size()<<std::endl;
    assert(out_grad.size()==1);
    if (input_grad.size()==0)
        input_grad = std::vector<double>(input_data.size(),0.0);  
    for (unsigned int i=0;i<input_grad.size();i++)
    {
        input_grad[i] += out_grad[0];
    }
    // std::cout<<"end sum back"<<std::endl;
    return {m_input};
}
