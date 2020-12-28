/*
 * event.h
 *
 *  Created on: Dec 20, 2018
 *      Author: OS1
 */

#ifndef EVENT_H_
#define EVENT_H_

typedef unsigned char IVTNo;

#include "IVTEntry.h"

class KernelEv;
class Event {
public:
Event (IVTNo ivtNo);
~Event ();
void wait ();
protected:
friend class KernelEv;
void signal(); // can call KernelEv
private:
KernelEv* myImpl;
};


#define PREPAREENTRY(brUlaza,stara)\
void interrupt inter##brUlaza(...);\
IVTEntry* ulaz##brUlaza = new IVTEntry(brUlaza,inter##brUlaza);\
void interrupt inter##brUlaza(...){\
	ulaz##brUlaza->signal();\
	if(stara == 1)\
	ulaz##brUlaza->old();\
}


#endif /* EVENT_H_ */

