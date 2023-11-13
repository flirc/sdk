/**
 * COPYRIGHT 2023 Flirc, Inc. All rights reserved.
 *
 * This copyright notice is Copyright Management Information under 17 USC 1202
 * and is included to protect this work and deter copyright infringement.
 * Removal or alteration of this Copyright Management Information without
 * the express written permission of Flirc, Inc. is prohibited, and any
 * such unauthorized removal or alteration will be a violation of federal law.
 *
 * @file
 * @brief   temporary file, will be replaced
 */

#ifndef I__IR_H
#define I__IR_H

#include <stdint.h>

#ifndef MAX_EDGES
#define MAX_EDGES			256
#endif

#ifndef MAX_DESC
#define MAX_DESC			100
#endif

#ifndef MAX_REP
#define MAX_REP				30
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BUILD_DLL
#ifdef _DLL
#undef _DLL
#endif
#define _DLL __declspec(dllexport)
#else
#define _DLL
#endif

#ifndef I__FLIRC_H__
struct ir_packet {
	uint16_t buf[256];
	uint16_t len;
	uint16_t elapsed;
};
#endif

/**
 * enum rc_proto - the Remote Controller protocol
 *
 * @RC_PROTO_UNKNOWN: Protocol not known
 * @RC_PROTO_OTHER: Protocol known but proprietary
 * @RC_PROTO_RC5: Philips RC5 protocol
 * @RC_PROTO_RC5X_20: Philips RC5x 20 bit protocol
 * @RC_PROTO_RC5_SZ: StreamZap variant of RC5
 * @RC_PROTO_JVC: JVC protocol
 * @RC_PROTO_SONY12: Sony 12 bit protocol
 * @RC_PROTO_SONY15: Sony 15 bit protocol
 * @RC_PROTO_SONY20: Sony 20 bit protocol
 * @RC_PROTO_NEC: NEC protocol
 * @RC_PROTO_NECX: Extended NEC protocol
 * @RC_PROTO_NEC32: NEC 32 bit protocol
 * @RC_PROTO_SANYO: Sanyo protocol
 * @RC_PROTO_MCIR2_KBD: RC6-ish MCE keyboard
 * @RC_PROTO_MCIR2_MSE: RC6-ish MCE mouse
 * @RC_PROTO_RC6_0: Philips RC6-0-16 protocol
 * @RC_PROTO_RC6_6A_20: Philips RC6-6A-20 protocol
 * @RC_PROTO_RC6_6A_24: Philips RC6-6A-24 protocol
 * @RC_PROTO_RC6_6A_32: Philips RC6-6A-32 protocol
 * @RC_PROTO_RC6_MCE: MCE (Philips RC6-6A-32 subtype) protocol
 * @RC_PROTO_SHARP: Sharp protocol
 * @RC_PROTO_XMP: XMP protocol
 * @RC_PROTO_CEC: CEC protocol
 * @RC_PROTO_IMON: iMon Pad protocol
 * @RC_PROTO_RCMM12: RC-MM protocol 12 bits
 * @RC_PROTO_RCMM24: RC-MM protocol 24 bits
 * @RC_PROTO_RCMM32: RC-MM protocol 32 bits
 */
enum rc_proto {
	RC_PROTO_UNKNOWN = 0,
	RC_PROTO_OTHER,
	RC_PROTO_RC5,
	RC_PROTO_RC5X_20,
	RC_PROTO_RC5_SZ,
	RC_PROTO_JVC,
	RC_PROTO_SONY12,
	RC_PROTO_SONY15,
	RC_PROTO_SONY20,
	RC_PROTO_NEC,
	RC_PROTO_NECX,
	RC_PROTO_NEC32,
	RC_PROTO_SANYO,
	RC_PROTO_MCIR2_KBD,
	RC_PROTO_MCIR2_MSE,
	RC_PROTO_RC6_0,
	RC_PROTO_RC6_6A_20,
	RC_PROTO_RC6_6A_24,
	RC_PROTO_RC6_6A_32,
	RC_PROTO_RC6_MCE,
	RC_PROTO_SHARP,
	RC_PROTO_XMP,
	RC_PROTO_CEC,
	RC_PROTO_IMON,
	RC_PROTO_RCMM12,
	RC_PROTO_RCMM24,
	RC_PROTO_RCMM32,
	RC_PROTO_ORTEK,
	RC_PROTO_DENON,
	RC_PROTO_FLIRC,
	RC_PROTO_NOKIA,
	RC_PROTO_TDC,
	RC_PROTO_AMINO, /* max BIT_ULL */
	RC_PROTO_NEC_APPLE,
};

/**
 * ir_lib_version() - Retrieves the version of the IR library.
 *
 * This function returns a string representing the current version of the
 * Flirc library. It can be used to display the library version or for
 * logging and diagnostic purposes.
 *
 * @return          - Constant character pointer to the string containing
 *                    the library version. The memory for this string is 
 *                    managed internally and should not be freed or modified.
 */
const char _DLL *ir_lib_version(void);

/**
 * enum_to_str() - Converts an IR protocol enumeration to a string.
 *
 * Given an IR protocol enumerated type, this function returns a
 * constant character pointer to the string representation of the
 * protocol. This is useful for logging, debugging, or any function
 * that requires a human-readable format of the protocol enum.
 *
 * @param protocol  - The enumerated value of type rc_proto
 *                    representing an IR protocol.
 *
 * @return          - Pointer to a string constant of the protocol's
 *                    name. Returns NULL if the protocol is not recognized.
 */
