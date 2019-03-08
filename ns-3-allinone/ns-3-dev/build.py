#!/usr/bin/env python
import os, sys, pip
from subprocess import call
sys.dont_write_bytecode = True
try:
    import intelhex
except ImportError:
    pip.main(["install", 'intelhex==2.0'])
    import intelhex
 
GENERATE_DIR = ''
 
def build_projgen():
    build_args = './waf'
    call(''.format(path, build_args), shell=True)

 
 
def main():
     build_projgen()
  
 
 
if __name__ == '__main__':
    main()