# Transport-Layer-Security-TLS-Protocol-Compression
The Transport Layer Security (TLS) protocol includes features to negotiate selection of a lossless data compression method as part of the TLS Handshake Protocol and then to apply the algorithm associated with the selected method as part of the TLS Record Protocol. Ref.http://buildbot.tools.ietf.org

# Project Requirements

In oder to run this project you need install Virtual Box (Oracle), version 6.0. 

## Operating System

Our project is running under Ubuntu, version 18.04.01.

## Prerequisites
  
#  1. NS-3 prerequsites

      Install Git , Version : 2.17.1
         $ sudo apt update
         $ sudo apt install git
      Install Phyton , Version : 3.7
    
#  2. Installing NS-3
      
      In order to install NS-3 you should follow link:
      [https://www.nsnam.org/docs/tutorial/html/getting-started.html#building-ns-3]
     
      List of used commands:
    
        $ cd
        $ mkdir workspace
        $ cd workspace
        $ git clone https://gitlab.com/nsnam/ns-3-allinone.git
        $ cd ns-3-allinone
        $ python download.py
        $ python download.py -n ns-3.29
      
 # 3. Testing NS-3
      
      For testing NS-3, run the following command:
   
        $ ./test.py
   
      You should see following message:
      92 of 92 tests passed (92 passed, 0 failed, 0 crashed, 0 valgrind errors)
   
# 4. Additional Setup

      a) Install cMake , version: 3.10.02, by using the following command
 
        $ sudo apt install cmake
        
      b) Install mercurial, by using the following command
      
        $ sudo apt-get install mercurial
        
      c) Add zlib library 
      
      d) Install waf
   
 # 5. Recommended tools
     
     a) Install Wireshark, version 2.6.6, by using following commands.
     
        $ sudo add-apt-repository ppa:dreibh/ppa
        $ sudo apt-get update
        $ sudo apt-get install wire-shark
        $ sudo apt-get –f install 
        
     b) Install version control, GitKraken, by using these commands
     
        $ wget [https://release.gitkraken.com/linux/gitkraken-amd64.deb]
        $ dpkg -i gitkraken-amd64.deb
        
        If any problem occurred: Package gconf2 is not installed. In order to install that packet run this command:
       
        $ sudo apt --fix-broken install
      
        Run this command again:
        
        $ dpkg -i gitkraken-amd64.deb

     c) IDE For C++
      
        Go to Visual Studio web site and download Visual studio for debian. 
        
        $sudo dpkg -i code_1.31.1-1549938243_amd64.deb

  # 6. Building project
      
        In order to build the project run this command. For running these commands you need you need to be in: $  workspace/Transport-Layer-Security/ns-3-allinone/ns-3-dev.
      
      $ ./waf configure
      $ ./waf build
      $ ./waf -- run
     
   
   
   
   
   
   
   
   
     
     

