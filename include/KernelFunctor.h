#pragma once
#include <memory>
class KernelFunctor
{
    public:
        KernelFunctor() =default;
        KernelFunctor(void* func):m_pFunc(func){}
        template<class Return, class... Parameters>
        Return call(Parameters... args)
        {
            // void* func = reinterpret_cast<void*>(&function);
            using ActualSignature = Return (Parameters...);
            ActualSignature* ret = reinterpret_cast<ActualSignature*>(m_pFunc);
            return (*ret)(std::forward<Parameters>(args)...);
        }
    private:
        void* m_pFunc;
};

using KernelFunctorPtr = std::shared_ptr<KernelFunctor>;