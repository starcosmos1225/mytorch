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
        std::vector<double>& get();
        int size();
        std::vector<int>& shapes();
        // std::vector<Edge* >& getEdgeList();
        // bool require_grad();
        // bool isLeaf();
        // void release();
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
        // bool m_requireGrad;
        
        //temp attr
        // int m_value;
        // A a;
        AutogradMeta* m_pGradMeta = nullptr;
};

