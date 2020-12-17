APP_VERSION = 2
SOFTDEVICE_REQ = 0xCA
KEY_PATH := ..
KEY_FILE = private.pem

PY_FLASH_ADDRESS_START = 0x60000
PY_FLASH_SIZE = 0x100000
PY_SCRIPT_PATH = littlefs-files/

TOOLCHAIN = GCC_ARM
TARGET = NRF52840_DK

MAKEFILE_PATH = $(abspath $(lastword $(MAKEFILE_LIST)))
MAKEFILE_DIRNAME = $(notdir $(patsubst %/,%,$(dir $(MAKEFILE_PATH))))
OUTPUT_PATH = BUILD/$(TARGET)/$(TOOLCHAIN)
OUTPUT_HEX = $(MAKEFILE_DIRNAME).hex
FULL_OUTPUT_HEX = $(OUTPUT_PATH)/$(OUTPUT_HEX)

HEAPSIZE = 16
MBED_CLI_FLAGS = -j0 --source .
MBED_CLI_FLAGS += -D "CONFIG_MEM_HEAP_AREA_SIZE=(1024*$(HEAPSIZE))"
MBED_CLI_FLAGS += -D "PY_FLASH_ADDRESS_START=$(PY_FLASH_ADDRESS_START)"
MBED_CLI_FLAGS += -D "PY_FLASH_SIZE=$(PY_FLASH_SIZE)"
# MBED_CLI_FLAGS += -D "-Wno-sign-compare"

# MBED_CLI_FLAGS += --source "mbed-os/targets/TARGET_NORDIC/TARGET_NRF5x/TARGET_NRF51/TARGET_MCU_NRF51822_UNIFIED/sdk/softdevice/s130/headers"
# MBED_CLI_FLAGS += --source "mbed-os/targets/TARGET_NORDIC/TARGET_NRF5x/TARGET_NRF51/TARGET_MCU_NRF51822_UNIFIED/sdk"
# MBED_CLI_FLAGS += --source "mbed-os/features/FEATURE_BLE/targets/TARGET_NORDIC/TARGET_NORDIC_SOFTDEVICE/TARGET_MCU_NRF51822/sdk/source/libraries/timer"
# MBED_CLI_FLAGS += --source "mbed-os/targets/TARGET_NORDIC/TARGET_NRF5x/TARGET_SDK_15_0/TARGET_SOFTDEVICE_COMMON/libraries/bootloader"
# MBED_CLI_FLAGS += --source "mbed-os/targets/TARGET_NORDIC/TARGET_NRF5x/TARGET_SDK_15_0/TARGET_SOFTDEVICE_COMMON/libraries/bootloader/dfu"
# MBED_CLI_FLAGS += --source "mbed-os/targets/TARGET_NORDIC/TARGET_NRF5x/TARGET_SDK_11/ble/common"
# MBED_CLI_FLAGS += --source "mbed-os/targets/TARGET_NORDIC/TARGET_NRF5x/TARGET_SDK_11/ble/ble_advertising"
# MBED_CLI_FLAGS += --source "mbed-os/targets/TARGET_NORDIC/TARGET_NRF5x/TARGET_SDK_11/ble/peer_manager"
# MBED_CLI_FLAGS += --source "mbed-os/targets/TARGET_NORDIC/TARGET_NRF5x/TARGET_SDK_11/ble/ble_services/ble_dfu"
# MBED_CLI_FLAGS += --source "mbed-os/targets/TARGET_NORDIC/TARGET_NRF5x/TARGET_SDK_11/drivers_nrf/clock"
# MBED_CLI_FLAGS += --source "mbed-os/targets/TARGET_NORDIC/TARGET_NRF5x/TARGET_SDK_11/drivers_nrf/config"
# MBED_CLI_FLAGS += --source "mbed-os/targets/TARGET_NORDIC/TARGET_NRF5x/TARGET_SDK_11/drivers_nrf/common"


