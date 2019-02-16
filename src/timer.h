/*
 * timer.h
 *
 *  Created on: Feb 28, 2016
 *      Author: demon
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t tot_overflow;
void init_timer1()
{
	TCCR1A = 0;// set registers to 0
	TCCR1B = 0;
	TCCR1C = 0;

    // enable overflow interrupt
    TIMSK1 |= (1 << TOIE1);

    // initialize counter
    TCNT1H = 0x00;
    TCNT1L = 0x00;
    // initialize overflow counter variable
    tot_overflow = 0;
}

#endif /* TIMER_H_ */
