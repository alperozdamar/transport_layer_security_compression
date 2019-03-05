/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007,2008,2009 INRIA, UDCAST
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Amine Ismail <amine.ismail@sophia.inria.fr>
 *                      <amine.ismail@udcast.com>
 */
#include "ns3/log.h"
#include "ns3/ipv4-address.h"
#include "ns3/nstime.h"
#include "ns3/inet-socket-address.h"
#include "ns3/inet6-socket-address.h"
#include "ns3/socket.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "udp-client.h"
#include "seq-ts-header.h"
#include <cstdlib>
#include <cstdio>

#include <iomanip> 
#include <vector>
#include <random>
#include <climits>
#include <algorithm>
#include <functional>
#include "ns3/boolean.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("UdpClient");

NS_OBJECT_ENSURE_REGISTERED (UdpClient);

//static uint8_t* byteArray;
static uint32_t PACKET_SIZE = 1100; //if we give 1100 it fragment so we reduce packet size

//TODO: Should be in Configuration Manager!!! (Because we are running our application 2 times.)
//bool highEntropy=false;

TypeId
UdpClient::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UdpClient")
    .SetParent<Application> ()
    .SetGroupName("Applications")
    .AddConstructor<UdpClient> ()
    .AddAttribute ("MaxPackets",
                   "The maximum number of packets the application will send",
                   UintegerValue (100),
                   MakeUintegerAccessor (&UdpClient::m_count),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Interval",
                   "The time to wait between packets", TimeValue (Seconds (1.0)),
                   MakeTimeAccessor (&UdpClient::m_interval),
                   MakeTimeChecker ())
    .AddAttribute ("RemoteAddress",
                   "The destination Address of the outbound packets",
                   AddressValue (),
                   MakeAddressAccessor (&UdpClient::m_peerAddress),
                   MakeAddressChecker ())
    .AddAttribute ("RemotePort", "The destination port of the outbound packets",
                   UintegerValue (100),
                   MakeUintegerAccessor (&UdpClient::m_peerPort),
                   MakeUintegerChecker<uint16_t> ())
    .AddAttribute ("PacketSize",
                   "Size of packets generated. The minimum packet size is 12 bytes which is the size of the header carrying the sequence number and the time stamp.",
                   UintegerValue (1024),
                   MakeUintegerAccessor (&UdpClient::m_size),
                   MakeUintegerChecker<uint32_t> (12,65507))
    .AddAttribute ("Entropy",
                   "Boolean Value",
                   BooleanValue(true),
                   MakeBooleanAccessor(&UdpClient::m_entropy),
                   MakeBooleanChecker()) 
  ;
  return tid;
}




UdpClient::UdpClient ()
{
  NS_LOG_FUNCTION (this);
  m_sent = 0;
  m_socket = 0;
  m_sendEvent = EventId ();
}

UdpClient::~UdpClient ()
{
  NS_LOG_FUNCTION (this);
}

void
UdpClient::SetRemote (Address ip, uint16_t port)
{
  NS_LOG_FUNCTION (this << ip << port);
  m_peerAddress = ip;
  m_peerPort = port;
}

void
UdpClient::SetRemote (Address addr)
{
  NS_LOG_FUNCTION (this << addr);
  m_peerAddress = addr;
}

void
UdpClient::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  Application::DoDispose ();
}

void
UdpClient::StartApplication (void)
{
  NS_LOG_FUNCTION (this);

  if (m_socket == 0)
    {
      TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
      m_socket = Socket::CreateSocket (GetNode (), tid);
      if (Ipv4Address::IsMatchingType(m_peerAddress) == true)
        {
          if (m_socket->Bind () == -1)
            {
              NS_FATAL_ERROR ("Failed to bind socket");
            }
          m_socket->Connect (InetSocketAddress (Ipv4Address::ConvertFrom(m_peerAddress), m_peerPort));
        }
      else if (Ipv6Address::IsMatchingType(m_peerAddress) == true)
        {
          if (m_socket->Bind6 () == -1)
            {
              NS_FATAL_ERROR ("Failed to bind socket");
            }
          m_socket->Connect (Inet6SocketAddress (Ipv6Address::ConvertFrom(m_peerAddress), m_peerPort));
        }
      else if (InetSocketAddress::IsMatchingType (m_peerAddress) == true)
        {
          if (m_socket->Bind () == -1)
            {
              NS_FATAL_ERROR ("Failed to bind socket");
            }
          m_socket->Connect (m_peerAddress);
        }
      else if (Inet6SocketAddress::IsMatchingType (m_peerAddress) == true)
        {
          if (m_socket->Bind6 () == -1)
            {
              NS_FATAL_ERROR ("Failed to bind socket");
            }
          m_socket->Connect (m_peerAddress);
        }
      else
        {
          NS_ASSERT_MSG (false, "Incompatible address type: " << m_peerAddress);
        }
    }

  m_socket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
  m_socket->SetAllowBroadcast (true);
  m_sendEvent = Simulator::Schedule (Seconds (0.0), &UdpClient::Send, this);
}

