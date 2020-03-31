/****************************************************************************
 *
 *   Copyright (c) 2020 ThunderFly s.r.o.. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file fg_communicator.cpp
 *
 * @author ThunderFly s.r.o., Vít Hanousek <info@thunderfly.cz>
 * @url https://github.com/ThunderFly-aerospace
 *
 * FlightGear and PX connection manager.
 */

#include <iostream>
#include <cstdlib>
#include <cstring>

#include <time.h>

#include "px4_communicator.h"
#include "fg_communicator.h"
#include "vehicle_state.h"


using namespace std;

int main(int argc, char **argv)
{
	cerr << "I'm Mavlink to FlightGear Bridge" << endl;;

	int delay_us = 5000;
	bool havePxData = false;
	bool haveFGData = false;
	bool sendEveryStep = true;

	cerr << "Targed Bridge Freq: " << 1000000.0 / delay_us << ", send data every step: " << sendEveryStep << std::endl;

	//parse parameters
	if (argc < 2) {
		cerr << "Use: bridge ControlCount ControlIndex0 ControlP0 ControlIndex1 ControlP1 ..." << endl;
	}

	int controlsCount = atoi(argv[1]);

	int *contolsMap = new int[controlsCount];
	double *controlsP = new double[controlsCount];

	for (int i = 0; i < controlsCount; i++) {
		contolsMap[i] = atoi(argv[2 + 2 * i]);
		controlsP[i] = atof(argv[2 + 2 * i + 1]);
	}

	cout << controlsCount << endl;

	for (int i = 0; i < controlsCount; i++) {
		cout << "  " << contolsMap[i] <<  "   " << controlsP[i] << endl;
	}

	VehicleState vehicle(controlsCount, contolsMap, controlsP);
	PX4Communicator px4(&vehicle);
	FGCommunicator fg(&vehicle);

	if (px4.Init() != 0) {
		cerr << "Unable to Init PX4 Communication" << endl;
		return -1;
	}

	if (fg.Init() != 0) {
		cerr << "Unable to Init FG Communication" << endl;
		return -1;
	}

	while (1) {

		bool fgRecved = (fg.Recieve(false) == 1);

		if (fgRecved) {
			haveFGData = true;
		}

		if (fgRecved || (haveFGData && sendEveryStep)) {
			px4.Send();
		}

		bool px4Recved = (px4.Recieve(false) == 1);

		if (px4Recved) {
			havePxData = true;
		}

		if (px4Recved || (havePxData && sendEveryStep)) {
			fg.Send();
		}

		usleep(delay_us);
	}


	cerr << "Bridge Exiting" << endl;
	fg.Clean();
	px4.Clean();

	delete [] contolsMap;
	delete [] controlsP;

}
