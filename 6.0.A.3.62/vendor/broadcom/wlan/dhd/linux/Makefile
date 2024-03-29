
# GNU Makefile for Broadcom Dongle Host Driver
#
# Copyright (C) 1999-2011, Broadcom Corporation
#
#         Unless you and Broadcom execute a separate written software license
# agreement governing use of this software, this software is licensed to you
# under the terms of the GNU General Public License version 2 (the "GPL"),
# available at http://www.broadcom.com/licenses/GPLv2.php, with the
# following added to such license:
#
#      As a special exception, the copyright holders of this software give you
# permission to link this software with independent modules, and to copy and
# distribute the resulting executable under terms of your choice, provided that
# you also meet, for each linked independent module, the terms and conditions of
# the license of that module.  An independent module is a module which is not
# derived from this software.  The special exception does not apply to any
# modifications of the software.
#
#      Notwithstanding the above, under no circumstances may you combine this
# software in any way with any other Broadcom software provided under a license
# other than the GPL, without Broadcom's express prior written consent.
#
# $Id: Makefile,v 1.93.6.24 2011-02-10 02:31:08 $
#

# Although 32ON64 flag is supposed to be passed from caller, set it
# if not set by caller on certain kernels and platforms
ifeq ($(shell uname -m),x86_64)
     ifneq ($(findstring x86_64,$(LINUXVER)),x86_64)
          export 32ON64=1
          # $(warning Passing 32ON64 flag to force 32bit driver for $(LINUXVER))
     endif # LINUXVER
endif # CURPROC

# Try a couple of places for LINUXDIR if not specified
ifeq ($(LINUXDIR),)
ifeq ($(LINUXVER),)
LINUXVER := 2.6.35.11-3120
endif
LINUXDIR := $(ANDROID_PRODUCT_OUT)/obj/KERNEL_OBJ
LINUXDIR_H := $(ANDROID_BUILD_TOP)/kernel
endif

ifeq ($(LINUXVER),)
     $(error LINUXVER=$(LINUXVER) is empty)
endif # LINUXVER

# Keep consistency with Linux kernel naming rules
KRNL_VERSION	:= $(shell echo $(LINUXVER) | sed -n 's/^\([0-9]\)\..*/\1/p')
ifneq ($(KRNL_VERSION),2)
KRNL_PATCHLEVEL	:= $(shell echo $(LINUXVER) | sed -n 's/^3\.\([0-9]\+\).*/\1/p')
else
KRNL_PATCHLEVEL	:= $(shell echo $(LINUXVER) | sed -n 's/^2\.\([0-9]\+\).*/\1/p')
KRNL_SUBLEVEL	:= $(shell echo $(LINUXVER) | cut -c5-6)
endif

KERNELVER_2_5_0_OR_LATER := $(strip $(shell \
		if [ $(KRNL_VERSION) -lt 3 ]; then \
			if [ $(KRNL_PATCHLEVEL) -lt 5 ]; then \
				echo FALSE; \
			else \
				echo TRUE; \
			fi \
		else \
			echo TRUE; \
		fi \
	))

# 2.6.37 kernel and older kernel need compat wireless
KRNL_NEED_COMPAT_WIRELESS := $(strip $(shell \
		if [ $(KRNL_VERSION) -eq 2 ]; then \
			if [ $(KRNL_PATCHLEVEL) -eq 6 ]; then \
				if [ $(KRNL_SUBLEVEL) -le 37 ]; then \
					echo TRUE; \
				else \
					echo FALSE; \
				fi \
			else\
				echo FALSE; \
			fi \
		else \
			echo FALSE; \
		fi \
	))

KERNELVER_2_6_35_OR_LATER := $(strip $(shell \
		if [ $(KRNL_VERSION) -lt 3 ]; then \
			if [ $(KRNL_PATCHLEVEL) -ge 6 ]; then \
				if [ $(KRNL_SUBLEVEL) -ge 35 ]; then \
					echo TRUE; \
				else \
					echo FALSE; \
				fi \
			else \
				echo FALSE; \
			fi \
		else \
			echo TRUE; \
		fi \
	))