void
UdpClient::StopApplication (void)
{
  NS_LOG_FUNCTION (this);
  Simulator::Cancel (m_sendEvent);
}

std::vector<bool> generateRandomSequence() 
{
    std::vector<bool> randomSequence;
    randomSequence.resize(PACKET_SIZE);

    std::random_device rd;
    std::mt19937 generator(rd());
    std::bernoulli_distribution distribution(0.5); // your 50/50 chance

    std::generate(randomSequence.begin(), randomSequence.end(),
        [&generator, &distribution] { return distribution(generator); });

    return randomSequence;
}

void
UdpClient::Send (void)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT (m_sendEvent.IsExpired ());
  SeqTsHeader seqTs;
  seqTs.SetSeq (m_sent);
  
  //uint8_t we should generate randomly generated byte array and put it into payload.
  //bufffer => randomly generated byte array!!!! 
  //Packet (uint8_t const*buffer, uint32_t size);

  //8+4+packetSize
    uint8_t* byteArray = new uint8_t[PACKET_SIZE]; 
    std::vector<bool> v1 = generateRandomSequence();
    int i=0;
    for (const auto& elem : v1)
    {
        //LOG
        //std::cout << elem; //Everything is perfect! We create 1000 byte long random number!
        if(elem==true){     //We converting into uint_8 because of packet constructor.
          byteArray[i]=1;   
        }else{
          byteArray[i]=0;
        }        
        i++;
    }

    //LOG...
  // std::cout<<"\nPacket  : ";
  //    for(int i=0; (unsigned)i < PACKET_SIZE;++i){         
  //        std::cout << unsigned(byteArray[i]);
  // } 
  // std::cout<<std::endl; 

  //NS_LOG_UNCOND("Alper.test.UDP.CLIENT.CPP!");  

  
  std::cout << "Alper.delete.Entropy Value:" <<this->m_entropy;


  Ptr<Packet> p;
  if(this->m_entropy == true){ //Random....
    p = Create<Packet> (byteArray, PACKET_SIZE-(8+4));   //Packet (uint8_t const*buffer, uint32_t size);
  }else{
    p = Create<Packet> (m_size-(8+4)); // 8+4 : the size of the seqTs header     
  }  
  

  p->AddHeader (seqTs);

  std::stringstream peerAddressStringStream;
  if (Ipv4Address::IsMatchingType (m_peerAddress))
    {
      peerAddressStringStream << Ipv4Address::ConvertFrom (m_peerAddress);
    }
  else if (Ipv6Address::IsMatchingType (m_peerAddress))
    {
      peerAddressStringStream << Ipv6Address::ConvertFrom (m_peerAddress);
    }

  if ((m_socket->Send (p)) >= 0)
    {
      ++m_sent;
      NS_LOG_INFO ("TraceDelay TX " << m_size << " bytes to "
                                    << peerAddressStringStream.str () << " Uid: "
                                    << p->GetUid () << " Time: "
                                    << (Simulator::Now ()).GetSeconds ());

    }
  else
    {
      NS_LOG_INFO ("Error while sending " << m_size << " bytes to "
                                          << peerAddressStringStream.str ());
    }

  if (m_sent < m_count)
    {
      m_sendEvent = Simulator::Schedule (m_interval, &UdpClient::Send, this);
    }
}

void
  UdpClient::SetEntropy(bool isHighEntropy1){
    NS_LOG_FUNCTION (this);
    isHighEntropy = isHighEntropy1; 
  }



} // Namespace ns3
