/*
 * PCB.h
 *
 *  Created on: Nov 12, 2018
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_
#include "Thread.h"

class Thread;
class ListaCekanja;

class PCB {
public:
	unsigned ss, sp, bp, kvant, tID, kvant_spavanja, aktivan;
	unsigned* stack;
	ListaCekanja* lista_cekanja;

	//flegovi
	unsigned zavrsio, spava, ceka, neogranicen, blokiran;
	Thread* myThread;

	static unsigned autoID;
	static volatile PCB* running;
	static void wrapper();

	PCB(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice, Thread* t);
	~PCB();
};

#endif /* PCB_H_ */
