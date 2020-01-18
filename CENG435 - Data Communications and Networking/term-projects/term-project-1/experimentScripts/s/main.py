import time
import socket
import sys
from threading import Thread
import os

class Client(Thread):
    def __init__(self,address,port,targetName,numberOfTries):
        #Part will be used for initializations(ex. socket, etc.)
        self.differenceAccumulator = 0
        self.address = address
        self.port = port
        self.numberOfTries = numberOfTries
        self.currentName = 's'
        self.targetName = targetName
        self.sentTimes = []

    def startClient(self):
        #Create thread and start it
        clientThread = Thread(target = self.clientFunction)
        self.clientThread = clientThread
        clientThread.start()

    def waitForClient(self):
        #Allows us to wait for it in main
        self.clientThread.join()


    def clientFunction(self):
        # Create a UDP socket
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        server_address = (self.address, self.port)

        currentTries = 0

        try:
            while currentTries < self.numberOfTries : 
                #print ( '\nClient sends "%s"' % message)
                #get sending time
                sentTime = str(time.time())
                #encode it so it can be sent over the network
                encodedSentTime = sentTime.encode()
                #send it to server
                sent = sock.sendto(encodedSentTime, server_address)
                self.sentTimes.append(sentTime)
                #wait for r3 ack
                data, server = sock.recvfrom(4096)
                if data :
                    #receive ack from r3 , so it doesn't flood the network with packets
                    print("ACK received by s")

                #print ( '\nClient received "%s"' % data )
                #print ("\nRTT in ms is : ", difference*1000)
                currentTries += 1

        finally:
            print ( 'Client closing socket')
            sock.close()

    
if __name__ == "__main__":
    #Initialization or target information for our node
    os.system("sudo ntpdate -u pool.ntp.org")
    numberOfClientThreads = 1
    numberOfTries = 1000

    targetAddresses = ['10.10.3.2'] 
    targetPorts = [12000] 
    targetNames = ['r3']

    clientThreads = []

    for i in range(numberOfClientThreads):
        #Initializes server and client classes
        clientThreads.append(Client(targetAddresses[i],targetPorts[i],targetNames[i],numberOfTries))

    for i in range(numberOfClientThreads):
        #Runs server and client
        clientThreads[i].startClient()

    #main thread waits for client and server threads to finish
    for i in range(numberOfClientThreads):
        clientThreads[i].waitForClient()
