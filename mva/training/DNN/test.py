import torch

model = torch.jit.load("model.pt", map_location=torch.device('cpu'))
model.eval()
m = torch.jit.script(model)
m.save("model_cpu.pt")