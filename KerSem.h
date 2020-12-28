/*
 * KerSem.h
 *
 *  Created on: Nov 16, 2018
 *      Author: OS1
 */

#ifndef KERSEM_H_
#define KERSEM_H_
class ListaCekanja;

class KernelSem {
public:
	int vrijednost, inicijalno;
	ListaCekanja* red_cekanja;
	int wait (int toBlock);
	void signal();
	int val () const;
	KernelSem(int vr);
	~KernelSem();
};

#endif /* KERSEM_H_ */
