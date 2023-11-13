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
 * @brief   Flirc Library functions
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <flirc/flirc.h>
#include <ir/ir.h>

#ifndef FRAME
#define FRAME			(1)
#endif

#ifndef NOFRAME
#define NOFRAME			(0)
#endif

#ifndef IS_ODD
#define IS_ODD(x)               (x & 0x1)
#endif

#ifndef IS_EVEN
#define IS_EVEN(x)              (!(IS_ODD(x)))
#endif

void ex_program(int sig) __attribute__ ((noreturn));
void ex_program(int sig)
{
	printf("\n");
	printf("Cleaning up....\n");
	fl_close_device();
	exit(1);
}

/**
 * All IR Captures have some loss. Edges are not clean, light bounces .The 
 * timing markers will vary for each signal captured, even if it's the same
 * protocol, and same button. 
 *
 * But if we decode the signal, we can clean up each edge as what is defined
 * in the spec. 
 *
 * Also works for encoding as well.
 */
static void print_cleaned(struct ir_prot *p)
{
	int i;

#ifdef PRINT_ELAPSED
	printf("+%d\n", p->elapsed*1000);
#endif

	for (i = 0; i < p->len; i++) {
		IS_EVEN(i) ? printf("+") : printf("-");
		printf("%d ", p->buf[i]);
	}
	printf("\n");
}

/**
 * Our decoder library automatically takes care of the pronto format for us.
 *
 * If we were to manually capture a raw signal and convert it to pronto without
 * knowing the protocol, we could lose invaluable information that may not 
 * transmit successfully. 
 *
 * Use the decode function on a frame to generate the pronto code. This function
 * prints the buffer in the common Hex format used by many applications.
 */
static void print_pronto(struct ir_prot *p)
{
	int i;

#ifdef PRINT_ELAPSED
	printf("+%d\n", p->elapsed*1000);
#endif

	for (i = 0; i < p->pronto_len; i++) {
		printf("%04X ", p->pronto[i]);
	}
	printf("\n");
}

/**
 * Prints our raw timing waveform in terms of positive and
 * negative pulses. Easiest to comprehend visually.
 */
static void print_raw(struct ir_packet *p)
{
	int i;

#ifdef PRINT_ELAPSED
	printf("+%d\n", p->elapsed*1000);
#endif

	for (i = 0; i < p->len; i++) {
		IS_EVEN(i) ? printf("+") : printf("-");
		printf("%d ", p->buf[i]);
	}
	printf("\n");
}

static void _listen(void) __attribute__ ((noreturn));
static void _listen(void)
{
	struct ir_prot d;
	struct ir_packet p;

	/**
	 * Stay here and poll the device. Once a complete packet is
	 * received, decode and print, and repeat. User must force
	 * quit the app
	 */
	while (1) {
		switch (fl_ir_packet_poll(&p)) {
		/**
		 * Packet received, print useful info and poll again
		 */
		case (FRAME):
			printf("----------------\n");
			ir_decode_packet(&p, &d);

			printf("0x%08X - %s : %d\n",
					d.scancode, d.desc, d.protocol);
			/* print the timing we received */
			print_raw(&p);
			/* the pronto version of what we received */
			print_pronto(&d);
			/* an idealized waveform based on protocol */
			print_cleaned(&d);
			printf("----------------\n");
			fflush(stdout);
			break;
		case (NOFRAME):
			break;
		case (-1):
			printf("error, disconnecting\n");
			break;
		default:
			printf("UNKNOWN ERROR\n");
			break;
		}
	}
}

/**
 * Wait for packet, decode, and resend after 2 seconds
 */
static void retransmit(void)
{
	struct ir_prot d;
	struct ir_packet p;

	int wait = 1;
	while (wait) {
		switch (fl_ir_packet_poll(&p)) {
		case (FRAME):
			wait = 0;
			break;
		case (NOFRAME):
			break;
		case (-1):
			printf("error, disconnecting\n");
			break;
		default:
			printf("UNKNOWN ERROR\n");
			break;
		}
	}

	/* decode packet received */
	ir_decode_packet(&p, &d);

	/* print the result received along with an idealized version */
	print_raw(&p);
	print_cleaned(&d);

	/* wait and then transmit */
	sleep(2);

	/* ensure we flush the interface of pending packets */
	fl_dev_flush();
	ir_tx(d.protocol, d.scancode, 1);
}

static void usage(void)
{
	printf("usage:\n");
	printf("ir listen\n");
	printf("     - This will poll the device, decode, and print the results\n");
	printf("ir transmit\n");
	printf("     - This will demonstrate transmission of 3 protocols, RC6, NEC, and RC5\n");
	printf("ir retransmit\n");
	printf("     - This will wait for a packet, decode, and retransmit the packet\n");
}

int main(int argc, char *argv[])
{
	/* catch our control-C so we can cleanly exit and close the device */
	(void) signal(SIGINT, ex_program);

	/* Print the versions of our libraries for convenience */
	printf("%-20s %-10s\n", "Lib Flirc Version: ", fl_lib_version());
	printf("%-20s %-10s\n", "Lib IR Version: ", ir_lib_version());

	/**
	 * open the alternate interface which has the ability to poll
	 * for a raw buf.
	 */
	if (fl_open_device_alt(0x20A0, "flirc.tv") < 0) {
		printf("Error: unable to open device\n");
		return -1;
	}

	/**
	 * Only necessary if you want to transmit. Purpose of libir is to
	 * support any transmitter.
	 */
	ir_register_tx(fl_transmit_raw);

	/* check for argument */
	if (argc > 1) {
		if (strcmp(argv[1], "listen") == 0) {
			_listen();
		} else if (strcmp(argv[1], "transmit") == 0) {
			printf("transmitting RC6\n");
			ir_tx(RC_PROTO_RC6_0, 0xC, 3);
			printf("transmitting NEC\n");
			ir_tx(RC_PROTO_NEC, 0x12341, 4);
			printf("transmitting RC5\n");
			ir_tx(RC_PROTO_RC5, 0xD, 5);
			return 0;
		} else if (strcmp(argv[1], "retransmit") == 0) {
			retransmit();
			return 0;
		} else {
			usage();
		}
	}

	usage();
	return 0;
}
