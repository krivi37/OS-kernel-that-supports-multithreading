/*
 * Kernel.cpp
 *
 *  Created on: Nov 12, 2018
 *      Author: OS1
 */

#include "Kernel.h"
#include "usluzni.h"
#include "Schedule.h"
#include "Thread.h"
#include <dos.h>
#include <iostream.h>
#include "PCB.h"
//#include "IVTEntry.h"

extern void tick();
extern int userMain(int argc, char** argv);


volatile int Kernel::return_vr = 0;
pInterrupt Kernel::staraRutina = 0;
unsigned Kernel::lockFlag = 1;
unsigned Kernel::zahtjevana_promjena_konteksta=0;
unsigned Kernel::brojac=20;
unsigned volatile Kernel::brojNitiURasporedjivacu=0;
unsigned volatile Kernel::brojAktivnihNiti=0;
PCB* Kernel::pocetni=0;
PCB* Kernel::idle_nit=0;
SleepLista* Kernel::SLista = new SleepLista();

unsigned tss, tsp, tbp;

class PocetnaNit:public Thread{
public:
	PocetnaNit(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice):Thread(stackSize, timeSlice){}
};

class UserNit:public Thread{
public:
	int argc;
	char** argv;
	void run(){
		Kernel::return_vr = userMain(argc, argv);
	}
	UserNit(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice, int a, char** b):Thread(stackSize, timeSlice){
		argc = a;
		argv = b;
	}
};


void interrupt Kernel::timer(...){
	if(!zahtjevana_promjena_konteksta){
		asm int 60h;
		tick();
		if(!PCB::running->neogranicen)brojac--;
		SLista->smanji_kvant();
	}

	if((brojac == 0 && !(PCB::running->neogranicen)) || zahtjevana_promjena_konteksta){
		if(lockFlag){
			zahtjevana_promjena_konteksta = 0;
			asm {
				// cuva sp i bp
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}

			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;

			//cout<<"promjena konteksta! brojac: "<<brojac<<endl;
			//asm cli;

			if (!(PCB::running->zavrsio||PCB::running->spava||PCB::running==idle_nit||PCB::running->ceka||PCB::running==pocetni||PCB::running->blokiran)) {
				Scheduler::put((PCB *) PCB::running);
				brojNitiURasporedjivacu++;
			}

			if(brojNitiURasporedjivacu){
				//cout<<"broj niti u rasp2: "<<brojNitiURasporedjivacu<<endl;
				//asm cli;
				PCB::running=Scheduler::get();
				brojNitiURasporedjivacu--;
			}
			else if(brojAktivnihNiti) PCB::running = idle_nit;
			else PCB::running = pocetni;

			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;
			if(!PCB::running->neogranicen)brojac = PCB::running->kvant;

			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
		}
		else zahtjevana_promjena_konteksta = 1;
	}
}

class Idle:public Thread{
public:
	void run(){
		while(Kernel::brojAktivnihNiti){
			//cLock;
			//cout<<"Idle ";
			//cUnLock;
			if(Kernel::brojNitiURasporedjivacu){
			dispatch();
			}
		}
	}
	Idle(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice):Thread(stackSize, timeSlice){}
};

void Kernel::init(){
	lock
	staraRutina = getvect(8);
	setvect(8, timer);
	setvect(0x60, staraRutina);
	PocetnaNit* poc = new PocetnaNit(0,20);
	poc->start();
	PCB::running = Scheduler::get();
	pocetni = (PCB*) PCB::running;
	Idle* idle = new Idle(1024,0);
	idle->start();
	idle_nit = Scheduler::get();
	brojNitiURasporedjivacu-=2;//idle i pocetna nit se ne stavljaju u scheduler
	brojAktivnihNiti-=2;
	//UserNit* user = new UserNit(1024, 20);
	//user->start();
	unlock

}

void Kernel::restore(){
	lock
	setvect(8, staraRutina);
	delete idle_nit;
	delete pocetni;
	unlock
}

int main(int argc, char** argv){
	Kernel::init();

	lock
	UserNit* user = new UserNit(1024, 4, argc, argv);
	unlock
	user->start();
	delete user;//poziva wait to complete
	int vr = userMain(argc, argv);
	Kernel::restore();
	return vr;
}
