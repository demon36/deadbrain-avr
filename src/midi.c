#include <usbdrv/usbdrv.h>
#include "midi.h"

#define EMPTY_PACKET_CIN 0x00
#define DATA_CIN 0x09
#define SYSEX_CIN 0x07 //sysex first part CIN

#define NOTE_HEADER 0x90 //note on
#define SYSEX_HEADER 0xF0 //sysex start

#define MIDI_QUEUE_SIZE 8

typedef struct{
	uint8_t cin; //cable number and code index page
	uint8_t hdr; //status
	uint8_t dat0;
	uint8_t dat1;
} midi_packet;

midi_packet usb_packet_buffer;
midi_packet midi_rot_q[MIDI_QUEUE_SIZE] = {0};
midi_packet* q_head = midi_rot_q;
midi_packet* q_tail = midi_rot_q;

//tells whether or not ptr points at end
uint8_t midi_q_is_end(midi_packet* ptr){
	if(ptr == &midi_rot_q[MIDI_QUEUE_SIZE-1]){
		return 1;
	}else{
		return 0;
	}
}

uint8_t midi_qpush_note_msg(uint8_t channel, uint8_t velocity){
	if(q_head->cin != EMPTY_PACKET_CIN){
		return 0;
	}

	q_head->cin = DATA_CIN;
	q_head->hdr = NOTE_HEADER;
	q_head->dat0 = channel;
	q_head->dat1 = velocity;

	if(midi_q_is_end(q_head)){
		q_head = midi_rot_q;
	}else{
		q_head++;
	}

	return 1;
}

uint8_t midi_qpush_sysex_msg(uint8_t data0, uint8_t data1);

uint8_t midi_release_qpacket(){
	if(q_tail->cin == EMPTY_PACKET_CIN || !usbInterruptIsReady()){
		return 0;
	}

	usb_packet_buffer.cin = q_tail->cin;
	usb_packet_buffer.hdr = q_tail->hdr;
	usb_packet_buffer.dat0 = q_tail->dat0;
	usb_packet_buffer.dat1 = q_tail->dat1;
	usbSetInterrupt((uchar*)&usb_packet_buffer, sizeof(midi_packet));

	q_tail->cin = EMPTY_PACKET_CIN;
//	memset(q_tail, 0, sizeof(midi_packet));
	if(midi_q_is_end(q_tail)){
		q_tail = midi_rot_q;
	}else{
		++q_tail;
	}

	return 1;
}
