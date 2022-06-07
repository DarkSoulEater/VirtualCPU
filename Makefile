FILE = 

.PHONY: init clean def BASM BCPU ASM CPU execute
def:
	echo Select build option

build: BASM BCPU

BASM:
	$(MAKE) -C ASM

BCPU:
	$(MAKE) -C CPU

execute:
	echo $(addprefix $(FILE), .as)
	ASM/build-debug/ASM.exe $(addprefix $(FILE), .as)
	CPU/build-debug/CPU.exe $(addprefix $(FILE), .bt)

ASM:
	ASM/build-debug/ASM.exe $(addprefix $(FILE), .as)

CPU:
	CPU/build-debug/CPU.exe $(addprefix $(FILE), .bt)

init:
	if not exist ASM/$(BIN_DIR)    md ASM/$(BIN_DIR)
	if not exist ASM/$(BUILD_PATH) md ASM/$(BUILD_PATH)
	if not exist CPU/$(BIN_DIR)    md CPU/$(BIN_DIR)
	if not exist CPU/$(BUILD_PATH) md CPU/$(BUILD_PATH)

clean:
	if exist $(BIN_DIR) rmdir /S /Q $(BIN_DIR)
	