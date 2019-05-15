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
// Based on an adapted version of the SimuLTE `AlertSender` and `AlertReceiver` simulation modules
//

#include <cmath>

#include "HetVNetDemoApp.h"

#include "inet/common/ModuleAccess.h"

Define_Module(HetVNetDemoApp);

HetVNetDemoApp::HetVNetDemoApp()
{
    sendPacket = nullptr;
}

HetVNetDemoApp::~HetVNetDemoApp()
{
    cancelAndDelete(sendPacket);
}

void HetVNetDemoApp::initialize(int stage)
{
    EV_TRACE << "HetVNetDemoApp::initialize - stage " << stage << std::endl;

    cSimpleModule::initialize(stage);

    if (stage != inet::INITSTAGE_APPLICATION_LAYER) return;

    packetSizeBytes = par("packetSize");
    packetInterval = par("period");
    localPortLte = par("localPortLte");
    localPortWlan = par("localPortWlan");
    destAddressLte_ = inet::L3AddressResolver().resolve(par("destAddressLte").stringValue());
    destAddressWlan = inet::L3AddressResolver().resolve(par("destAddressWlan").stringValue());
    simtime_t startTime = par("startTime");

    sigDemoPktSent = registerSignal("demoPktSent");
    sigDemoPktRcvDelay = registerSignal("demoPktRcvDelay");
    sigDemoPktRcvd = registerSignal("demoPktRcvd");

    nextSequenceNumber = 0;

    {
        socketLte.setOutputGate(gate("udpOut"));
        socketLte.bind(localPortLte);

        inet::IInterfaceTable* ift = inet::getModuleFromPar<inet::IInterfaceTable>(par("interfaceTableModule"), this);
        inet::MulticastGroupList mgl = ift->collectMulticastGroups();
        socketLte.joinLocalMulticastGroups(mgl);
        inet::InterfaceEntry* ie = ift->getInterfaceByName("wlan");
        if (!ie) throw cRuntimeError("Wrong multicastInterface setting: no interface named wlan");
        socketLte.setMulticastOutputInterface(ie->getInterfaceId());
    }

    {
        socketWlan.setOutputGate(gate("udpOut"));
        socketWlan.bind(localPortWlan);

        inet::IInterfaceTable* ift = inet::getModuleFromPar<inet::IInterfaceTable>(par("interfaceTableModule"), this);
        inet::MulticastGroupList mgl = ift->collectMulticastGroups();
        socketWlan.joinLocalMulticastGroups(mgl);
        inet::InterfaceEntry* ie = ift->getInterfaceByName("wlan0");
        if (!ie) throw cRuntimeError("Wrong multicastInterface setting: no interface named wlan0");
        socketWlan.setMulticastOutputInterface(ie->getInterfaceId());
    }

    if (startTime > 0) {
        sendPacket = new cMessage("sendPacket");
        scheduleAt(simTime(), sendPacket);
    }
}

void HetVNetDemoApp::handleMessage(cMessage* msg)
{
    if (msg->isSelfMessage()) {
        if (msg == sendPacket) {
            sendHetVNetDemoPacket();
            return;
        }
        else {
            throw cRuntimeError("Unknown self message");
        }
    }

    HetVNetDemoPacket* pkt = check_and_cast<HetVNetDemoPacket*>(msg);
    if (pkt == 0) {
        throw cRuntimeError("Unknown packet type");
    }

    simtime_t delay = simTime() - pkt->getCreationTime();

    emit(sigDemoPktRcvDelay, delay);
    emit(sigDemoPktRcvd, (long) 1);

    EV_TRACE << "Packet received from " << (pkt->getIsWlan() ? "WLAN" : "LTE!") << " interface: SeqNo[" << pkt->getSequenceNo() << "] Delay[" << delay << "]" << std::endl;

    delete msg;
}

void HetVNetDemoApp::sendHetVNetDemoPacket()
{
    {
        HetVNetDemoPacket* packet = new HetVNetDemoPacket("Lte Demo Packet");
        packet->setIsWlan(0);
        packet->setSequenceNo(nextSequenceNumber);
        packet->setCreationTime(simTime());
        packet->setByteLength(packetSizeBytes);
        EV_TRACE << "Sending message [" << nextSequenceNumber << "] via LTE!" << std::endl;

        socketLte.sendTo(packet, destAddressLte_, localPortLte);

        emit(sigDemoPktSent, (long) 1);
    }

    {
        HetVNetDemoPacket* packet = new HetVNetDemoPacket("Wlan Demo Packet");
        packet->setIsWlan(1);
        packet->setSequenceNo(nextSequenceNumber);
        packet->setCreationTime(simTime());
        packet->setByteLength(packetSizeBytes);
        EV_TRACE << "Sending message [" << nextSequenceNumber << "] via WLAN" << std::endl;

        socketWlan.sendTo(packet, destAddressWlan, localPortWlan);

        emit(sigDemoPktSent, (long) 1);
    }

    nextSequenceNumber++;

    scheduleAt(simTime() + packetInterval, sendPacket);
}
