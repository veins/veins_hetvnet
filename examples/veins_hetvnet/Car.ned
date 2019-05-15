//
// Copyright (C) 2012 Antonio Virdis, Daniele Migliorini, Matteo Maria Andreozzi, Giovanni Accongiagioco, Generoso Pagano, Vincenzo Pii.
// Copyright (C) 2019 Christoph Sommer <sommer@ccs-labs.org>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// SPDX-License-Identifier: LGPL-3.0-only
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation, version 3.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

//
// Based on an adapted version of the SimuLTE `cars` simulation
//

package veins_hetvnet.simulations;

import inet.applications.contract.ITCPApp;
import inet.applications.contract.IUDPApp;
import inet.linklayer.contract.IWirelessNic;
import inet.mobility.contract.IMobility;
import inet.networklayer.common.InterfaceTable;
import inet.networklayer.configurator.ipv4.HostAutoConfigurator;
import inet.networklayer.contract.INetworkLayer;
import inet.networklayer.contract.IRoutingTable;
import inet.transportlayer.tcp.TCP;
import inet.transportlayer.udp.UDP;
import lte.stack.phy.ILteNic;

module Car
{
    parameters:
        @networkNode();
        @display("bgb=400,518");

        string mobilityType = default("VeinsInetMobility");
        int numTcpApps = default(0);
        int numUdpApps = default(0);
        string nodeType = "UE";
        int masterId;
        int macNodeId = default(0);
        int macCellId = default(0);
        bool d2dCapable = default(false);
        string nicType = d2dCapable ? "LteNicUeD2D" : "LteNicUe";
        string networkLayerType = default("IPv4NetworkLayer");
        string routingTableType = default("IPv4RoutingTable");
        *.interfaceTableModule = default(absPath(".interfaceTable"));
        *.routingTableModule = default(absPath(".routingTable"));

    gates:
        input radioIn @directIn;
        input inetRadioIn @directIn;

    submodules:
        mobility: <mobilityType> like IMobility {
            @display("p=50,175;is=s");
        }
        interfaceTable: InterfaceTable {
            @display("p=50,75;is=s");
        }
        routingTable: <routingTableType> like IRoutingTable if routingTableType != "" {
            parameters:
                @display("p=50,125;is=s");
        }
        tcpApp[numTcpApps]: <> like ITCPApp {
            @display("p=175,50,row");
        }
        tcp: TCP if numTcpApps>0 {
            @display("p=175,150");
        }
        udpApp[numUdpApps]: <> like IUDPApp {
            @display("p=325,50,row");
        }
        udp: UDP if numUdpApps>0 {
            @display("p=325,150");
        }
        lteNic: <nicType> like ILteNic {
            nodeType = nodeType;
            d2dCapable = d2dCapable;
            @display("p=324.684,406.116");
        }
        networkLayer: <networkLayerType> like INetworkLayer {
            parameters:
                @display("p=250,258");
        }
        configuratorLte: HostAutoConfigurator {
            @display("p=49.068,22.968");
        }
        configuratorWlan: HostAutoConfigurator {
            @display("p=49.068,22.968");
        }
        wlan[1]: <default("Ieee80211Nic")> like IWirelessNic {
            parameters:
                @display("p=174.348,406.116,row,60;q=queue");
        }

    connections allowunconnected:

        for i=0..numTcpApps-1 {
            tcpApp[i].tcpOut --> tcp.appIn++;
            tcpApp[i].tcpIn <-- tcp.appOut++;
        }

        tcp.ipOut --> networkLayer.transportIn++ if numTcpApps>0;
        tcp.ipIn <-- networkLayer.transportOut++ if numTcpApps>0;

        for i=0..numUdpApps-1 {
            udpApp[i].udpOut --> udp.appIn++;
            udpApp[i].udpIn <-- udp.appOut++;
        }

        udp.ipOut --> networkLayer.transportIn++ if numUdpApps>0;
        udp.ipIn <-- networkLayer.transportOut++ if numUdpApps>0;

        lteNic.radioIn <-- radioIn;

        networkLayer.ifOut++ --> lteNic.upperLayerIn;
        networkLayer.ifIn++ <-- lteNic.upperLayerOut;

        inetRadioIn --> { @display("m=s"); } --> wlan[0].radioIn;
        wlan[0].upperLayerOut --> networkLayer.ifIn++;
        wlan[0].upperLayerIn <-- networkLayer.ifOut++;

}
