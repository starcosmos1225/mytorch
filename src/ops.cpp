#include "ops.h"
#include "iostream"
#include "torch_random.h"
void randn16(std::vector<double>& data, int begin)
{
    for (int i=begin;i<begin+8;i++)
    {
        double u1 = 1 - data[i]; // [0, 1) -> (0, 1] for log.
        double u2 = data[i + 8];
        double radius = std::sqrt(-2 * std::log(u1));
        double theta = 2.0f * PI * u2;
        data[i] = radius * std::cos(theta);
        data[i + 8] = radius * std::sin(theta);
    }
}
void create_randn(std::vector<double>& data)
{
    unsigned int size = data.size();
    // std::cout<<"begin randn"<<std::endl;
    for (unsigned int i=0;i<data.size();i++)
    {
        
        data[i] = Random<>::singleton().valueInRange(0.0f,1.0f);
        // std::cout<<"data ["<<i<<"]="<<data[i]<<std::endl;
    }
    // std::cout<<"size:"<<size<<std::endl;
    if (size<16)
    {
        for (unsigned int i=0;i<16-size;i++)
            data.push_back(Random<>::singleton().valueInRange(0.0f,1.0f));
        randn16(data,0);
        for (unsigned int i=0;i<16-size;i++)
            data.pop_back();
    }else
    {
        for (unsigned int i=0;i<=size-16;i+=16)
        {
            randn16(data,i);
        }
        if (size%16!=0)
        {
            for (unsigned int i=0;i<16;i++)
            {
                data[size-16+i] = Random<>::singleton().valueInRange(0.0f,1.0f);
            }
            randn16(data, size-16);
        }
    }
    
}
Tensor* randn(std::vector<int> shapes, bool require_grad)
{
    int size=1;
    for (int s:shapes)
    {
        size *=s;
    }
    std::vector<double> data(size);
    create_randn(data);
    return new Tensor(shapes,data, require_grad);
}

Tensor* sigmoid(Tensor* input)
{
    // std::cout<<"call sigmoid"<<std::endl;
    auto& data = input->data();
    std::vector<double> new_data(data.size(),0.0);
    // std::cout<<"call sigmoid size:"<<data.size()<<std::endl;
    for (unsigned int i=0;i<data.size();i++)
    {
        new_data[i] = 1.0/(1.0+std::exp(-data[i]));
    }
    return new Tensor(input->shapes(), new_data, input->require_grad());
}

Tensor* sum(Tensor* input)
{
    // std::cout<<"call sum"<<std::endl;
    auto& data = input->data();
    std::vector<double> new_data(1,0.0);
    for (unsigned int i=0;i<data.size();i++)
    {
        new_data[0] += data[i]; 
    }
    return new Tensor({1}, new_data, input->require_grad());
}