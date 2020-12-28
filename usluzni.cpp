/*
 * usluzni.cpp
 *
 *  Created on: Nov 12, 2018
 *      Author: OS1
 */

#include "usluzni.h"
#include "PCB.h"
#include "Kernel.h"
#include <iostream.h>
#include "Schedule.h"

ElementListe::ElementListe(PCB* p){
	element = p;
	sledeci = 0;
}

ElementListe::~ElementListe(){
	element = 0;
	sledeci = 0;
}

void SleepLista::stavi(PCB* pcb, unsigned kvant){//implementirano tako da se cuva razlika
	lock
	ElementListe* novi = new ElementListe(pcb);
	novi->element->spava = 1;
	if(!prvi){
		prvi = novi;
		prvi->element->kvant_spavanja = kvant;
		novi = 0;
	}
	else {
		if(kvant < prvi->element->kvant_spavanja){
			prvi->element->kvant_spavanja = prvi->element->kvant_spavanja - kvant;
			novi->element->kvant_spavanja = kvant;
			novi->sledeci = prvi;
			prvi = novi;
			novi = 0;

		}
		else {
			ElementListe* tek = prvi;
			ElementListe* pret = prvi;
			while (tek && kvant>=tek->element->kvant_spavanja){
				pret = tek;
				kvant = kvant - tek->element->kvant_spavanja;
				tek = tek->sledeci;
			}
			pret->sledeci = novi;
			novi->element->kvant_spavanja = kvant;
			novi->sledeci = tek;
			if(tek)tek->element->kvant_spavanja = tek->element->kvant_spavanja - kvant;
			tek = 0;
			pret = 0;
			novi = 0;
		}
	}
	unlock


}

void SleepLista::smanji_kvant(){
	lock
	prvi->element->kvant_spavanja--;
	while (prvi && prvi->element->kvant_spavanja == 0){
				ElementListe *tek = prvi;
				prvi -> element->spava = 0;
				Scheduler::put(prvi->element);
				Kernel::brojNitiURasporedjivacu++;
				prvi = prvi->sledeci;
				delete tek;
		}
	unlock
}

SleepLista::SleepLista(){
	prvi = 0;
}

SleepLista::~SleepLista(){
	lock
	if(prvi)delete prvi;
	unlock
}

void ListaCekanja::stavi(PCB* pcb){
	ElementListe* novi = new ElementListe(pcb);
	novi->element->ceka = 1;//nit moze da ceka maksimalno jednu drugu nit u datom momentu zbog realizacije delete operatora
	novi->sledeci = prvi;//uvijek se umece na pocetak, nebitan je poredak
	prvi = novi;
}

PCB* ListaCekanja::uzmi(){//mozda ce valjati za semafor
	PCB* pcb = prvi->element;
	ElementListe* tek = prvi;
	prvi = prvi->sledeci;//ako nema sledeceg prvi postaje nula
	lock
	delete tek;
	unlock
	pcb->ceka = 0;
	return pcb;
}

/*int ListaCekanja::nije_prazna(){
	if(prvi)return 1;
	else return 0;
}*/

void ListaCekanja::isprazni(){
	while(prvi){
		prvi->element->ceka = 0;
		Scheduler::put(prvi->element);
		Kernel::brojNitiURasporedjivacu++;
		ElementListe* tek = prvi;
		prvi = prvi->sledeci;
		delete tek;
	}
}

ListaCekanja::~ListaCekanja(){
	lock
	if(prvi)delete prvi;
	unlock
}

ListaCekanja::ListaCekanja(){
	prvi = 0;
}