# Allow CROSS_COMPILE to specify compiler base
ifeq ($(CROSS_COMPILE),)
CROSS_COMPILE := arm-eabi-
endif
CC := $(CROSS_COMPILE)gcc
LD := $(CROSS_COMPILE)ld
NM := $(CROSS_COMPILE)nm
OBJCOPY := $(CROSS_COMPILE)objcopy

# driver source base and C file path
ifeq ($(SRCBASE),)
SRCBASE := $(shell /bin/pwd)/../..
endif
vpath %.c $(SRCBASE)/dhd/sys $(SRCBASE)/shared $(SRCBASE)/bcmsdio/sys $(SRCBASE)/wl/sys $(SRCBASE)/wl/phy

## Initialize DFLAGS
DFLAGS :=
CFILES :=


# basic options (defines in DFLAGS, includes in IFLAGS)
DFLAGS += -DLINUX -DSRCBASE=\"$(SRCBASE)\" -DBCMDRIVER -DBCMDONGLEHOST
DFLAGS += -DUNRELEASEDCHIP -DBCMDMA32
DFLAGS += -DWLBTAMP
DFLAGS += -DBCMFILEIMAGE
#DFLAGS += -DPROP_TXSTATUS
ifeq ($(BCMQT),1)
	DFLAGS += -DBCMSLTGT -DBCMQT
endif
ifeq ($(WLTEST),1)
	DFLAGS += -DIOCTL_RESP_TIMEOUT=20000
	DFLAGS += -DDHD_SPROM
endif

# Past 2.6.29 kernels, arch specific bits are re-organized in linux kernel. So
# append new include paths to existing ones to get 2.6.29+ kernels compile

# First include from linux kernel dirs
IFLAGS := -I$(LINUXDIR_H)/include
IFLAGS += -I$(LINUXDIR_H)/arch/arm/include

# From current workspace
IFLAGS += -I.
IFLAGS += -I$(SRCBASE)/include
IFLAGS += -I$(SRCBASE)/shared
IFLAGS += -I$(SRCBASE)/dhd/sys
IFLAGS += -I$(SRCBASE)/dongle
IFLAGS += -I$(SRCBASE)/wl/sys
IFLAGS += -I$(SRCBASE)/wl/phy

ifneq ($(wildcard $(LINUXDIR)/.config),)
include $(LINUXDIR)/.config
else
# This is dangerous, since we don't know if they are really configured.
CONFIG_WIRELESS_EXT=y
DFLAGS += -DCONFIG_WIRELESS_EXT
endif

ifeq ($(CONFIG_MMC_MSM7X00A),y)
DFLAGS += -Dlinux
DFLAGS += -DDHD_SDALIGN=64 -DMAX_HDR_READ=64 -DDHD_FIRSTREAD=64
endif

WFLAGS := -Wall -Wstrict-prototypes
ifeq (,$(findstring 2.4.18,$(LINUXVER)))
WFLAGS += -Werror
endif

CFILES += dhd_linux.c dhd_linux_sched.c linux_osl.c bcmutils.c bcmevent.c dhd_common.c
CFILES += dhd_custom_gpio.c siutils.c sbutils.c aiutils.c hndpmu.c dhd_bta.c

# threading options
ifeq ($(findstring -nothread-,-$(TARGET)-),)
DFLAGS += -DDHDTHREAD
endif

# Building hsic for 43239, 43236 and 4330
ifneq ($(findstring -hsic-,-$(TARGET)-),)
DFLAGS += -DEXTENDED_VID_PID=' { USB_DEVICE(0x0a5c, 0xbd1b) }, \
                               { USB_DEVICE(0x0a5c, 0xbd17) }, \
                                { USB_DEVICE(0x0a5c, 0xbd19) }'
endif
ifeq ($(WLTEST),1)
	CFILES += bcmsrom.c bcmotp.c
endif

ifeq ($(CONFIG_NET_RADIO),y)
CFILES += wl_iw.c bcmwifi.c
else
	ifeq ($(CONFIG_WIRELESS_EXT),y)
	CFILES += wl_iw.c bcmwifi.c
	endif
endif

OFILES=$(CFILES:.c=.o)

# Make debug a separate option
ifneq ($(findstring -debug-,-$(TARGET)-),)
DFLAGS += -DDHD_DEBUG
endif

