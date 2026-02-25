from torchvision.datasets import MNIST
import torchvision.transforms as transforms

from torch.utils.data import DataLoader

import torch
import torch.nn as nn

#定义数据集预处理
transform = transforms.Compose([transforms.ToTensor(),transforms.Normalize((0.1307,),(0.3081,))])

#加载训练集与测试集
train_data = MNIST(root = './data',train = True,download=True,transform=transform)
test_data = MNIST(root = './data',train = False,download=True,transform=transform)

#创建数据加载器
train_loader = DataLoader(train_data,shuffle = True,batch_size=64)
test_loader = DataLoader(test_data,shuffle = False,batch_size=64)

#模型构造
class Model(nn.Module):
    def __init__(self):
        super(Model,self).__init__()
        self.linear1 = nn.Linear(784,256)
        self.linear2 = nn.Linear(256,64)
        self.linear3 = nn.Linear(64,10)
    def forward(self,x):
        x = x.view(x.size(0),-1)
        x = torch.relu(self.linear1(x))
        x = torch.relu(self.linear2(x))
        x = torch.relu(self.linear3(x))
        return x

#损失函数
criterion = nn.CrossEntropyLoss()#softmax+交叉熵函数

#定义优化器
model = Model()
optimizer = torch.optim.SGD(model.parameters(),lr=0.2)

#训练函数
def train(model,train_loader,criterion,optimizer):
    for idx,(inputs,targets) in enumerate(train_loader):
        optimizer.zero_grad()
        outputs = model(inputs)
        loss = criterion(outputs,targets)
        loss.backward()
        optimizer.step()
    torch.save(model.state_dict(),"model.pth")
    print("模型训练完毕")

#测试函数
def test(model,test_loader):
    correct,total = 0,0
    with torch.no_grad():
        for(input,target) in test_loader:
            output = model(input)
            values,predict = torch.max(output.data,dim = 1)
            total += target.size(0)
            correct += (predict == target).sum().item()
        print(correct/total)


#train(model,train_loader,criterion,optimizer)
model.load_state_dict(torch.load("model.pth"))
test(model,test_loader)


