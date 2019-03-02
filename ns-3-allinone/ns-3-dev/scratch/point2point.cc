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

NS_LOG_COMPONENT_DEFINE("Point to point connection"); 

static const std::string CONFIG_FILE = "config.txt";
//static int UDP_PACKET_COUNT = 3;  
uint32_t MAX_PACKET_COUNT = 2;    
static uint32_t MTU_SIZE = 2000; 
static uint32_t PACKET_SIZE = 1100; //TODO: This is for 0. Low enthropy. 

uint16_t serverPort = 9;

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

int main (int argc, char *argv[])
{
  /* READ COMMAND LINE ARGUMENTS  */
  uint32_t CompressionDataRate = 1;
  bool IsHighEntropy = 0; 
  CommandLine cmd;
  cmd.AddValue("CompressionDataRate", "CompressionDataRate [Mbps]", CompressionDataRate);
  cmd.AddValue("IsHighEntropy", "IsHighEntropy [0/1]", IsHighEntropy); 
  cmd.Parse (argc, argv);
  NS_LOG_UNCOND("Compression Link Data Rate:"<< CompressionDataRate);




    
  /* READ CONFIGURATION FILE */
  int  protocolNumberInDecimal = readConfigurationFile();
  
  Time interPacketInterval = Seconds (0.1);
  NS_LOG_INFO ("Report Timing!");
  Time::SetResolution (Time::NS);
  //LogComponentEnable("UdpClientApplication", LOG_LEVEL_INFO);
  //LogComponentEnable("UdpServerApplication", LOG_LEVEL_INFO);

  NS_LOG_INFO ("Create Node!");
  NodeContainer nodes;
  nodes.Create(4);
  
  InternetStackHelper stack;
  stack.Install(nodes);

  /* Link btw Sender Router1 */
  PointToPointHelper P2PSenderRouter1;
  P2PSenderRouter1.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
  P2PSenderRouter1.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (1)));
  P2PSenderRouter1.SetDeviceAttribute ("Mtu", UintegerValue (MTU_SIZE));
  
  std::string dataRateString = std::to_string(CompressionDataRate);
  dataRateString = dataRateString +"Mbps";
  NS_LOG_UNCOND("dataRateString:"<< dataRateString);  

  /* Link btw Router1 Router2 */
  PointToPointHelper P2PRouter1Router2; 
  P2PRouter1Router2.SetDeviceAttribute("DataRate", StringValue(dataRateString)); 
  P2PRouter1Router2.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (1)));
  P2PRouter1Router2.SetDeviceAttribute ("Mtu", UintegerValue (MTU_SIZE));

  /* Link btw Router2 Receiver */
  PointToPointHelper P2PRouter2Receiver;
  P2PRouter2Receiver.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
  P2PRouter2Receiver.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (1)));
  P2PRouter2Receiver.SetDeviceAttribute ("Mtu", UintegerValue (MTU_SIZE));

  /* Connect node Sender & Router1 */
  NetDeviceContainer deviceSenderRouter1; 
  deviceSenderRouter1 = P2PSenderRouter1.Install(nodes.Get(0),nodes.Get(1));

  /* Connect node Router1 & Router2 */
  NetDeviceContainer deviceRouter1Router2; 
  deviceRouter1Router2 = P2PRouter1Router2.Install(nodes.Get(1),nodes.Get(2));
  
  Ptr <PointToPointNetDevice> PpNdRouter1Router2 = DynamicCast<PointToPointNetDevice> (deviceRouter1Router2.Get(0));
  Ptr <PointToPointNetDevice> PpNdRouter2Sender = DynamicCast<PointToPointNetDevice> (deviceRouter1Router2.Get(1));  
  PpNdRouter1Router2 -> SetCompressFlag(false);
  PpNdRouter1Router2 -> SetCompressProtocolNumber(protocolNumberInDecimal); 
  PpNdRouter1Router2 -> SetDecompressFlag(false); 
     
  /* Connect node Router2 & Receiver */  
  NetDeviceContainer deviceRouter2Receiver; 
  deviceRouter2Receiver = P2PRouter2Receiver.Install(nodes.Get(2),nodes.Get(3));

  NS_LOG_INFO ("Assign IP Addresses!");  
  Ipv4AddressHelper ipv4Address;
  /* Assign IP to SenderRouter1 */  
  ipv4Address.SetBase ("10.0.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaceSenderRouter1;
  interfaceSenderRouter1 = ipv4Address.Assign (deviceSenderRouter1);
 

  /* Assign IP to Router1Router2 */  
  ipv4Address.SetBase ("10.0.2.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaceRouter1Router2;
  interfaceRouter1Router2= ipv4Address.Assign (deviceRouter1Router2);
 
  /* Assign IP to Router2Receiver */
  ipv4Address.SetBase ("10.0.3.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaceRouter2Receiver;
  interfaceRouter2Receiver= ipv4Address.Assign(deviceRouter2Receiver);
  Address serverAddress;
  serverAddress = Address(interfaceRouter2Receiver.GetAddress(1));

  NS_LOG_INFO ("Create Server Application!");

  /* Create Server */  
  UdpServerHelper server (serverPort);
  ApplicationContainer serverApps = server.Install (nodes.Get(3));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (5000.0));       

  UdpClientHelper client (serverAddress, serverPort);
  client.SetAttribute ("MaxPackets", UintegerValue (MAX_PACKET_COUNT)); 
  client.SetAttribute ("Interval", TimeValue (interPacketInterval));   
  client.SetAttribute ("PacketSize", UintegerValue (PACKET_SIZE));
  
  ApplicationContainer clientApps = client.Install (nodes.Get(0));
  //Start counter...
  clientApps.Start (Seconds (4.0));
  clientApps.Stop (Seconds (4000.0));
    //End Counter
    //Calculate..... <100 Compression.

    //GENERATE RANDOM PAYLOAD
    //generateRandomUDPPayload();

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  #if 1
  AsciiTraceHelper ascii;
  P2PSenderRouter1.EnableAsciiAll(ascii.CreateFileStream("Sender.tr"));
  P2PSenderRouter1.EnablePcap("Sender", deviceSenderRouter1.Get(0),false, false);

  P2PRouter1Router2.EnableAsciiAll(ascii.CreateFileStream("Router1.tr"));
  P2PRouter1Router2.EnablePcap("Router1", deviceRouter1Router2.Get(0),false, false);

  P2PRouter1Router2.EnableAsciiAll(ascii.CreateFileStream("Router2.tr"));
  P2PRouter1Router2.EnablePcap("Router2", deviceRouter2Receiver.Get(0),false, false);

  P2PRouter2Receiver.EnableAsciiAll(ascii.CreateFileStream("Receiver.tr"));
  P2PRouter2Receiver.EnablePcap("Receiver", deviceRouter2Receiver.Get(1),false, false);
  #elif 0 
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