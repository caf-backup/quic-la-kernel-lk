LOCAL_DIR := $(GET_LOCAL_DIR)

INCLUDES += -I$(LOCAL_DIR)/include -I$(LK_TOP_DIR)/platform/msm_shared

PLATFORM := msm8974

MEMBASE := 0x0F900000 # SDRAM
MEMSIZE := 0x00100000 # 1MB

BASE_ADDR        := 0x00000

TAGS_ADDR        := BASE_ADDR+0x00000100
KERNEL_ADDR      := BASE_ADDR+0x00008000
RAMDISK_ADDR     := BASE_ADDR+0x01000000
SCRATCH_ADDR     := 0x11000000

SIGNING_TOOL_PATH := $(LK_TOP_DIR)/signing/gensecimage/

#DEFINES += DISPLAY_SPLASH_SCREEN=1
DEFINES += DISPLAY_TYPE_MIPI=1
DEFINES += DISPLAY_TYPE_DSI6G=1
DEFINES += CONFIG_ARCH_MSM8974_APOLLO=1

MODULES += \
	dev/keys \
	dev/pmic/pm8x41 \
	dev/backlight/lp855x \
	dev/panel/msm \
    lib/ptable \
    lib/libfdt

DEFINES += \
	MEMSIZE=$(MEMSIZE) \
	MEMBASE=$(MEMBASE) \
	BASE_ADDR=$(BASE_ADDR) \
	TAGS_ADDR=$(TAGS_ADDR) \
	KERNEL_ADDR=$(KERNEL_ADDR) \
	RAMDISK_ADDR=$(RAMDISK_ADDR) \
	SCRATCH_ADDR=$(SCRATCH_ADDR)


OBJS += \
    $(LOCAL_DIR)/init.o \
    $(LOCAL_DIR)/meminfo.o \
    $(LOCAL_DIR)/target_display.o
