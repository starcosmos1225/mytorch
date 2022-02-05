
#include "Tensor.h"
#include <iostream>
Tensor::Tensor(std::vector<int> shapes, std::vector<double> data,bool require_grad):
m_shapes(std::move(shapes)),
m_data(std::move(data))
{
    m_size = 1;
    for (int i:m_shapes)
    {
        m_size *= i;
    }
    m_ndims = m_shapes.size();
    if (require_grad)
    {
        m_pGradMeta = new AutogradMeta();
    }
}

Tensor::~Tensor(){
    delete(m_pGradMeta);
}

std::vector<double>& Tensor::get()
{
    return m_data;
}

bool Tensor::require_grad(){
    return m_pGradMeta != nullptr;
}

std::vector<int>& Tensor::shapes()
{
    return m_shapes;
}

int Tensor::size()
{
    return m_size;
}

std::string Tensor::toString()
{
    std::string res;
    for (int i=0;i<m_size;i++)
    {
        if (i!=0)
            res +=", ";
        res += std::to_string(m_data[i]);
    }
    return res;  
}

void Tensor::insertEdge(Edge* edge)
{
    assert(m_pGradMeta!=nullptr);
    m_pGradMeta->insertEdge(edge);
}

AutogradMeta* Tensor::getGradMeta(){
    return m_pGradMeta;
}