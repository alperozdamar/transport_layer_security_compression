#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"
#include <iostream>
#include <fstream>
#include <iomanip> 


using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Point to point conection");

static const std::string CONFIG_FILE = "config.txt";
static uint8_t* byteArray;
static uint32_t PACKET_SIZE = 1100; 
static int UDP_PACKET_COUNT = 1;  

/**
 * 
 * Reading Configuration File...
 * 
 * */
int readConfigurationFile(){
  int  protocolNumberInDecimal=0;
  std::ifstream cFile (CONFIG_FILE);
      if (cFile.is_open())
      {
          std::string line;
          while(getline(cFile, line)){
              line.erase(std::remove_if(line.begin(), line.end(), isspace),
                                  line.end());
              if(line[0] == '#' || line.empty())
                  continue;
              auto delimiterPos = line.find("=");
              std::string name = line.substr(0, delimiterPos);
              std::string value = line.substr(delimiterPos + 1);            
              std::cout << name << " " << value << '\n';

              if(name.compare("protocolNumberInDecimal")==0 ){
                int intValue = atoi(value.c_str());
                protocolNumberInDecimal=intValue;              
                std::cout << name << "=" << protocolNumberInDecimal << '\n';
                //std::cout << "Alper.Hex:" << std::hex << protocolNumberInDecimal;
              }else{
                NS_LOG_UNCOND("protocolNumberInDecimal is not equal to "<< name);
              }
          }        
      }
      else {
          std::cerr << "Couldn't open config file for reading.\n";
      }

      return protocolNumberInDecimal;
}

void returnarray(uint8_t* array, int size)
{
    for (int i = 0; i < size; i++)
    {        
        std::cout << array[i];
    }
    std::cout << "\n";
}

uint8_t* assignzero(int size)
{
    uint8_t* b = new uint8_t[size];
    for (int i = 0; i < size; i++)
    {
        b[i] = false;
    }
    return b;
}

void generate()
{    
    int t = rand() % PACKET_SIZE;
    byteArray = assignzero(PACKET_SIZE);
    for (int x = 0; x < t; x++)
    {
        byteArray[rand() % PACKET_SIZE] = true;
    }
    //returnarray(byteArray, PACKET_SIZE);
}


void generateRandomUDPPayload(){
  long inc=0; 
  srand(time(0));       
  while (inc < UDP_PACKET_COUNT)
  {
    generate();
    inc += 1;
    NS_LOG_UNCOND("Alper.Random.Packet:"); 
    returnarray(byteArray,PACKET_SIZE);
  } 
}