# enable queuing in host
ifneq ($(findstring -sdstd-,-$(TARGET)-),)
DFLAGS += -DPROP_TXSTATUS
ifneq ($(findstring -dbgwlfc-,-$(TARGET)-),)
DFLAGS += -DPROP_TXSTATUS_DEBUG
endif
endif

# Make big-endian a separate option
ifneq ($(findstring -be-,-$(TARGET)-),)
DFLAGS += -DIL_BIGENDIAN
endif

ifneq ($(findstring -dnglimage-,-$(TARGET)-),)
## Embeddable dongle image name
DNGL_IMAGE_NAME ?= 4325b0/sdio-g-cdc-reclaim-idsup-wme
DFLAGS += -DBCM_DNGL_EMBEDIMAGE -DIMAGE_NAME="$(DNGL_IMAGE_NAME)"
IFLAGS += -I$(SRCBASE)/dongle/rte/wl/builds/$(DNGL_IMAGE_NAME)
endif

ifneq ($(findstring -cdc-,-$(TARGET)-),)
DFLAGS += -DBDC -DTOE
DFLAGS += -DDHD_BCMEVENTS -DSHOW_EVENTS
CFILES += dhd_cdc.c
endif
ifneq ($(findstring -rndis-,-$(TARGET)-),)
DFLAGS += -DRNDIS
CFILES += dhd_rndis.c
endif

ifneq ($(findstring -dbususb-,-$(TARGET)-),)
DFLAGS += -DBCMDHDUSB -DBCMDBUS
CFILES += dbus.c dbus_usb.c dbus_usb_linux.c
endif
ifneq ($(findstring -dbususb-dnglimage-,-$(TARGET)-),)
DFLAGS += -DBCMDHDUSB -DBCMDBUS
DFLAGS += -DBCM_DNGL_EMBEDIMAGE
CFILES += dbus.c dbus_usb.c dbus_usb_linux.c
endif
ifneq ($(findstring -usb-,-$(TARGET)-),)
DFLAGS += -DBCMDHDUSB -DBCMDBUS
CFILES += dbus.c dbus_usb.c dbus_usb_linux.c
endif
ifneq ($(findstring -dbus-,$(TARGET)-),)
DFLAGS += -DBCMDBUS -DBCMSDIO
CFILES += dbus.c dbus_sdio.c dbus_sdio_linux.c
endif
ifneq ($(findstring -dbussdstd-,$(TARGET)-),)
DFLAGS += -DBCMSDIO -DBCMSDIOH_STD
CFILES += bcmsdh.c bcmsdstd.c bcmsdstd_linux.c bcmsdh_linux.c
DFLAGS += -DBCMDBUS
CFILES += dbus.c dbus_sdio.c dbus_sdio_linux.c
endif
ifneq ($(findstring -sdio-,-$(TARGET)-),)
DFLAGS += -DBCMSDIO
CFILES += dhd_sdio.c
endif
ifneq ($(findstring -sdstd-,$(TARGET)-),)
DFLAGS += -DBCMSDIO -DBCMSDIOH_STD
CFILES += dhd_sdio.c bcmsdh.c bcmsdstd.c bcmsdstd_linux.c bcmsdh_linux.c
ifneq ($(findstring -hc3-,$(TARGET)-),)
DFLAGS += -DSDHOST3=1
endif
endif
ifneq ($(findstring -nexus-,-$(TARGET)-),)
CFILES += wldev_common.c wl_android.c
CFILES += dhd_linux_mon.c
DFLAGS += -Dlinux
DFLAGS += -DOEM_ANDROID
DFLAGS += -DBCMDBG
DFLAGS += -DDHD_USE_STATIC_BUF
DFLAGS += -DCUSTOMER_HW
DFLAGS += -DCUSTOM_OOB_GPIO_NUM=128
DFLAGS += -DOOB_INTR_ONLY
DFLAGS += -DMMC_SDIO_ABORT
#DFLAGS += -DCONFIG_FIRST_SCAN
DFLAGS += -DKEEP_ALIVE
DFLAGS += -DBCMSDIO -DBCMLXSDMMC -DBCMPLATFORM_BUS -DEMBEDDED_PLATFORM

