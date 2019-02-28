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
      
        sudo apt-get install zlib1g-dev
        sudo apt-get install libpng-dev
        
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

  ## 6. Building project
      
        In order to build the project run this command. 
        xFor running these commands you need you need to be in: $  workspace/Transport-Layer-Security/ns-3-allinone/ns-3-dev.
      
      $ ./waf configure
      $ ./waf build
      $ ./waf -- run
      $ ./waf --run "scratch/point2point --CompressionDataRate=20"
   
  ## 7. Project description
    
   # Project outcomes
    
       1. Enable network compression for point-to-point links in ns-3.
       2. Implement the network application that detects the presence of network compression by end-hosts.
       3. Verify and validate your simulated compression link and compression detection application
   
   # Overview
    
For this project you will implement a network level compression link. You will then implement a network application to detect whether network compression is present to validate your simulated compression link. It is inspired by the work, End-to-End Detection of Compression of Traffic Flows by Intermediaries [https://lasr.cs.ucla.edu/vahab/resources/compression_detection.pd](https://lasr.cs.ucla.edu/vahab/resources/compression_detection.pd), which is recommended that you read in detail up to Section VI.
Crucial to success in this project will be a deep and detailed reading and understanding of the ns-3 documentation, where it is relevant to your project. Among the ns-3 documentation [https://www.nsnam.org/releases/ns-3-29/documentation/] are tutorials, a reference manual, a model library, and a full API reference. You should explore and make use of all of these resources.

   # Components
   
   #  (1) Compression Link
   
   Your compression link application must be built using ns-3. It will be responsible for compression and decompression of incoming and outgoing packets. You will use the Lempel-Ziv-Stac algorithm for compression and decompression, for which you may find and use a library. Your compression link should follow some requirements in the RFC 1974: PPP Stac LZS Compression Protocol.
Specifically, you should implement functionality that takes a PPP packet, and first checks the protocol number in the header. You should also have a configuration file that specifies packet types to compress. If you determine that the checked packet type matches one in the configuration file, you should pre-process then compress it. For the purposes of this project, your configuration file will only have one protocol entered, IP, protocol number 0x0021.
Pre-processing should first take the matched packet then replace the original protocol number with the LZS protocol number, 0x4021. It should then take the original protocol number and append it to the original data, and compress that whole bitstring and replace it with the original data section in the original packet, as illustrated in Figure 1. Decompressor, at the other side of the compression link, should then reverse all the pre-processing steps performed at the compressor, to retrieve the original incoming packet ((1) in Figure 1), before pushing it to the next interface.
You should not worry about implementing Compression Control Protocol and its corresponding control packets, as you may assume the two routers have already reached an Opened state and LZS has been negotiated as the primary compression algorithm. Similarly, you should also not implement anything from the negotiation phase, and not worry about using the exact compression data format, specified in X3.241-1994.
You may only need to modify PointToPointNetDevice to enable compression/decompression on point-to-point links. A good starting point to get familiar with, in addition to PointToPointNetDevice is the ns-3 point-to-point model overview.

![Image](image.png)
   
A good starting point for building ns-3 applications might be this ns-3 wiki article, How To Make and Use A New Application.
  
   
     
     