COMPILE_PREFFIX = \
	@echo "before compile cpp";
COMPILE_SUFFIX = \
	@echo "after compile cpp"; 
	# \
	# python3 mbed-littlefs2/tools/mklittlefs.py $(OUTPUT_PATH)/fs.bin $(PY_SCRIPT_PATH); \
	# objcopy --change-addresses $(PY_FLASH_ADDRESS_START) -I binary -O ihex $(OUTPUT_PATH)/fs.bin $(OUTPUT_PATH)/fs.hex; \
	# mergehex -m $(OUTPUT_PATH)/fs.hex $(FULL_OUTPUT_HEX) -o $(FULL_OUTPUT_HEX);

clean:
	rm -f -r BUILD;

init: clean
	mbed config root .
	mbed toolchain $(TOOLCHAIN)
	mbed target $(TARGET)
	mbed deploy

cmphex:
	$(call COMPILE_PREFFIX)
	mbed compile $(MBED_CLI_FLAGS)
	$(call COMPILE_SUFFIX)
	nrfutil pkg generate --application $(FULL_OUTPUT_HEX) --application-version $(APP_VERSION) \
	--hw-version 52 --sd-req 0xCA --key-file $(KEY_PATH)/$(KEY_FILE) $(OUTPUT_PATH)/package.zip
	sudo nrfutil dfu usb-serial -pkg $(OUTPUT_PATH)/package.zip -p /dev/ttyACM1 -b 115200
	# nrfjprog --reset --program $(FULL_OUTPUT_HEX) --chiperase

compile: $(KEY_PATH)/$(KEY_FILE)
	$(call COMPILE_PREFFIX) 
	mbed compile $(MBED_CLI_FLAGS) 
	$(call COMPILE_SUFFIX) 
	nrfutil pkg generate --application $(FULL_OUTPUT_HEX) --application-version $(APP_VERSION) \
	--hw-version 52 --sd-req $(SOFTDEVICE_REQ) --key-file $(KEY_PATH)/$(KEY_FILE) $(OUTPUT_PATH)/package.zip
	gio copy $(OUTPUT_PATH)/package.zip $(shell echo /run/user/1000/gvfs/*/|head -n 1)Phone/Download

compile-old: $(KEY_PATH)/$(KEY_FILE)
	$(call COMPILE_PREFFIX) 
	mbed compile $(MBED_CLI_FLAGS) 
	$(call COMPILE_SUFFIX) 
	nrfutil pkg generate --application $(FULL_OUTPUT_HEX) --application-version $(APP_VERSION) \
	--hw-version 52 --sd-req $(SOFTDEVICE_REQ) $(OUTPUT_PATH)/package.zip

sd:
	nrfutil pkg generate --softdevice s140_nrf52_7.0.1_softdevice.hex $(OUTPUT_PATH)/package.zip \
	--hw-version 52 --sd-req 0x00
	nrfutil dfu serial -pkg $(OUTPUT_PATH)/package.zip -p /dev/ttyACM0 -b 115200

flash:
	nrfutil dfu serial -pkg $(OUTPUT_PATH)/package.zip -p /dev/ttyACM0 -b 115200


test3:
	nrfutil pkg generate --application $(FULL_OUTPUT_HEX) --application-version $(APP_VERSION) \
	--hw-version 52 --sd-req $(SOFTDEVICE_REQ) --key-file $(KEY_PATH)/$(KEY_FILE) $(OUTPUT_PATH)/package.zip
	gio copy $(OUTPUT_PATH)/package.zip $(shell echo /run/user/1000/gvfs/*/|head -n 1)Phone/Download

test:
	cp $(OUTPUT_PATH)/package.zip /media/sf_common

test2:
	nrfutil pkg generate --application /media/sf_common/ble_app_buttonless_dfu_pca10056_s140.hex --application-version $(APP_VERSION) \
	--hw-version 52 --sd-req $(SOFTDEVICE_REQ) --key-file $(KEY_PATH)/$(KEY_FILE) $(OUTPUT_PATH)/package.zip

