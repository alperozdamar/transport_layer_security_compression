#!/usr/bin/env python
import os, sys
import subprocess


 
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
  

def main():
     build_projgen()
 
if __name__ == '__main__':
    main()