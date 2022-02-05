import torch

a = torch.randn((3,6),requires_grad=True)
b = torch.randn((3,6),requires_grad=True)
c = a+b 
F = torch.nn.Sigmoid()
d = F(c)
loss = torch.sum(d)
loss.backward()
a.grad
b.grad
