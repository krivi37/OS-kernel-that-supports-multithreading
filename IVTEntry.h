/*
 * IVTEntry.h
 *
 *  Created on: Dec 20, 2018
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "event.h"
#include "usluzni.h"

class KernelEv;
class PCB;

class IVTEntry {
public:
	IVTNo ulaz;
	KernelEv* dogadjaj;
	pInterrupt staraRutina;
	unsigned pozovi_staru;

	static IVTEntry* tabela[256];
	static IVTEntry* getEntry(IVTNo ivtn);

	void old();
	void signal();
	void wait();
	IVTEntry(IVTNo ivtn, pInterrupt nova);
	~IVTEntry();
};

#endif /* IVTENTRY_H_ */