const char _DLL *enum_to_str(enum rc_proto protocol);

/**
 * str_to_enum() - Converts a string representation of a protocol to its enum.
 *
 * This function takes a string representing an IR protocol's name and
 * converts it to the corresponding enumerated value of type rc_proto.
 * This can be used when parsing text-based configurations or input that
 * specifies IR protocols by name.
 *
 * @param *protocol - Pointer to a string containing the name of an IR protocol.
 *
 * @return          - The enumerated value of type rc_proto that corresponds to
 *                    the given string. Returns RC_UNKNOWN or an appropriate error
 *                    value if the string does not match a known protocol.
 */
enum rc_proto _DLL str_to_enum(const char *protocol);

struct ir_prot {
	/* used to override protocol to sub protocol, NEC->APPLE_NEC */
	enum rc_proto protocol;
	/* 32 Bit Scancode from remote */
	uint32_t scancode;
	/* 32 bit entire code extracted from remote (don't use this) */
	uint32_t fullcode;
	/* 32 Bit Unique Hash */
	uint32_t hash;
	/* repeat detected = 1, 0 = no repeat */
	uint8_t repeat;
	/* amount of bits collected */
	uint32_t bits;
	/* Used to describe our collected signal */
	char desc[MAX_DESC];
	/* cleaned signal after decode */
	uint16_t buf[MAX_EDGES];
	/* length of clean buf */
	uint16_t len;
	/* pronto hex code */
	uint16_t pronto[MAX_EDGES];
	/* pronto length */
	uint16_t pronto_len;
};

/**
 * ir_enabled_protocols() - Prints all currently enabled IR protocols.
 *
 * Outputs a list or details of all the IR protocols that are currently enabled
 * or active in the system. Useful for diagnostic or user-feedback purposes.
 *
 * @return - None.
 */
void _DLL ir_enabled_protocols(void);

/**
 * ir_decode_packetf() - Decodes an IR signal buffer into a meaningful command or data.
 *
 * Given a buffer of IR signal timings, this function attempts to decode and 
 * interpret the signal, translating it into a specific command or data format.
 *
 * @param *buf - Pointer to the buffer containing IR signal timings.
 * @param len  - Length of the IR signal data buffer.
 * @param p    - Pointer to a struct that will be populated
 *
 * @return     - An integer representing the result of the decode operation. This 
 *               could be a command ID, error code, or other relevant value.
 */
int _DLL ir_decode_packet(struct ir_packet *ir, struct ir_prot *p);

/**
 * ir_decode_packetf() - Decodes an IR signal buffer into a meaningful command or data.
 *
 * Given a buffer of IR signal timings, this function attempts to decode and 
 * interpret the signal, translating it into a specific command or data format.
 *
 * @param *buf - Pointer to the buffer containing IR signal timings.
 * @param len  - Length of the IR signal data buffer.
 * @param p    - Pointer to a struct that will be populated
 *
 * @return     - An integer representing the result of the decode operation.
 * 		 This could be a command ID, error code, or other relevant
 * 		 value.
 */
int _DLL ir_encode(enum rc_proto protocol, uint32_t scancode, struct ir_packet *ir);

/**
 * ir_tx() - Transmits an IR code based on the provided protocol and scancode.
 *
 * This function manages the transmission of an IR signal according to the 
 * given protocol and scancode. It defaults to a single transmission and can 
 * repeat the signal as specified. The function leverages 'ir_encode' to 
 * handle encoding and depends on a previously registered IR transmit 
 * callback for actual transmission. Special repeat frames, inherent to 
 * certain protocols, are also managed.
 *
 * @param protocol   - The IR protocol used for the transmission.
 * @param scancode   - The scancode or value to be transmitted.
 * @param repeat     - Specifies the number of times the signal is repeated.
 *                     A value of 0 means default single transmission.
 *
 * @return           - Returns 0 on successful transmission. If the transmit 
 *                     callback hasn't been set, or any other error occurs, 
 *                     it returns -1.
 */
int _DLL ir_tx(enum rc_proto protocol, uint32_t scancode, int repeat);

/**
 * ir_register_tx() - Registers a callback function for transmitting IR protocols.
 *
 * This function allows the user to set a custom callback to handle the
 * transmission of IR signals. The suggested callback to use is 'fl_transmit_raw'
 * found in the 'flirc.h' header, but users can set their own if needed.
 *
 * @param cb         - A pointer to the callback function with the following signature:
 *                     int callback(uint16_t *buf, uint16_t len, uint16_t ik,
 *                                  uint8_t rep);
 *                     Where:
 *                     - buf: Points to the buffer containing IR signal data.
 *                     - len: The length of the IR signal data buffer.
 *                     - ik:  Inter-key delay. Time between end of one keypress and start 
 *                            of the next, if any.
 *                     - rep: Indicates if the key is being held (repeated).
 *
 * @return           - Returns 0 if the callback registration is successful.
 *                     On error, an appropriate error code is returned.
 */
int _DLL ir_register_tx(int (*cb)(uint16_t *buf, uint16_t len, 
                             uint16_t ik, uint8_t rep));

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* I__IR_H__ */
