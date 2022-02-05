#pragma once
#include "KernelFunctor.h"
#include <string>
#include <unordered_map>
#include <iostream>
class Dispatcher{
    public:
        Dispatcher();
        static Dispatcher& singleton(){
            static Dispatcher dispatcher;
            return dispatcher;
        }
        KernelFunctorPtr getFunctor(std::string name);
    private:
        template <class FuncType>
        void registerOPS(std::string name, FuncType* function)
        {
            std::cout<<"register1:"<<name<<std::endl;
            void* func = reinterpret_cast<void*>(function);
            KernelFunctorPtr funcPtr = std::make_shared<KernelFunctor>(func);
            m_kernelFunctors[name] = funcPtr;
        }
        std::unordered_map<std::string, KernelFunctorPtr> m_kernelFunctors; 
};