p:
	particle compile argon particle --saveTo particle.bin

dfu:
	particle compile argon particle --saveTo particle.bin
	particle usb dfu e00fce68300279b8bd04d2d2
	# particle usb dfu e00fce68ab9b36d26123999e
	particle flash --usb particle.bin


# nrfjprog -f NRF52 --program backupuicr.hex --chiperase
# nrfjprog -f NRF52 --program backupcode.hex
# 000002614 [hal] INFO: :TROLOLO: AT+CMUX=0
# 0000002614 [gsm0710muxer] INFO: Starting GSM07.10 muxer
# 0000002615 [gsm0710muxer] INFO: Openning mux channel 0
# 0000002615 [gsm0710muxer] INFO: GSM07.10 muxer thread started
# 0000002616 [gsm0710muxer] INFO: :TROLOLO: sendC ch=0 ctl=63 cmd=1 sz=0
# data=0
# 0000002619 [gsm0710muxer] INFO: :TROLOLO: processInputData newDATA=4 #=1
# 0000002620 [gsm0710muxer] INFO: :TROLOLO: processInputData newDATA=2 #=2
# 0000002620 [gsm0710muxer] INFO: Openning mux channel 1
# 0000003885 [gsm0710muxer] INFO: :TROLOLO: sendC ch=1 ctl=63 cmd=1 sz=0
# data=0                                              
# 0000003887 [gsm0710muxer] INFO: :TROLOLO: processInputData newDATA=4 #=3
# 0000003887 [gsm0710muxer] INFO: :TROLOLO: processInpu0000003888 [gsm07102
# 0000003889 [gsm0710muxer] INFO: :TROLOLO: sendC ch=1 ctl=239 cmd=0 sz=2
# data=AT
# 0000003889 [gsm0710muxer] INFO: 1 byte = 41
# FO: :TROLOLO: sendC ch=1 ctl=239 cmd=0 sz=2
# data=

# 0000003892 [gsm0710muxer] INFO: 1 byte = d
# 2 byte = a

# sz=4                                                                     
# >ta=�                                                                   
# 0000003896 [gsm0710muxer] INFO: :TROLOLO: processInputData newDATA=20 #=6
# 0000003898 [gsm0710muxer] INFO: :TROLOLO: writeChannel ch=1 sz=13        
# 0000003898 [gsm0710muxer] INFO: :TROLOLO: sendC ch=1 ctl=239 cmd=0 sz=13 
# data=AT+CWDHCP=0,3                                                       
# 0000003899 [gsm0710muxer] INFO: :TROLOLO: writeChannel ch=1 sz=2         
# 0000003900 [gsm0710muxe03901 [gsm0710muxer] INFO: 1 byte = d             
# 2 byte = a                                                               
                                                                         
# 0000003900 [gsm0710muxer] INFO: :TROLOLO: processInputData newDATA=19 #=7
# 0000003903 [gsm0710muxer] INFO: :TROLOLO: processInputData newDATA=4 #=8 
# 0000003904 [gsm0710muxer] INFO: :TROLOLO: processInputData newDATA=4 #=9 
# :TROLOLO: processInputData newDATA=4 #=10                                
# 0000003906 [gsm0710mu0000003908 [gsm0710muxer] INFO: :TROLOLO: sendC ch=1
# data=AT+GETMAC=0                                                         
# 0000003909 [gsm0710muxer] INFO: :TROLOLO: writeChannel ch=1 sz=2         
# 0000003909 [gsm0710muxer] INFO: :TROLOLO: sendC ch=1 ctl=239 cmd=0 sz=2  
# data=                                                                    
                                                                         
# 0000003910 [gsm0710muxer] INFO: 1 byte = d                               
# 2 byte = a                                