/*
 * Sem.h
 *
 *  Created on: Nov 16, 2018
 *      Author: OS1
 */

#ifndef SEM_H_
#define SEM_H_

class KernelSem;
class Semaphore {
public:
Semaphore (int init=1);
virtual ~Semaphore ();
virtual int wait (int toBlock);
virtual void signal();
int val () const; // Returns the current value of the semaphore
private:
KernelSem* myImpl;
};

#endif /* SEM_H_ */
