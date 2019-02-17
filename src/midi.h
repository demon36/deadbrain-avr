#ifndef MIDI_H_
#define MIDI_H_

#include <stdint.h>

uint8_t midi_qpush_note_msg(uint8_t channel, uint8_t velocity); //return 0 at full queue
uint8_t midi_qpush_sysex_msg(uint8_t data0, uint8_t data1);
uint8_t midi_release_qpacket();//pops a message from queue and sends it to host



#endif /* MIDI_H_ */
