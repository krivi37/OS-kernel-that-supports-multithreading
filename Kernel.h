/*
 * Kernel.h
 *
 *  Created on: Nov 12, 2018
 *      Author: OS1
 */

#ifndef KERNEL_H_
#define KERNEL_H_

#include "usluzni.h"

class PCB;

class Kernel {
public:
	static volatile int return_vr;
	static pInterrupt staraRutina;
	static PCB* pocetni;
	static PCB* idle_nit;
	static SleepLista* SLista;
	static unsigned lockFlag, brojac, zahtjevana_promjena_konteksta;
	static volatile unsigned brojAktivnihNiti;
	static volatile unsigned brojNitiURasporedjivacu;
	static void init();
	static void interrupt timer(...);
	static void restore();
};

#endif /* KERNEL_H_ */