ifeq ($(KERNELVER_2_6_35_OR_LATER), TRUE)
$(warning : extra defintion for Android Gingerbread **)
DFLAGS += -DHW_OOB
DFLAGS += -DCSCAN
DFLAGS += -DARP_OFFLOAD_SUPPORT
endif
ifeq ($(CONFIG_WIRELESS_EXT),y)
DFLAGS += -DSOFTAP
endif
CFILES += dhd_sdio.c bcmsdh_sdmmc.c bcmsdh.c bcmsdh_linux.c bcmsdh_sdmmc_linux.c

ifneq ($(findstring -cfg80211-,-$(TARGET)-),)

ifndef COMPAT_WIRELESS
ifeq ($(KRNL_NEED_COMPAT_WIRELESS), TRUE)
$(warning : old Kernel needs compat-wireless**)
COMPAT_WIRELESS=/projects/hnd/tools/linuxwireless/compat-wireless
export COMPAT_WIRELESS
endif
endif

DFLAGS += -DWL_CFG80211
DFLAGS += -DWLP2P -DPNO_SUPPORT
ifdef COMPAT_WIRELESS
DFLAGS += -DWL_COMPAT_WIRELESS
endif

CFILES += wl_cfg80211.c
CFILES += wl_cfgp2p.c
CFILES += wldev_common.c
CFILES += bcmwifi.c
endif

else
ifneq ($(findstring -oob-,-$(TARGET)-),)
DFLAGS += -DOOB_INTR_ONLY
DFLAGS += -DHW_OOB
DFLAGS += -DMMC_SDIO_ABORT
else
ifneq ($(findstring -sdmmc-,-$(TARGET)-),)
ifeq ($(findstring -prime-,-$(TARGET)-),)
DFLAGS += -DBCMSDIO -DBCMLXSDMMC -DBCMPLATFORM_BUS -DSDIO_ISR_THREAD
endif
CFILES += dhd_sdio.c bcmsdh_sdmmc.c bcmsdh.c bcmsdh_linux.c bcmsdh_sdmmc_linux.c
endif
endif
endif

DFLAGS += -DCONFIG_WIFI_CONTROL_FUNC
DFLAGS += -DPKT_FILTER_SUPPORT
DFLAGS += -DSOFTAP
DFLAGS += -DGET_CUSTOM_MAC_ENABLE
DFLAGS += -DWAPI

ifneq ($(findstring -dk8k-,-$(TARGET)-),)
DFLAGS += -Dlinux
DFLAGS += -DOEM_ANDROID
DFLAGS += -DMMC_SDIO_ABORT
DFLAGS += -DBCMSDIO -DBCMLXSDMMC -DBCMPLATFORM_BUS
endif

ifneq ($(findstring -prime-,-$(TARGET)-),)
$(info : ******* prime build **********************)
CFILES += wldev_common.c wl_android.c
CFILES += dhd_linux_mon.c
CFILES += wl_cfg80211.c
CFILES += wl_cfgp2p.c
CFILES += bcmwifi.c
DFLAGS += -Dlinux
DFLAGS += -DOEM_ANDROID
DFLAGS += -DMMC_SDIO_ABORT
DFLAGS += -DWL_CFG80211
DFLAGS += -DCUSTOM_OOB_GPIO_NUM=2
DFLAGS += -DBCMDRIVER -DBCMDONGLEHOST -DUNRELEASEDCHIP -DBCMDMA32 -DWLBTAMP   \
	-DDHDTHREAD -DDHD_GPL -DDHD_SCHED -DDHD_DEBUG -DSDTEST -DBDC -DTOE    \
	-DDHD_BCMEVENTS -DSHOW_EVENTS -DDONGLEOVERLAYS -DBCMDBG               \
	-DCUSTOMER_HW2 -DCUSTOM_OOB_GPIO_NUM=2 -DOOB_INTR_ONLY -DHW_OOB       \
	-DMMC_SDIO_ABORT -DBCMSDIO -DBCMLXSDMMC -DBCMPLATFORM_BUS -DWLP2P     \
	-DNEW_COMPAT_WIRELESS -DWIFI_ACT_FRAME -DARP_OFFLOAD_SUPPORT          \
	-DKEEP_ALIVE -DCSCAN -DGET_CUSTOM_MAC_ENABLE -DPKT_FILTER_SUPPORT     \
	-DEMBEDDED_PLATFORM -DENABLE_INSMOD_NO_FW_LOAD -DPNO_SUPPORT          \
	-DBCMFILEIMAGE
