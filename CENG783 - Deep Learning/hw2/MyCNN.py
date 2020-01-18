import torch.nn as nn

class Net(nn.Module):
  def __init__(self):
    super(Net, self).__init__()
    # <<< YOUR CODE HERE >>> #
    self.numberOfClasses = 10
    self.firstConv = nn.Conv2d(1,6,3)
    self.secondConv = nn.Conv2d(6,10,3)
    self.adaptiveMaxPooling = nn.AdaptiveMaxPool2d((8,8))
    self.firstFull = nn.Linear(640,10)
    #self.secondFull = nn.Linear(100,10)

    self.relu = nn.ReLU()

  def forward(self, x):
    # <<< YOUR CODE HERE >>> #
    x = self.firstConv(x)
    x = self.relu(x)
    x = self.secondConv(x)
    x = self.relu(x)
    x = self.adaptiveMaxPooling(x)
    x = self.flatten(x,640)
    
    x = self.firstFull(x)

    return x
  
  def flatten(self,x,featureNumber):
    return x.view(1,featureNumber)