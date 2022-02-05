#include <Tensor.h>
Tensor* randn(std::vector<int> shapes, bool require_grad);
Tensor* sigmoid(Tensor* input);
Tensor* sum(Tensor* input);