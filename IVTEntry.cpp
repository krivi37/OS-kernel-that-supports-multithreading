/*
 * IVTEntry.cpp
 *
 *  Created on: Dec 20, 2018
 *      Author: OS1
 */

#include "IVTEntry.h"
#include <dos.h>
#include "KerEv.h"

IVTEntry* IVTEntry::tabela[256]={0};

IVTEntry::IVTEntry(IVTNo ivtn, pInterrupt nova) {
	lock
	ulaz = ivtn;
	dogadjaj = 0;
	staraRutina = getvect(ivtn);
	setvect (ivtn, nova);
	IVTEntry::tabela[ulaz] = this;
	unlock
}

IVTEntry::~IVTEntry() {
	lock
	IVTEntry::tabela[ulaz] = 0;
	setvect(ulaz, staraRutina);
	if (ulaz == 0x09)
				asm{
				mov al, 20h
				out 20h, al
		}
	unlock
}

void IVTEntry::signal(){
	lock
	dogadjaj->signal();
	unlock
}

void IVTEntry::wait(){
	lock
	dogadjaj->wait();
	unlock
}

void IVTEntry::old(){
	lock
	staraRutina();
	unlock
}

IVTEntry* IVTEntry::getEntry(IVTNo ivtn){
	return IVTEntry::tabela[ivtn];
}
