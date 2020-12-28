/*
 * KerSem.cpp
 *
 *  Created on: Nov 16, 2018
 *      Author: OS1
 */

#include "KerSem.h"
#include "PCB.h"
#include "usluzni.h"
#include "Schedule.h"
#include "Kernel.h"

void dispatch();

KernelSem::KernelSem(int vr) {
	vrijednost = vr;
	inicijalno = vr;
	red_cekanja = new ListaCekanja();
}

KernelSem::~KernelSem() {
	delete red_cekanja;
}

int KernelSem::wait (int toBlock){
	//lock
	/*if(toBlock!=0){
			if(vrijednost <= 0){
				vrijednost--;
				red_cekanja->stavi((PCB*)PCB::running);
				unlock
				dispatch();
				return 1;
			}
		}
		else {
			if(vrijednost <= 0){
				unlock
				return -1;
			}
		}*/

	if(vrijednost <= 0){
		if(toBlock!=0){
			vrijednost--;
			red_cekanja->stavi((PCB*)PCB::running);
			//unlock;
			dispatch();
			return 1;
		}
		else {
			//unlock
			return -1;
		}
	}
	vrijednost--;
	//unlock
	return 0;
}

void KernelSem::signal(){
	vrijednost++;
	if(vrijednost <= 0 && red_cekanja->prvi){
		PCB* pcb = red_cekanja->uzmi();
		Scheduler::put(pcb);
		Kernel::brojNitiURasporedjivacu++;
	}
}

int KernelSem::val()const{
	return vrijednost;
}
