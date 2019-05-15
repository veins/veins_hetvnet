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

#pragma once

#include <string.h>
#include <omnetpp.h>

#include "inet/transportlayer/contract/udp/UDPSocket.h"
#include "inet/networklayer/common/L3Address.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "HetVNetDemoPacket_m.h"

class HetVNetDemoApp : public cSimpleModule {
    inet::UDPSocket socketLte;
    inet::UDPSocket socketWlan;

    int nextSequenceNumber;
    int packetSizeBytes;
    simtime_t packetInterval;

    simsignal_t sigDemoPktSent;

    cMessage* sendPacket;

    int localPortLte;
    int localPortWlan;
    inet::L3Address destAddressLte_;
    inet::L3Address destAddressWlan;

    void sendHetVNetDemoPacket();

    simsignal_t sigDemoPktRcvDelay;
    simsignal_t sigDemoPktRcvd;

public:
    ~HetVNetDemoApp();
    HetVNetDemoApp();

protected:
    virtual int numInitStages() const
    {
        return inet::NUM_INIT_STAGES;
    }
    void initialize(int stage);
    void handleMessage(cMessage* msg);
};
