#!/usr/bin/env python
import os, sys
import subprocess
import numpy as np
import matplotlib.pyplot as plt1
import matplotlib.pyplot as plt2


 
def build_projgen():
    subprocess.call('./waf --run "scratch/point2point --IsCompress=0 --CompressionDataRate=1"', shell=True, cwd='ns-3-dev/')
    subprocess.call('./waf --run "scratch/point2point --IsCompress=0 --CompressionDataRate=2"', shell=True, cwd='ns-3-dev/')
    subprocess.call('./waf --run "scratch/point2point --IsCompress=0 --CompressionDataRate=3"', shell=True, cwd='ns-3-dev/')
    subprocess.call('./waf --run "scratch/point2point --IsCompress=0 --CompressionDataRate=4"', shell=True, cwd='ns-3-dev/')
    subprocess.call('./waf --run "scratch/point2point --IsCompress=0 --CompressionDataRate=5"', shell=True, cwd='ns-3-dev/')
    subprocess.call('./waf --run "scratch/point2point --IsCompress=0 --CompressionDataRate=6"', shell=True, cwd='ns-3-dev/')
    subprocess.call('./waf --run "scratch/point2point --IsCompress=0 --CompressionDataRate=7"', shell=True, cwd='ns-3-dev/')
    subprocess.call('./waf --run "scratch/point2point --IsCompress=0 --CompressionDataRate=8"', shell=True, cwd='ns-3-dev/')
    subprocess.call('./waf --run "scratch/point2point --IsCompress=0 --CompressionDataRate=9"', shell=True, cwd='ns-3-dev/')
    subprocess.call('./waf --run "scratch/point2point --IsCompress=0 --CompressionDataRate=10"', shell=True, cwd='ns-3-dev/')

    subprocess.call('./waf --run "scratch/point2point --IsCompress=1 --CompressionDataRate=1"', shell=True, cwd='ns-3-dev/')
    subprocess.call('./waf --run "scratch/point2point --IsCompress=1 --CompressionDataRate=2"', shell=True, cwd='ns-3-dev/')
    subprocess.call('./waf --run "scratch/point2point --IsCompress=1 --CompressionDataRate=3"', shell=True, cwd='ns-3-dev/')
    subprocess.call('./waf --run "scratch/point2point --IsCompress=1 --CompressionDataRate=4"', shell=True, cwd='ns-3-dev/')
    subprocess.call('./waf --run "scratch/point2point --IsCompress=1 --CompressionDataRate=5"', shell=True, cwd='ns-3-dev/')
    subprocess.call('./waf --run "scratch/point2point --IsCompress=1 --CompressionDataRate=6"', shell=True, cwd='ns-3-dev/')
    subprocess.call('./waf --run "scratch/point2point --IsCompress=1 --CompressionDataRate=7"', shell=True, cwd='ns-3-dev/')
    subprocess.call('./waf --run "scratch/point2point --IsCompress=1 --CompressionDataRate=8"', shell=True, cwd='ns-3-dev/')
    subprocess.call('./waf --run "scratch/point2point --IsCompress=1 --CompressionDataRate=9"', shell=True, cwd='ns-3-dev/')
    subprocess.call('./waf --run "scratch/point2point --IsCompress=1 --CompressionDataRate=10"', shell=True, cwd='ns-3-dev/')
  

def GraphPlot():

     data = np.genfromtxt('ns-3-dev/output_rar_team.txt',delimiter=',')
     y1 = []
     y2 = []
     for i in range(1, 10):
          y1 = data[i]
          y2 = data[i+10]
     print(y1)
     print(y2)
     labels = ["1Mbps", "2Mbps", "3Mbps", "4Mbps","5Mbps","6Mbps","7Mbps","8Mbps", "9Mbps", "10Mbps"]  
     plt1.plot(data[0:9], 'ro')
     plt2.plot(data[10:19], 'r--')
     plt3.plot(data, 'ro')
     plt1.axis([0, 10, 0, 8000])
     plt1.xlabel("Data Rate")
     plt1.ylabel("Delta(H) - Delta(L)[mS]")
     plt1.title("Point to Point Network with Compression Link")
     plt1.legend("no-compression")
     plt2.legend("compression")
     plt1.show()
     plt2.show()

     return        

def main():
     build_projgen()
     GraphPlot()
 
if __name__ == '__main__':
    main()