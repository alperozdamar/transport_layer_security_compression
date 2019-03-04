import numpy as np
import matplotlib.pyplot as plt1
import matplotlib.pyplot as plt2

def GraphPlot():
    data = np.genfromtxt('measure.txt',delimiter=',')
    print(data)
    Subj = ["1Mbps", "2Mbps", "3Mbps", "4Mbps","5Mbps","6Mbps","7Mbps","8Mbps", "9Mbps", "10Mbps"]

    plt1.plot(data)
    plt1.xlim(0,6000)
    plt1.xlabel("Number of packet")
    plt1.ylabel("Delta(H) - Delta(L)[S]")
    plt1.legend(Subj)
    plt2.plot(data)
    plt1.show()
    plt2.show()
    return        

if __name__ == '__main__':
     GraphPlot()

