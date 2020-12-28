/*
 * usluzni.h
 *
 *  Created on: Nov 12, 2018
 *      Author: OS1
 */

#ifndef USLUZNI_H_
#define USLUZNI_H_

class PCB;

typedef void interrupt (*pInterrupt)(...);

#define lock asm {\
	pushf;\
	cli;\
}

#define unlock asm popf;

#define cLock {\
	Kernel::lockFlag=0;\
}

#define cUnLock {\
	Kernel::lockFlag=1;}//\
	//if(Kernel::zahtjevana_promjena_konteksta)\
	//	dispatch();\
//}

class ElementListe{
public:
	PCB* element;
	ElementListe* sledeci;
	ElementListe(PCB* p);
	~ElementListe();
};

class SleepLista{//treba da postoji samo jedna sleep lista
public:
	ElementListe* prvi;
	void stavi(PCB* pcb, unsigned kvant);
	void smanji_kvant();
	~SleepLista();
	SleepLista();
};

class ListaCekanja{//i za semafor?
public:
	ElementListe* prvi;
	void stavi(PCB* pcb);
	//int nije_prazna();
	void isprazni();
	PCB* uzmi();//mozda ce trebati za semafore
	ListaCekanja();
	~ListaCekanja();
};


#endif /* USLUZNI_H_ */
