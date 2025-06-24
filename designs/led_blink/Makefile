## Open Source tools for FPGA Compilation
## Usage: make <command>
## Commands:

filename = top
pcf_file = ../common/io_mdp_u3.pcf

help:
	@fgrep -h "##" $(MAKEFILE_LIST) | fgrep -v fgrep | sed -e 's/\\$$//' | sed -e 's/##//'

build: ## Synthesis to Bitstream generation

build: synth pnr asm

synth: ## Synthesis using Yosys
	yosys -p "synth_ice40 -top top -json $(filename).json" $(filename).v

pnr: ## Place & Route using nextpnr-ice40 tools
	nextpnr-ice40 --up5k --package uwg30 --json $(filename).json --pcf $(pcf_file) --asc $(filename).asc
	
asm: ## Bitstream Generation 
	icepack $(filename).asc $(filename).bin

prog: #for sram
	iceprog -S $(filename).bin

prog_flash:
	iceprog $(filename).bin

clean:
	rm -rf $(filename).blif $(filename).json $(filename).asc $(filename).bin
