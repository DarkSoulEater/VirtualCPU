FILE = 
SFML_PATH = C:/SFML-2.5.1

BIN_DIR = bin
BUILD_PATH = build-release

.PHONY: init clean def BASM BCPU ASM CPU execute
def:
	echo Select build option

build: BASM BCPU

BASM:
	$(MAKE) SFML_PATH=$(SFML_PATH) -C ASM

BCPU:
	$(MAKE) SFML_PATH=$(SFML_PATH) -C CPU

execute:
	ASM/$(BUILD_PATH)/ASM.exe $(addprefix $(FILE), .as)
	CPU/$(BUILD_PATH)/CPU.exe $(addprefix $(FILE), .bt)

ASM:
	ASM/$(BUILD_PATH)/ASM.exe $(addprefix $(FILE), .as)

CPU:
	CPU/$(BUILD_PATH)/CPU.exe $(addprefix $(FILE), .bt)

init:
	if not exist "ASM/$(BIN_DIR)"    md "ASM/$(BIN_DIR)"
	if not exist "ASM/$(BUILD_PATH)" md "ASM/$(BUILD_PATH)"
	if not exist "CPU/$(BIN_DIR)"    md "CPU/$(BIN_DIR)"
	if not exist "CPU/$(BUILD_PATH)" md "CPU/$(BUILD_PATH)"

clean:

	