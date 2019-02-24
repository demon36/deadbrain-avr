#ifndef MIDI_H_
#define MIDI_H_

#include <stdint.h>

uint8_t midi_qpush_note_msg(uint8_t channel, uint8_t velocity); //return 0 at full queue
uint8_t midi_qpush_sysex_single_packet(uint8_t dat0, uint8_t dat1, uint8_t dat2, uint8_t dat3);
uint8_t midi_qpush_sysex_start_packet(uint8_t dat0, uint8_t dat1, uint8_t dat2, uint8_t dat3, uint8_t dat4);
uint8_t midi_qpush_sysex_cont_packet(uint8_t dat0, uint8_t dat1, uint8_t dat2, uint8_t dat3, uint8_t dat4, uint8_t dat5);
uint8_t midi_qpush_sysex_end_packet(uint8_t* dat, uint8_t len);
uint8_t midi_qpush_sysex_dump(uint8_t* dat, uint8_t len);
uint8_t midi_qpop_packet();//pops a message from queue and sends it to host

#endif /* MIDI_H_ */
