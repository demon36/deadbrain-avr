#include <usbdrv/usbdrv.h>
#include <string.h>
#include "midi.h"

#define EMPTY_PACKET_CIN		0x00
#define DATA_CIN				0x09
#define SYSEX_CIN_START_CONT	0x04 //start/continue CIN

#define SYSEX_CIN_END_AFTER_1	0x05
#define SYSEX_CIN_END_AFTER_2	0x06
#define SYSEX_CIN_END_AFTER_3	0x07

#define NOTE_ON_HEADER			0x90
#define SYSEX_HEADER			0xF0
#define SYSEX_FOOTER			0xF7

#define MIDI_QUEUE_SIZE 128

typedef struct{
	uint8_t cin; //cable number and code index page
	uint8_t b0; //status
	uint8_t b1;
	uint8_t b2;
	uint8_t cin2;
	uint8_t b3;
	uint8_t b4;
	uint8_t b5;
} midi_packet;

midi_packet usb_packet_buffer;
midi_packet midi_rot_q[MIDI_QUEUE_SIZE] = {{0}};
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

uint8_t midi_qpush_note_msg(uint8_t note, uint8_t velocity){
	if(q_head->cin != EMPTY_PACKET_CIN){
		return 0;
	}

	q_head->cin = DATA_CIN;
	q_head->b0 = NOTE_ON_HEADER;
	q_head->b1 = note;
	q_head->b2 = velocity;
	q_head->cin2 = 0x00;
	q_head->b3 = 0x00;
	q_head->b4 = 0x00;
	q_head->b5 = 0x00;

	if(midi_q_is_end(q_head)){
		q_head = midi_rot_q;
	}else{
		q_head++;
	}

	return 1;
}

uint8_t midi_qpush_raw_packet(uint8_t cin, uint8_t b0, uint8_t b1, uint8_t b2, uint8_t cin2, uint8_t b3, uint8_t b4, uint8_t b5){
	if(q_head->cin != EMPTY_PACKET_CIN){
		return 0;
	}

	q_head->cin = cin;
	q_head->b0 = b0;
	q_head->b1 = b1;
	q_head->b2 = b2;
	q_head->cin2 = cin2;
	q_head->b3 = b3;
	q_head->b4 = b4;
	q_head->b5 = b5;

	if(midi_q_is_end(q_head)){
		q_head = midi_rot_q;
	}else{
		++q_head;
	}

	return 1;
}

uint8_t midi_qpush_sysex_single_packet(uint8_t dat0, uint8_t dat1, uint8_t dat2, uint8_t dat3){
	return midi_qpush_raw_packet(
			SYSEX_CIN_START_CONT,
			SYSEX_HEADER,
			dat0,
			dat1,
			SYSEX_CIN_END_AFTER_3,
			dat2,
			dat3,
			SYSEX_FOOTER
		);
}

uint8_t midi_qpush_sysex_start_packet(uint8_t dat0, uint8_t dat1, uint8_t dat2, uint8_t dat3, uint8_t dat4){
	return midi_qpush_raw_packet(
			SYSEX_CIN_START_CONT,
			SYSEX_HEADER,
			dat0,
			dat1,
			SYSEX_CIN_START_CONT,
			dat2,
			dat3,
			dat4
		);
}

uint8_t midi_qpush_sysex_cont_packet(uint8_t dat0, uint8_t dat1, uint8_t dat2, uint8_t dat3, uint8_t dat4, uint8_t dat5){
	return midi_qpush_raw_packet(
			SYSEX_CIN_START_CONT,
			dat0,
			dat1,
			dat2,
			SYSEX_CIN_START_CONT,
			dat3,
			dat4,
			dat5
		);
}

uint8_t midi_qpush_sysex_end_packet(uint8_t* dat, uint8_t len){
	switch(len){
		case 5:
			return midi_qpush_raw_packet(
					SYSEX_CIN_START_CONT,
					dat[0],
					dat[1],
					dat[2],
					SYSEX_CIN_END_AFTER_3,
					dat[3],
					dat[4],
					SYSEX_FOOTER
				);
		case 4:
			return midi_qpush_raw_packet(
					SYSEX_CIN_START_CONT,
					dat[0],
					dat[1],
					dat[2],
					SYSEX_CIN_END_AFTER_2,
					dat[3],
					SYSEX_FOOTER,
					0x00
				);
		case 3:
			return midi_qpush_raw_packet(
					SYSEX_CIN_START_CONT,
					dat[0],
					dat[1],
					dat[2],
					SYSEX_CIN_END_AFTER_1,
					SYSEX_FOOTER,
					0x00,
					0x00
				);
		case 2:
			return midi_qpush_raw_packet(
					SYSEX_CIN_END_AFTER_3,
					dat[0],
					dat[1],
					SYSEX_FOOTER,
					0x00,
					0x00,
					0x00,
					0x00
				);
		case 1:
			return midi_qpush_raw_packet(
					SYSEX_CIN_END_AFTER_2,
					dat[0],
					SYSEX_FOOTER,
					0x00,
					0x00,
					0x00,
					0x00,
					0x00
				);
		case 0:
			return midi_qpush_raw_packet(
					SYSEX_CIN_END_AFTER_1,
					SYSEX_FOOTER,
					0x00,
					0x00,
					0x00,
					0x00,
					0x00,
					0x00
				);
	}
	return 0;
}

uint8_t midi_qpush_sysex_dump(uint8_t* dat, uint8_t len){
	uint8_t cursor = 0;
	midi_qpush_sysex_start_packet(
			dat[cursor],
			dat[cursor+1],
			dat[cursor+2],
			dat[cursor+3],
			dat[cursor+4]
		);
	cursor += 5;

	while(len - cursor > 5){
		midi_qpush_sysex_cont_packet(
				dat[cursor],
				dat[cursor+1],
				dat[cursor+2],
				dat[cursor+3],
				dat[cursor+4],
				dat[cursor+5]
			);
		cursor += 6;
	}

	midi_qpush_sysex_end_packet(
			&dat[cursor],
			len - cursor
		);
	//add error check
	return 1;
}

uint8_t midi_qpop_packet(){
	if(q_tail->cin == EMPTY_PACKET_CIN || !usbInterruptIsReady()){
		return 0;
	}

	usb_packet_buffer.cin = q_tail->cin;
	usb_packet_buffer.b0 = q_tail->b0;
	usb_packet_buffer.b1 = q_tail->b1;
	usb_packet_buffer.b2 = q_tail->b2;
	usb_packet_buffer.cin2 = q_tail->cin2;
	usb_packet_buffer.b3 = q_tail->b3;
	usb_packet_buffer.b4 = q_tail->b4;
	usb_packet_buffer.b5 = q_tail->b5;

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