endif

ifneq ($(findstring -sdspi-,$(TARGET)-),)
DFLAGS += -DBCMSDIO -DBCMSDIOH_SPI -DTESTDONGLE # -DBCMSDYIELD
CFILES += dhd_sdio.c bcmsdh.c bcmsdspi.c bcmsdspi_linux.c bcmsdh_linux.c
endif
ifneq ($(findstring -gspi-,$(TARGET)-),)
DFLAGS += -DBCMSDIO -DBCMSDIOH_SPI -DTESTDONGLE -DBCMSPI
CFILES += dhd_sdio.c bcmsdh.c bcmspibrcm.c bcmsdspi_linux.c bcmsdh_linux.c
endif
ifneq ($(findstring -gspibcm,$(TARGET)-),)
DFLAGS += -DBCMSDIO -DBCMSDIOH_SPI -DTESTDONGLE -DBCMSPI
CFILES += dhd_sdio.c bcmsdh.c bcmgspi.c bcmsdbrcm_linux.c bcmsdh_linux.c hnddma.c
endif
ifneq ($(findstring -pci,$(TARGET)-),)
CFILES += bcmpcispi.c
endif
ifneq ($(findstring -sdext-,$(TARGET)-),)
DFLAGS += -DBCMSDIO -DTESTDONGLE
CFILES += dhd_sdio.c
endif

ifneq ($(findstring -n2dbg-,$(TARGET)-),)
DFLAGS += -DWLMEDIA_HTSF -DHOST_TS
endif

#ifdef WIFI_ACT_FRAME
DFLAGS += -DWIFI_ACT_FRAME
#endif

CFLAGS += -fshort-wchar $(DFLAGS) $(WFLAGS) $(IFLAGS)

LDFLAGS := -r
MODULES := dhd.o
ifeq ($(KERNELVER_2_5_0_OR_LATER), TRUE)
  ##Kernel module names in 2.6 kernel have .ko suffix
  KMODULES:=dhd.ko
  ifneq ($(findstring -nexus-,$(TARGET)-),)
    KMODULES:=bcm4330.ko
  endif
  ifneq ($(findstring -prime-,$(TARGET)-),)
      KMODULES:=bcmdhd.ko
  endif
else
  KMODULES:=$(MODULES)
endif

# host options
HOSTCC := $(CC)
ifeq ($(KERNELVER_2_5_0_OR_LATER), FALSE)
  HOSTCFLAGS := $(CFLAGS) $(shell $(MAKE) --no-print-directory -s -C $(LINUXDIR) script 'SCRIPT=@echo $$(CFLAGS) $$(MODFLAGS)')
else
  HOSTCFLAGS := $(CFLAGS) -D__KERNEL__
  DHDCFLAGS = $(HOSTCFLAGS) -I$(shell pwd)
  export DHDCFLAGS
  DHDOFILES = $(OFILES)
  export DHDOFILES
endif

TARGETS := \
	dhd-cdc-usb dhd-cdc-dbususb dhd-cdc-dbususb-dnglimage dhd-cdc-sdio dhd-cdc-dbus dhd-cdc-sdstd \
	dhd-cdc-dbussdstd dhd-cdc-sdspi-pci dhd-cdc-sdmmc-nexus-gpl dhd-cdc-sdmmc-nexus-cfg80211-gpl\
	dhd-cdc-sdmmc-gpl-dk8k-apsta-btamp \
	dhd-cdc-sdmmc-gpl-dk8k-debug \
	dhd-cdc-sdmmc-prime-cfg80211-gpl-debug \
	dhd-cdc-sdstd-nothread dhd-cdc-sdmmc-gpl \
	dhd-cdc-usb-gpl dhd-cdc-usb-hsic-gpl dhd-cdc-usb-hsic-padwar-gpl

