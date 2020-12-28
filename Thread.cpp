/*
 * Thread.cpp
 *
 *  Created on: Nov 12, 2018
 *      Author: OS1
 */

#include "Thread.h"
#include "PCB.h"
#include "Schedule.h"
#include "Kernel.h"
#include "usluzni.h"
#include <iostream.h>


Thread::Thread(StackSize stackSize, Time timeSlice) {
	lock
	if (stackSize>65535){
		stackSize = 65535;
	}
	myPCB = new PCB(stackSize, timeSlice, this);
	unlock
}

Thread::~Thread() {
	waitToComplete();
	lock
	delete myPCB;
	unlock
}

void Thread::start(){
	lock
	if(!myPCB->aktivan){
	Kernel::brojNitiURasporedjivacu++;
	Kernel::brojAktivnihNiti++;
	Scheduler::put(myPCB);
	myPCB->aktivan=1;
	}
	unlock
}

void dispatch(){
	lock
	Kernel::zahtjevana_promjena_konteksta=1;
	asm int 8h
	unlock

}

void Thread::sleep(Time timeToSleep){
	lock
	Kernel::SLista->stavi((PCB*)PCB::running, timeToSleep);
	unlock
	dispatch();
}

void Thread::waitToComplete(){
	if(!this->myPCB->zavrsio){
	lock
	this->myPCB->lista_cekanja->stavi((PCB*)PCB::running);
	unlock
	dispatch();
	}
}

ID Thread::getId(){
	return myPCB->tID;
}
