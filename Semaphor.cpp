/*
 * Sem.cpp
 *
 *  Created on: Nov 16, 2018
 *      Author: OS1
 */

#include "Semaphor.h"
#include "Kernel.h"
#include "usluzni.h"
#include "PCB.h"
#include "KerSem.h"

Semaphore::Semaphore(int init) {
	lock
	myImpl = new KernelSem(init);
	unlock
}

Semaphore::~Semaphore() {
	lock
	delete myImpl;
	unlock
}

int Semaphore::wait(int toBlock){
	lock
	int r = myImpl->wait(toBlock);
	unlock
	return r;
}

void Semaphore::signal(){
	lock
	myImpl->signal();
	unlock
}

int Semaphore::val()const{
	return myImpl->val();
}