TARGETS += dhd-cdc-usb-gpl-apsta
TARGETS += dhd-cdc-sdstd-apsta
TARGETS += \
	dhd-cdc-sdio-dnglimage \
	dhd-cdc-sdstd-dnglimage dhd-cdc-dbussdstd-dnglimage \
	dhd-cdc-dbus-dnglimage dhd-cdc-sdspi-pci-dnglimage \
	dhd-cdc-gspi-pci \
	dhd-cdc-gspibcm \
	dhd-cdc-sdstd-hc3 \
	dhd-cdc-sdstd-n2dbg \
	dhd-cdc-sdstd-n2dbg-debug-nothread \
	dhd-cdc-sdstd-debug-nothread \
	dhd-cdc-sdstd-largemtu \
	dhd-cdc-sdstd-debug-largemtu

#ifdef RNDIS
TARGETS += dhd-rndis-usb
#endif
TARGETS += dhd-cdc-sdext-be
TARGETS += dhd-cdc-sdext-be-dnglimage
TARGETS += $(foreach tgt, $(TARGETS), $(tgt)-debug)

OBJDIR=dhd-android

all: dhd-cdc-sdmmc-nexus-gpl-debug

# Allow making target with the LINUXVER suffix already on it.
# (Typical of command line tab completion; trailing slash still not allowed)
%-$(LINUXVER): force
	$(MAKE) $(@:%-$(LINUXVER)=%)

$(TARGETS):
	@echo "MAKING $@"
	$(MAKE) TARGET=$@ objdir

# Show compiler version, for the current target build
showenv:
	@echo "CC = $(CC) (ver=`$(CC) -dumpversion`; host=`hostname`; processor=`uname -m`)"

objdir: showenv
	@echo "Making objdir $(OBJDIR)"
	@echo "TARGET is $(TARGET)"
	mkdir -p $(OBJDIR)
ifeq ($(KERNELVER_2_5_0_OR_LATER), FALSE)
	$(MAKE) -C $(OBJDIR) -f $(SRCBASE)/dhd/linux/Makefile SRCBASE=$(SRCBASE) dep
endif
	$(MAKE) -C $(OBJDIR) -f $(SRCBASE)/dhd/linux/Makefile SRCBASE=$(SRCBASE) modules
ifeq ($(KERNELVER_2_5_0_OR_LATER), TRUE)
	$(OBJCOPY) --strip-unneeded $(OBJDIR)/$(KMODULES) $(OBJDIR)/$(KMODULES).stripped
else
	$(OBJCOPY) --strip-unneeded $(OBJDIR)/dhd.o $(OBJDIR)/dhd.o.stripped
endif

dep: $(foreach file,$(CFILES),.$(file).depend)
.%.c.depend: %.c
	$(HOSTCC) $(HOSTCFLAGS) -M $< > $@ || (rm -f $@; exit 1)
.%.c.depend::
	touch $@

ifeq ($(KERNELVER_2_5_0_OR_LATER), TRUE)
modules: $(OFILES)
	test -r ./Makefile || ln -s $(SRCBASE)/dhd/linux/makefile.26 ./Makefile
	$(MAKE) -C $(LINUXDIR) M=$(shell pwd) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) $(if $(VERBOSE),V=1) modules
else
modules: $(MODULES)
endif

dhd.o: $(OFILES)
	$(LD) $(LDFLAGS) -o $@ $^

ifeq ($(KERNELVER_2_5_0_OR_LATER), TRUE)
%.o: %.c
  # when make is called from 2.6, vpath doesn't work so we need to link the files.
	test -r ./$< || ln -s $< .
else
%.o: %.c
	$(HOSTCC) $(HOSTCFLAGS) -c -o $@ $<
	@( \
	echo 'ifneq ($$(HOSTCFLAGS),$(HOSTCFLAGS))' ; \
	echo '$@: force' ; \
	echo 'endif' ; \
	) > .$*.c.flags
endif

force:

clean:
	rm -rf dhd-*

ifneq ($(wildcard .*.depend),)
include $(wildcard .*.depend)
endif
ifneq ($(wildcard .*.flags),)
include $(wildcard .*.flags)
endif
