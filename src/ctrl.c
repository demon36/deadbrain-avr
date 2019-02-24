#include <string.h>
#include <usbdrv/usbdrv.h>
#include "dsp.h"
#include "midi.h"
#include "sys_ex_defs.h"

void ctr_reply_settings(){

	uint8_t* config_ptr = (uint8_t*)settings;
	uint8_t len = sizeof(DeadBrainConfig);

	//make sure no bytes have FSB set
	for(uint8_t i = 0; i < len; i++){
		if(config_ptr[i] & 0x80){
			config_ptr[i] &= 0x7F;
		}
	}

	midi_qpush_sysex_dump(config_ptr, sizeof(DeadBrainConfig));

}

void ctrl_reply_status(uint8_t status){
	midi_qpush_sysex_single_packet(status, 0x00, 0x00, 0x00);
}

/**
 * @return reply status byte
 */
uint8_t ctrl_handle_set_config_request(uint8_t request_id, uint8_t channel_index, uint8_t attrib_value){

	if(channel_index == 0xFF || attrib_value == 0xFF){
		return SYSEX_REPLY_FAIL;
	}

	switch(request_id){

		case SYSEX_SET_ENABLED:
			settings[channel_index].enabled = attrib_value;
			return SYSEX_REPLY_OK;

		case SYSEX_SET_SCAN_TIME:
			settings[channel_index].scan_time = attrib_value;
			return SYSEX_REPLY_OK;

		case SYSEX_SET_THRESHOLD:
			settings[channel_index].threshold = attrib_value;
			return SYSEX_REPLY_OK;

		case SYSEX_SET_NOTE:
			settings[channel_index].note = attrib_value;
			return SYSEX_REPLY_OK;

		case SYSEX_SET_RETRIGGER:
			settings[channel_index].retrigger = attrib_value;
			return SYSEX_REPLY_OK;

		default:
			return SYSEX_REPLY_FAIL;
	}

	return SYSEX_REPLY_FAIL;
}

void usbFunctionWriteOut(uchar* data, uchar len)
{
	//request example: F0 01 55 66 F7
	//would translate to: 04 F0 01 55, 06 66 F7 00

	uint8_t channel_index = 0xFF;
	uint8_t attrib_value = 0xFF;

	if(len < 4){
		ctrl_reply_status(SYSEX_REPLY_FAIL);
		return;
	} else if(len > 5){
		channel_index = data[3];
		attrib_value = data[5];
	}
	uint8_t request_id = data[2];

	switch(request_id){
		case SYSEX_GET_CONFIG:
			ctr_reply_settings();
			break;

		case SYSEX_SAVE_CONFIG:
			dsp_save_settings();
			ctrl_reply_status(SYSEX_REPLY_OK);
			break;

		case SYSEX_DEBUG_START://not yet supported
		case SYSEX_DEBUG_END:
			ctrl_reply_status(SYSEX_REPLY_FAIL);
			break;

		default://set config request
			ctrl_reply_status(ctrl_handle_set_config_request(request_id, channel_index, attrib_value));
			break;
	}

}
