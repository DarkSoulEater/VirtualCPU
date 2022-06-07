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
	$(MAKE) init -C CPU
	$(MAKE) init -C ASM

clean:
	if exist $(BIN_DIR) rmdir /S /Q $(BIN_DIR)
	