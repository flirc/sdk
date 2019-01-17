=========
Flirc CLI
=========

flirc_util is a generic C project framework based on prjstart by
Robert Curtis. The buildsystem supports. This is the entire source
code for the public flirc_util application that ships with flirc.

This should be used as an exmaple to interface with flirc.

Please see ../lib/flirc.h for provided API functions. Further
documentation will be provided shortly.

For windows installation, please make sure mingw is installed
and in the path. Please use the 32 bit version, to ensure 
compatibility with 64 bit systems. To complie for windows:

    $ make HOSTOS=win

-----------
Buildsystem
-----------

The Makefile and files under `buildsystem' should not need to be
modified. Targets are configured with target makefiles. One is
provided to build flirc_util.

Running make alone will provide a help and list of detected targets.

::

    $ make
    usage: make <target[.config]>
           make TARGETMK=<target makefile> [CONFIG=<config>]
           make all
           make clean
    other options:
           VERBOSE    setting this to 1 enables verbose output
           INSTALL    setting this to 1 runs the install script for
                      each goal specified
    targets:
     * flirc_util

Target Makefiles
++++++++++++++++

TARGET
    The target makefile's basename is used to create this variable unless
    it is defined in the environment.

SOURCES
    List of all sources to compile for the target.

LIBRARIES
    List of all required libraries to link against. Each library
    becomes a -l<library> flag during the link stage.
    NOTE: If you are using the standard C++ libary (e.g. iostream),
    you must add stdc++ to this list.

OPTIONS
    Global build options for all configurations. Each option becomes
    a -D<option> style flag.

FRAMEWORKS
    List of frameworks to build against. (Obj-C / OS X)

All flags can be appended to in the target makefile as well. For
example, `CPPFLAGS += <flag>'


-------
Logging
-------

see include/logging.h

------------
Dependencies
------------

On Ubuntu
    sudo apt-get install libusb-1.0-0-dev libreadline-dev libhidapi-dev
