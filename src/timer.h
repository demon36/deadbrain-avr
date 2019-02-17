/*
 * timer.h
 *
 *  Created on: Feb 28, 2016
 *      Author: demon
 */

#ifndef TIMER_H_
#define TIMER_H_

void timer_init();
void timer_register_cb(void (*cb)());

#endif /* TIMER_H_ */
