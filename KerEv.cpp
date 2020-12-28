/*
 * KerEv.cpp
 *
 *  Created on: Dec 20, 2018
 *      Author: OS1
 */

#include "KerEv.h"
#include "Schedule.h"
#include "Kernel.h"
#include "PCB.h"
#include "IVTEntry.h"

KernelEv::KernelEv(IVTNo ivtn) {
	nit = (PCB*)PCB::running;
	ulaz = IVTEntry::getEntry(ivtn);
	ulaz->dogadjaj=this;
}


KernelEv::~KernelEv() {
	//lock
	if(nit->blokiran){
		nit->blokiran = 0;
		Scheduler::put(nit);
		Kernel::brojNitiURasporedjivacu++;
	}
	//ulaz->dogadjaj = 0;
	delete ulaz;
	//unlock
}

void KernelEv::signal(){
	//lock
	if(nit->blokiran){
		nit->blokiran = 0;
		Scheduler::put(nit);
		Kernel::brojNitiURasporedjivacu++;
	}
	//unlock
	dispatch();
}

void KernelEv::wait(){
	//lock
	if(PCB::running == nit){//ako je running sigurno nije blokirana
		nit->blokiran = 1;
	}
	//unlock
	dispatch();
}

