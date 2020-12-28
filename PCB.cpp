/*
 * PCB.cpp
 *
 *  Created on: Nov 12, 2018
 *      Author: OS1
 */

#include "PCB.h"
#include <dos.h>
#include "Kernel.h"
#include "usluzni.h"
#include <iostream.h>

volatile PCB* PCB::running=0;
unsigned PCB::autoID=0;


void PCB::wrapper(){
	PCB::running->myThread->run();
	lock
	PCB::running->zavrsio=1;
	PCB::running->aktivan=0;
	Kernel::brojAktivnihNiti--;
	if(PCB::running->lista_cekanja->prvi)
	PCB::running->lista_cekanja->isprazni();
	unlock
	dispatch();
}

PCB::PCB(StackSize stackSize, Time timeSlice, Thread* t) {
	zavrsio = 0;
	tID = ++PCB::autoID;
	myThread = t;
	spava = ceka = kvant_spavanja = aktivan = blokiran = 0;
	kvant = timeSlice;
	if (timeSlice){
		neogranicen = 0;
	}
	else neogranicen=1;
	lista_cekanja=new ListaCekanja();
	if(stackSize){
		unsigned vel = stackSize/sizeof(unsigned);
		stack = new unsigned[vel];
		stack[vel-1]=0x200;
		#ifndef BCC_BLOCK_IGNORE
		stack[vel-2]=FP_SEG(PCB::wrapper);
		stack[vel-3]=FP_OFF(PCB::wrapper);
		ss = FP_SEG(stack+(vel-12));
		sp = FP_OFF(stack+(vel-12));
		bp = sp;
		#endif
	}

}


PCB::~PCB() {
	lock
	delete []stack;
	delete lista_cekanja;
	unlock
}

