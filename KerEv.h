/*
 * KerEv.h
 *
 *  Created on: Dec 20, 2018
 *      Author: OS1
 */

#ifndef KEREV_H_
#define KEREV_H_

#include "event.h"

class IVTEntry;
class PCB;

class KernelEv {
public:

	IVTEntry* ulaz;
	PCB* nit;
	KernelEv(IVTNo ivtn);
	~KernelEv();
	void signal();
	void wait();
};

#endif /* KEREV_H_ */
