#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"


using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Point to point conection");

int 
main (int argc, char *argv[])
{

  NS_LOG_INFO ("Command Line Argument!");
  CommandLine cmd;
  cmd.AddValue ("useIpv6", "Use Ipv6", useV6);
  cmd.Parse (argc, argv);

  NS_LOG_INFO ("Decleare variable!");
  bool useV6 = false;
  Address serverAddress;
  uint16_t port = 9;  
  uint32_t responseSize = 1100;
  uint32_t packetSize = 1100;
  uint32_t maxPacketCount = 2;
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
  PointToPointHelper pointToPoint;
  /// pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
  /// pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));
  pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));
  pointToPoint.SetDeviceAttribute ("Mtu", UintegerValue (1100));

  NS_LOG_INFO ("connect nodes using DeviceContainer!");
  NetDeviceContainer devices;
  devices = pointToPoint.Install(nodes);

 // NetDeviceContainer deviceNode1_2;
 // deviceNode1_2 = pointToPoint.Install(nodes);


  NS_LOG_INFO ("Assign IP Addresses!");
  if (useV6 == false)
    {
      Ipv4AddressHelper ipv4Address;
      ipv4Address.SetBase ("10.1.1.0", "255.255.255.0");
      Ipv4InterfaceContainer interfaces = ipv4Address.Assign (devices);
      serverAddress = Address(interfaces.GetAddress (1));
    }
  else
    {
      Ipv6AddressHelper ipv6Address;
      ipv6Address.SetBase ("2001:0000:f00d:cafe::", Ipv6Prefix (64));
      Ipv6InterfaceContainer interfaces = ipv6Address.Assign (devices);
      serverAddress = Address(interfaces.GetAddress (1,1));
    }

  // assign IP to nodes and generate interface from node and IP
  ///Ipv4AddressHelper address;
  ///address.SetBase("10.1.1.0", "255.255.255.0");
  ///Ipv4InterfaceContainer interfaces = address.Assign (devices);

  NS_LOG_INFO ("Create Server Applications!");
  UdpEchoServerHelper echoServer (port);
  echoServer.SetAttribute ("PacketSize", UintegerValue (responseSize));
  ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  // set server
  ///UdpEchoServerHelper echoServer(9);
  // create application on top of server
  ///ApplicationContainer serverApps = echoServer.Install(nodes.Get(1));
  ///serverApps.Start(Seconds(1.0));
  ///serverApps.Stop(Seconds(10.0));

  NS_LOG_INFO ("Create Server Applications!");
  UdpEchoClientHelper echoClient (serverAddress, port);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  echoClient.SetAttribute ("Interval", TimeValue (interPacketInterval));
  echoClient.SetAttribute ("PacketSize", UintegerValue (packetSize));
 
  ApplicationContainer clientApps = echoClient.Install(nodes.Get(0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));


  // create interface btw client and server, set attribute
  ///UdpEchoClientHelper echoClient(interfaces.GetAddress(1), 9);
  ///echoClient.SetAttribute("MaxPackets", UintegerValue(1));
  ///echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
  ///echoClient.SetAttribute("PacketSize", UintegerValue(1024));

  // create application on top of client
  ///ApplicationContainer clientApps = echoClient.Install(nodes.Get(0));
  ///clientApps.Start(Seconds(2.0));
  ///clientApps.Stop(Seconds(10.0));

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}











 

 
