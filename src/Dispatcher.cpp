#include "Dispatcher.h"
#include "ops.h"
Dispatcher::Dispatcher()
{
    registerOPS("randn",&randn);
}
KernelFunctorPtr Dispatcher::getFunctor(std::string name)
{
    if (m_kernelFunctors.find(name)==m_kernelFunctors.end())
        return nullptr;
    return m_kernelFunctors[name];
}