import numpy as np

def activationFunction(t):
    if(t<=-1):
        return 0.
    elif(t<1):
        return 0.5 + t/2
    else:
        return 1.

def biasGradient(t):
    if(t<=-1):
        return 0.
    elif(t<1):
        return 0.5
    else:
        return 0.

def inRange(t):
    if(t<=-1):
        return 0.
    elif(t<1):
        return 1.
    else:
        return 0.


def vectorizedActivationFunction(T):
    return np.vectorize(activationFunction)(T)

def vectorizedBiasGradient(T):
    return np.vectorize(biasGradient)(T)

def vectorizedWeightGradient(T,X):
    toBeMultiplied = vectorizedBiasGradient(T)
    toBeMultiplied = toBeMultiplied.reshape(-1,1)
    returnValue = np.multiply(toBeMultiplied,X)
    return returnValue
    

def f(x,w,b): # do not change this line!
    # implement the function f() here
    # x is a N-by-D numpy array
    # w is a D dimensional numpy array
    # b is a scalar
    # Should return three things:
    # 1) the output of the f function, as a N dimensional numpy array,
    # 2) gradient of f with respect to w,
    # 3) gradient of f with respect to b

    #print("X shape : ",x.shape)
    #print("W shape : ",w.shape)

    multiplicationResult = np.matmul(w,x.T)
    additionResult = np.add(multiplicationResult,b)
    activatedResult = vectorizedActivationFunction(additionResult)

    wFGradient = vectorizedWeightGradient(activatedResult,x)
    bFGradient = vectorizedBiasGradient(activatedResult)
    
    return activatedResult, wFGradient , bFGradient

def l2loss(x,y,w,b): # do not change this line!
    # implement the l2loss here
    # x is a N-by-D numpy array
    # y is a N dimensional numpy array
    # w is a D dimensional numpy array
    # b is a scalar
    # Should return three items:
    # 1) the L2 loss which is a scalar,
    # 2) the gradient of the loss with respect to w,
    # 3) the gradient of the loss with respect to b
    activation , fWGradient , fBGradient = f(x,w,b)

    datasetSize = x.shape[0]
    difference = np.subtract(activation,y)
 
    squared = np.square(difference)
    summary = np.sum(squared)
    loss = summary/(datasetSize)
    print("Loss : ", loss)

    activatedMatrix = np.vectorize(inRange)(activation)
    wLGradient = np.matmul(difference.T,x)
    bLGradient = np.sum(difference)/datasetSize

    return loss,wLGradient,bLGradient


def minimize_l2loss(x,y,w,b, num_iters=1000, eta=0.0001): # do not change this line!
    # implement the gradient descent here
    # x is a N-by-D numpy array
    # y is a N dimensional numpy array
    # w is a D dimensional numpy array
    # b is a scalar
    # num_iters (optional) is number of iterations
    # eta (optional) is the stepsize for the gradient descent
    # Should return the final values of w and b
    for i in range(num_iters):
        loss, wLGradient , bLGradient = l2loss(x,y,w,b)
        weightDifference = np.multiply(eta,wLGradient)
        biasDifference = np.multiply(eta,bLGradient)
        w = np.subtract(w,weightDifference)
        b = np.subtract(b,biasDifference)
    return w,b
