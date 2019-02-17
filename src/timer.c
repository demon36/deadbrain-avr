#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

void (*overflow_cb)() = 0;

void timer_init()
{
	TCCR1A = 0;
	TCCR1B = _BV(CS11); //set time prescaler to 8
	TCCR1C = 0;

    // enable overflow interrupt
    TIMSK1 |= (1 << TOIE1);

    // initialize counter
    TCNT1H = 0x00;
    TCNT1L = 0x00;
}

void timer_register_cb(void (*cb)()){
	overflow_cb = cb;
}

ISR(TIMER1_OVF_vect)
{
	//this should be called each 26.214ms
	//(1/(2MGhz/8))*65535
	if(overflow_cb != 0){
		overflow_cb();
	}
}
