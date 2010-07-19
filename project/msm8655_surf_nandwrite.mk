# top level project rules for the msm8655_surf_nandwrite project
#
LOCAL_DIR := $(GET_LOCAL_DIR)

TARGET := msm8655_surf

MODULES += app/nandwrite

DEFINES += WITH_DEBUG_JTAG=1
DEFINES += ENABLE_NANDWRITE=1
#DEFINES += WITH_DEBUG_DCC=1
#DEFINES += WITH_DEBUG_UART=1
#DEFINES += WITH_DEBUG_FBCON=1

