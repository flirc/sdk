FLIRC IR
========

Minimal example demo of using ir.h which will help decode and encode IR.

Use a standardized struct to get an IR buffer from a flirc device. Poll
the device which will return 1 when a new frame is ready.

The frame is buf, the length of the buf, and the time elapsed since the last
frame. The time since last frame is often used in the decoder, but not always
necessary.

Pass this struct to a decoder along with another reference object which will be 
filled out with useful decoded information. 

int ir_decode_packet(struct ir_packet *ir, struct ir_prot *p);

The most important parts are the enum value returned from ir_decode_packet,
as well as the scancode, which is the decoded 32 bit value hex information. 

To transmit a protocol and signal, only two things are needed. The enum value
representing the encoding (i.e RC5), and the scancode. Use the following
helper function:

int fl_transmit_prot(enum rc_proto protocol, uint32_t scancode, uint8_t repeat)

This helper function will encode the packet into the raw timing buffer,
the length, and insert the interpacket spacing needed as per the protocol before
sending it to a flirc usb device for transmission.

ir_encode will also give us the raw timing buffer of a given protocol scancode,
which you can be used directly with fl_transmit_raw as needed. However,
the helper function was written because ir_encode should be called before every
ir transmission, even if the IR protocol and scancode has not changede. 
The reason being that RC5 has a toggle bit which changes the raw timing buffer
on each successful call of the same button press. ir_encode ensures this 
toggle bit is managed for us.

Buildsystem
-----------

Running make alone will build our example app and link in our libraries.

    $ make
    usage: make 
           make ir
           make clean
    other options:
           VERBOSE    setting this to 1 enables verbose output
           HOSTOS     specify: WIN, LINUX, DARWIN, LIBREELEC
