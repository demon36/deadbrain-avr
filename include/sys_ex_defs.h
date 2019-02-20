#ifndef SYS_EX_DEFS_H_
#define SYS_EX_DEFS_H_

//requests
#define SYSEX_GET_CONFIG		0x01
#define SYSEX_SAVE_CONFIG		0x02
#define SYSEX_ENABLE_CHANNEL	0x03
#define SYSEX_SET_SCAN_TIME		0x04
#define SYSEX_SET_THRESHOLD		0x05
#define SYSEX_SET_NOTE			0x06
#define SYSEX_SET_RETRIGGER		0x07
#define SYSEX_DEBUG_SIGNAL		0x08
#define SYSEX_END_DEBUG			0x09

#define SYSEX_REPLY_OK			0x0A
#define SYSEX_REPLY_FAIL		0x0B

#define NUM_CHANNELS			16

typedef struct{
	uint8_t enabled;
	uint8_t scan_time;
	uint8_t threshold;
	uint8_t note;
	uint8_t retrigger;
} channel_cfg;

typedef channel_cfg deadbrain_config[NUM_CHANNELS];

#endif /* SYS_EX_DEFS_H_ */
