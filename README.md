# veins_hetvnet #

Sample combination of Veins, INET, and SimuLTE.

Integrates code from the SimuLTE `cars` simulation as well as `AlertSender` and `AlertReceiver` code into a simulation that sends packets via both WLAN and LTE.

## Supported program versions ##

- Veins 5.0-alpha2 (see <http://veins.car2x.org/>)
- OMNeT++ 5.5.1 (see <https://omnetpp.org/>)
- INET 3.6.5 (see <https://inet.omnetpp.org/>)
- SimuLTE 1.0.1 with Veins integration (git commit fa994699) (see <http://simulte.com/>)

## Supported platforms ##

- Debian 9

## Setup ##

To quickly set things up, download "Instant Veins 5.0-alpha2-i1 with SimuLTE".
Or, follow below instructions.

First, make sure that you are using compatible program versions (see above) on a compatible platform (see above).
Store the above-mentioned software in the following paths:
- `~/src/veins`
- `~/src/omnetpp`
- `~/src/inet`
- `~/src/simulte`
- `~/src/veins_hetvnet` (this software)

Import all projects into the OMNeT++ IDE, making sure to also import the `veins_inet3` subproject stored in the `subprojects/veins_inet3` subfolder of the `veins` project.
Enable the `SimuLTE_Cars` feature of the SimuLTE OMNeT++ project.

Build all projects in both debug and release mode.

## Running ##

Try out `veins_hetvnet` by running the example in `examples/veins_hetvnet/omnetpp.ini`.

If you built the project on the command line (by running `./configure` followed by `make`) you can also run it as follows:

```
cd examples/veins_hetvnet
./run -d -u Cmdenv | grep "HetVNetDemoApp::handleMessage"
```

You should see the following output:

```
...
[TRACE] General:0 Highway.car[2].udpApp[0] HetVNetDemoApp::handleMessage - Packet received from WLAN interface: SeqNo[255] Delay[0.0001300217]
[TRACE] General:0 Highway.car[1].udpApp[0] HetVNetDemoApp::handleMessage - Packet received from WLAN interface: SeqNo[255] Delay[0.000130872022]
[TRACE] General:0 Highway.car[2].udpApp[0] HetVNetDemoApp::handleMessage - Packet received from LTE! interface: SeqNo[255] Delay[0.014]
...
```

That's it!

## License ##

Veins is composed of many parts. See the version control log for a full list of
contributors and modifications. Each part is protected by its own, individual
copyright(s), but can be redistributed and/or modified under an open source
license. License terms are available at the top of each file. Parts that do not
explicitly include license text shall be assumed to be governed by the "GNU
General Public License" as published by the Free Software Foundation -- either
version 2 of the License, or (at your option) any later version
(SPDX-License-Identifier: GPL-2.0-or-later). Parts that are not source code and
do not include license text shall be assumed to allow the Creative Commons
"Attribution-ShareAlike 4.0 International License" as an additional option
(SPDX-License-Identifier: GPL-2.0-or-later OR CC-BY-SA-4.0). Full license texts
are available with the source distribution.

