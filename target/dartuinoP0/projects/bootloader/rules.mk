LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

MODULE_SRCS += \
	$(LOCAL_DIR)/bootloader_stubs.c \
	$(LOCAL_DIR)/usb.c


MODULE_DEPS += \
	lib/fs

include make/module.mk