int 
main (int argc, char *argv[])
{

/////// READ COMMAND LINE ARGUMENTS  ///////////
uint32_t CompressionDataRate = 8;
CommandLine cmd;
cmd.AddValue("CompressionDataRate", "CompressionDataRate [Mbps]", CompressionDataRate);
cmd.Parse (argc, argv);
NS_LOG_UNCOND("Compression Link Data Rate:"<< CompressionDataRate);
    

/////// READ CONFIGURATION FILE ///////////
int  protocolNumberInDecimal = readConfigurationFile();

 // NS_LOG_INFO ("Command Line Argument!");
 // CommandLine cmd;
 // cmd.AddValue ("useIpv6", "Use Ipv6", useV6);
 // cmd.Parse (argc, argv);

  NS_LOG_INFO ("Decleare variable!");
 uint16_t router1Port = 9;
 uint32_t packetSize = 1100;
 uint32_t maxPacketCount = 1;
 Time interPacketInterval = Seconds (0);

 NS_LOG_INFO ("Report Timing!");
 Time::SetResolution (Time::NS);
 LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
 LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

NS_LOG_INFO ("Create Node!");
NodeContainer nodes;
 nodes.Create(4);

 InternetStackHelper stack;
 stack.Install(nodes);

 NS_LOG_INFO ("Create link between nodes and set attribute!");

  /* Link btw Sender Router1 */
  PointToPointHelper P2PSenderRouter1;
  P2PSenderRouter1.SetDeviceAttribute("DataRate", StringValue("8Mbps"));
  P2PSenderRouter1.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (1)));
  P2PSenderRouter1.SetDeviceAttribute ("Mtu", UintegerValue (1100));
  
  /* Link btw Router1 Router2 */
  PointToPointHelper P2PRouter1Router2;
  P2PRouter1Router2.SetDeviceAttribute("DataRate", StringValue("8Mbps"));
  P2PRouter1Router2.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (1)));
  P2PRouter1Router2.SetDeviceAttribute ("Mtu", UintegerValue (1100));

  /* Link btw Router2 Receiver */
  PointToPointHelper P2PRouter2Receiver;
  P2PRouter2Receiver.SetDeviceAttribute("DataRate", StringValue("8Mbps"));
  P2PRouter2Receiver.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (1)));
  P2PRouter2Receiver.SetDeviceAttribute ("Mtu", UintegerValue (1100));


  NS_LOG_INFO ("connect nodes using DeviceContainer!");
  /* Connect node Sender & Router1 */
  NetDeviceContainer deviceSenderRouter1; 
  deviceSenderRouter1 = P2PSenderRouter1.Install(nodes.Get(0),nodes.Get(1));

  /* Connect node Router1 & Router2 */
  NetDeviceContainer deviceRouter1Router2; 
  deviceRouter1Router2 = P2PRouter1Router2.Install(nodes.Get(1),nodes.Get(2));
  
  Ptr <PointToPointNetDevice> PpNdRouter1Router2 = DynamicCast<PointToPointNetDevice> (deviceRouter1Router2.Get(0));
  PpNdRouter1Router2 -> SetCompressFlag(true);
  PpNdRouter1Router2 -> SetCompressProtocolNumber(protocolNumberInDecimal); 
  Ptr <PointToPointNetDevice> PpNdRouter2Sender = DynamicCast<PointToPointNetDevice> (deviceRouter1Router2.Get(1));  
  PpNdRouter1Router2 -> SetDecompressFlag(true);
  PpNdRouter1Router2 -> SetCompressProtocolNumber(protocolNumberInDecimal); 

  /* Connect node Router2 & Receiver */  
  NetDeviceContainer deviceRouter2Receiver; 
  deviceRouter2Receiver = P2PRouter2Receiver.Install(nodes.Get(2),nodes.Get(3));

  NS_LOG_INFO ("Assign IP Addresses!");  
  Ipv4AddressHelper ipv4Address;
  /* Assign IP to SenderRouter1 */  
  ipv4Address.SetBase ("10.0.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaceSenderRouter1;
  interfaceSenderRouter1 = ipv4Address.Assign (deviceSenderRouter1);
  Address SenderRouter1Address;
  SenderRouter1Address = Address(interfaceSenderRouter1.GetAddress (1));

  /* Assign IP to Router1Router2 */  
  ipv4Address.SetBase ("10.0.2.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaceRouter1Router2;
  interfaceRouter1Router2= ipv4Address.Assign (deviceRouter1Router2);
  Address Router1Router2Address;
  Router1Router2Address = Address(interfaceRouter1Router2.GetAddress (1));

  /* Assign IP to Router2Receiver */
  ipv4Address.SetBase ("10.0.3.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaceRouter2Receiver;
  interfaceRouter2Receiver= ipv4Address.Assign (deviceRouter2Receiver);
  Address Router2ReceiverAddress;
  Router2ReceiverAddress = Address(interfaceRouter2Receiver.GetAddress (1));

  NS_LOG_INFO ("Create Server Applications!");

  /* Create Server */
  UdpEchoServerHelper echoReceiverServer (router1Port);
  ApplicationContainer serverApps = echoReceiverServer.Install (nodes.Get (3));
  serverApps.Start (Seconds (2.0));
  serverApps.Stop (Seconds (100.0));
  UdpEchoClientHelper echoSenderClient (Router2ReceiverAddress, router1Port);
  //UdpEchoClientHelper echoSenderClient (SenderRouter2Address, router1Port);
  echoSenderClient.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  echoSenderClient.SetAttribute ("Interval", TimeValue (interPacketInterval));
  echoSenderClient.SetAttribute ("PacketSize", UintegerValue (packetSize));
  ApplicationContainer senderClientApps = echoSenderClient.Install(nodes.Get(0));
  
  //GENERATE RANDOM PAYLOAD
  generateRandomUDPPayload();

  //void SetFill (Ptr<Application> app, uint8_t *fill, uint32_t fillLength, uint32_t dataLength);
  //echoSenderClient.SetFill(senderClientApps.Get(0),"We can send message from here"); //1100    
  echoSenderClient.SetFill(senderClientApps.Get(0),byteArray,PACKET_SIZE,PACKET_SIZE); //1100  
  senderClientApps.Start (Seconds (4.0)); 
  senderClientApps.Stop (Seconds (90.0)); 


  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  #if 1
  AsciiTraceHelper ascii;
  P2PSenderRouter1.EnableAsciiAll(ascii.CreateFileStream("Sender.tr"));
  P2PSenderRouter1.EnablePcap("Sender", deviceSenderRouter1.Get(0),false, false);

  P2PRouter1Router2.EnableAsciiAll(ascii.CreateFileStream("Router1.tr"));
  P2PRouter1Router2.EnablePcap("Router1", deviceRouter1Router2.Get(0),false, false);

  P2PRouter1Router2.EnableAsciiAll(ascii.CreateFileStream("Router2.tr"));
  P2PRouter1Router2.EnablePcap("Router2", deviceRouter1Router2.Get(1),false, false);

  P2PRouter2Receiver.EnableAsciiAll(ascii.CreateFileStream("Receiver.tr"));
  P2PRouter2Receiver.EnablePcap("Receiver", deviceRouter2Receiver.Get(1),false, false);
  #elif
  P2PSenderRouter1.EnablePcapAll ("SenderRouter1");
  P2PRouter1Router2.EnablePcapAll ("Router1Router2");
  P2PRouter2Receiver.EnablePcapAll ("Router2Receiver");
  #endif

  AnimationInterface anim("p2p.xml");
  anim.SetConstantPosition(nodes.Get(0), 0.0, 0.0);
  anim.SetConstantPosition(nodes.Get(1), 10.0, 10.0);
  anim.SetConstantPosition(nodes.Get(2), 20.0, 20.0);
  anim.SetConstantPosition(nodes.Get(3), 30.0, 30.0);

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}