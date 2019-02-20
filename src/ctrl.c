#include <string.h>
#include <usbdrv/usbdrv.h>
#include "dsp.h"
#include "midi.h"
#include "sys_ex_defs.h"

void ctr_reply_settings(){
	uint8_t* config_ptr = (uint8_t*)settings;
	uint8_t len = sizeof(deadbrain_config);

	uint8_t cursor = 0;
	midi_qpush_sysex_start_packet(
			config_ptr[0],
			config_ptr[1],
			config_ptr[2],
			config_ptr[3],
			config_ptr[4]
		);
	cursor += 5;

	while(len - cursor > 5){
		midi_qpush_sysex_cont_packet(
				config_ptr[cursor],
				config_ptr[cursor+1],
				config_ptr[cursor+2],
				config_ptr[cursor+3],
				config_ptr[cursor+4],
				config_ptr[cursor+5]
			);
		cursor += 6;
	}

	midi_qpush_sysex_end_packet(
			&config_ptr[cursor],
			len - cursor
		);

}

void ctrl_reply_status(uint8_t status){
	midi_qpush_sysex_single_packet(status, 0, 0, 0);
}

void usbFunctionWriteOut(uchar * data, uchar len)
{
	if(len < 5){
		ctrl_reply_status(SYSEX_REPLY_FAIL);
		return;
	}

	uint8_t request_id = data[2];
	uint8_t channel_index = data[3];
	uint8_t attrib_value = data[4];

	switch(request_id){

		case SYSEX_GET_CONFIG:
			ctr_reply_settings();
			break;

		case SYSEX_SAVE_CONFIG:
			save_settings();
			ctrl_reply_status(SYSEX_REPLY_OK);
			break;

		case SYSEX_ENABLE_CHANNEL:
			settings[channel_index].enabled = 0x01;
			ctrl_reply_status(SYSEX_REPLY_OK);
			break;

		case SYSEX_SET_SCAN_TIME:
			settings[channel_index].scan_time = attrib_value;
			ctrl_reply_status(SYSEX_REPLY_OK);
			break;

		case SYSEX_SET_THRESHOLD:
			settings[channel_index].threshold = attrib_value;
			ctrl_reply_status(SYSEX_REPLY_OK);
			break;

		case SYSEX_SET_NOTE:
			settings[channel_index].note = attrib_value;
			ctrl_reply_status(SYSEX_REPLY_OK);
			break;

		case SYSEX_SET_RETRIGGER:
			settings[channel_index].retrigger = attrib_value;
			ctrl_reply_status(SYSEX_REPLY_OK);
			break;

		case SYSEX_DEBUG_SIGNAL:
			ctrl_reply_status(SYSEX_REPLY_FAIL);
			break;

		default:
			ctrl_reply_status(SYSEX_REPLY_FAIL);
			break;
	}

}
