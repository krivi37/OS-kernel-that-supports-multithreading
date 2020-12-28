/*
 * event.cpp
 *
 *  Created on: Dec 20, 2018
 *      Author: OS1
 */

#include "event.h"
#include "usluzni.h"
#include "KerEv.h"

Event::Event(IVTNo ivtNo) {
	lock
	myImpl = new KernelEv(ivtNo);
	unlock

}

Event::~Event() {
	lock
	delete myImpl;
	unlock
}

void Event::signal(){//mozda bude potrebno zakljucavanje
	lock
	myImpl->signal();
	unlock
}

void Event::wait(){
	lock
	myImpl->wait();
	unlock
}
