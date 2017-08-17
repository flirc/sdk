#!/bin/bash
################################################################################
# m
#
#    This is a simple script interface to the prjstart buildsystem.
#
################################################################################

SCRIPTROOT=$(dirname "$0")
cd "$SCRIPTROOT"

make "$@" 2>&1 | ./buildsystem/colorizebuild
