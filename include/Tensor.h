#pragma once
#include "Autograd.h"
#include <vector>
class Tensor//: public Node
{
    public:
        Tensor() = default;
        ~Tensor();
        Tensor(std::vector<int> shapes,std::vector<double> data,bool require_grad=false);
        // Tensor operator +(Tensor& B);
        std::vector<double>& data();
        int size();
        std::vector<int>& shapes();
        bool require_grad();
        std::string toString();
        void insertEdge(Edge* edge);
        AutogradMeta* getGradMeta();
    private:
        int m_size;
        int m_ndims;
        std::vector<int> m_shapes;
        std::vector<int> m_strides;
        std::vector<double> m_data;
        std::vector<double> m_grad;
        AutogradMeta* m_pGradMeta = nullptr;
};

