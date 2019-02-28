#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include <fstream>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Point to point conection");

int 
main (int argc, char *argv[])
{

uint32_t CompressionDataRate = 8;
CommandLine cmd;
cmd.AddValue("CompressionDataRate", "CompressionDataRate [Mbps]", CompressionDataRate);
cmd.Parse (argc, argv);


NS_LOG_UNCOND("Compression Link Data Rate:"<< CompressionDataRate);




 // NS_LOG_INFO ("Command Line Argument!");
 // CommandLine cmd;
 // cmd.AddValue ("useIpv6", "Use Ipv6", useV6);
 // cmd.Parse (argc, argv);

  NS_LOG_INFO ("Decleare variable!");
  uint16_t router1Port = 9; 

 // uint32_t responseSize = 1100;
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
  

  std::string dataRateString = std::to_string(CompressionDataRate);
  dataRateString = dataRateString +"Mbps";
  NS_LOG_UNCOND("dataRateString:"<< dataRateString);

  /* Link btw Router1 Router2 */
  PointToPointHelper P2PRouter1Router2;
  P2PRouter1Router2.SetDeviceAttribute("DataRate", StringValue(dataRateString)); 
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
  Ptr <PointToPointNetDevice> PpNdRouter2Sender = DynamicCast<PointToPointNetDevice> (deviceRouter1Router2.Get(1));  
  PpNdRouter1Router2 -> SetDecompressFlag(true);


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
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (100.0));
UdpEchoClientHelper echoSenderClient (Router2ReceiverAddress, router1Port);
  //UdpEchoClientHelper echoSenderClient (SenderRouter2Address, router1Port);
  echoSenderClient.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  echoSenderClient.SetAttribute ("Interval", TimeValue (interPacketInterval));
  echoSenderClient.SetAttribute ("PacketSize", UintegerValue (packetSize));
  ApplicationContainer senderClientApps = echoSenderClient.Install(nodes.Get(0));
  echoSenderClient.SetFill(senderClientApps.Get(0),"We can send message from here");
  senderClientApps.Start (Seconds (2.0));
  senderClientApps.Stop (Seconds (90.0));


  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  AsciiTraceHelper ascii;
  P2PSenderRouter1.EnableAsciiAll(ascii.CreateFileStream("Sender.tr"));
  P2PSenderRouter1.EnablePcap("Sender", deviceSenderRouter1.Get(0),false, false);

  P2PRouter1Router2.EnableAsciiAll(ascii.CreateFileStream("Router1.tr"));
  P2PRouter1Router2.EnablePcap("Router1", deviceRouter1Router2.Get(0),false, false);

  P2PRouter2Receiver.EnableAsciiAll(ascii.CreateFileStream("Router2.tr"));
  P2PRouter2Receiver.EnablePcap("Router2", deviceRouter2Receiver.Get(0),false, false);

  P2PRouter2Receiver.EnableAsciiAll(ascii.CreateFileStream("Receiver.tr"));
  P2PRouter2Receiver.EnablePcap("Receiver", deviceRouter2Receiver.Get(1),false, false);

  //P2PSenderRouter1.EnablePcapAll ("SenderRouter1");
  //P2PRouter1Router2.EnablePcapAll ("Router1Router2");
  //P2PRouter2Receiver.EnablePcapAll ("Receiver");

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}










 

 
