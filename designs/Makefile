# ====== Configurations ======
PROJ_NAME   ?= led_blink
proj_name   = $(PROJ_NAME)
src_dir     = $(proj_name)
pcf_file    = boards/lattice/ice40-mdp/io_mdp_u3.pcf
build_dir   = build
sim_dir     = sim
MODE       ?= release
output_bin = $(build_dir)/$(proj_name).bin
timing_rpt = $(build_dir)/$(proj_name)_timing.rpt
util_rpt   = $(build_dir)/$(proj_name)_utilization.rpt
sim_exe    = $(sim_dir)/$(proj_name)_sim
sim_vcd    = $(sim_dir)/$(proj_name).vcd

# ====== Colors ======
GREEN  = \033[0;32m
YELLOW = \033[1;33m
RED    = \033[0;31m
NC     = \033[0m

# ====== Mode Flags ======
ifeq ($(MODE),debug)
    SYNTH_ARGS = -dsp -noabc
else
    SYNTH_ARGS =
endif

# ====== Build Rules ======
.PHONY: build clean prog prog_flash info all help reports timing_report util_report sim sim_wave sim_clean

all: info build prog

help:
	@echo "$(YELLOW)Available targets:$(NC)"
	@echo "  $(GREEN)help$(NC)       - Show this help message"
	@echo "  $(GREEN)info$(NC)       - Display build configuration"
	@echo "  $(GREEN)build$(NC)      - Build the project (synthesis, place & route, bitstream)"
	@echo "  $(GREEN)sim$(NC)        - Run Verilator simulation"
	@echo "  $(GREEN)sim_wave$(NC)   - Run simulation and open waveform viewer"
	@echo "  $(GREEN)reports$(NC)    - Generate timing and utilization reports"
	@echo "  $(GREEN)timing_report$(NC) - Generate timing analysis report"
	@echo "  $(GREEN)util_report$(NC) - Generate resource utilization report"
	@echo "  $(GREEN)prog$(NC)       - Program bitstream to SRAM (volatile)"
	@echo "  $(GREEN)prog_flash$(NC) - Flash bitstream to device (persistent)"
	@echo "  $(GREEN)clean$(NC)      - Remove build directory"
	@echo "  $(GREEN)sim_clean$(NC)  - Remove simulation directory"
	@echo "  $(GREEN)all$(NC)        - Run info, build, and prog targets"
	@echo ""
	@echo "$(YELLOW)Variables:$(NC)"
	@echo "  $(GREEN)PROJ_NAME$(NC)  - Project name (default: led_blink)"
	@echo "  $(GREEN)MODE$(NC)       - Build mode: release or debug (default: release)"
	@echo ""
	@echo "$(YELLOW)Examples:$(NC)"
	@echo "  make build PROJ_NAME=my_project"
	@echo "  make all MODE=debug"
	@echo "  make build PROJ_NAME=uart_test MODE=debug"
	@echo "  make reports PROJ_NAME=my_project"
	@echo "  make sim PROJ_NAME=led_blink"
	@echo "  make sim_wave PROJ_NAME=led_blink"

info:
	@echo "$(YELLOW)[INFO]$(NC) Project name: $(proj_name)"
	@echo "$(YELLOW)[INFO]$(NC) Build mode: $(MODE)"
	@echo "$(YELLOW)[INFO]$(NC) Output binary: $(output_bin)"
	@echo "$(YELLOW)[INFO]$(NC) Timing report: $(timing_rpt)"
	@echo "$(YELLOW)[INFO]$(NC) Utilization report: $(util_rpt)"
	@echo "$(YELLOW)[INFO]$(NC) Simulation executable: $(sim_exe)"
	@echo "$(YELLOW)[INFO]$(NC) Simulation VCD: $(sim_vcd)"

build: $(output_bin) 

$(build_dir)/$(proj_name).json: $(src_dir)/top.v
	@mkdir -p $(build_dir)
	@echo "$(GREEN)[BUILD]$(NC) Synthesizing..."
	@yosys -p "synth_ice40 -top top $(SYNTH_ARGS) -json $@" $<
	@echo "$(GREEN)[REPORT]$(NC) Generating utilization report..."
	@yosys -p "read_verilog $<; synth_ice40 -top top $(SYNTH_ARGS); stat -top top" > $(util_rpt) 2>&1

$(build_dir)/$(proj_name).asc: $(build_dir)/$(proj_name).json
	@echo "$(GREEN)[BUILD]$(NC) Running place-and-route..."
	@nextpnr-ice40 --up5k --package uwg30 --json $< --pcf $(pcf_file) --asc $@
	@echo "$(GREEN)[REPORT]$(NC) Generating timing report..."
	@icetime -d up5k -P uwg30 -p $(pcf_file) -t -r $(timing_rpt) $@

$(output_bin): $(build_dir)/$(proj_name).asc
	@echo "$(GREEN)[BUILD]$(NC) Packing bitstream..."
	@icepack $< $@

prog: $(output_bin)
	@echo "$(GREEN)[FLASH]$(NC) Programming to SRAM..."
	@iceprog -S $<

prog_flash: $(output_bin)
	@echo "$(GREEN)[FLASH]$(NC) Flashing to device..."
	@iceprog $<

clean:
	@echo "$(RED)[CLEAN]$(NC) Removing build directory..."
	@rm -rf $(build_dir)

sim_clean:
	@echo "$(RED)[CLEAN]$(NC) Removing simulation directory..."
	@rm -rf $(sim_dir)

# ====== Simulation Targets ======
sim: $(sim_exe)
	@echo "$(GREEN)[SIM]$(NC) Running Verilator simulation..."
	@cd $(sim_dir) && ./$(notdir $(sim_exe))

sim_wave: $(sim_vcd)
	@echo "$(GREEN)[SIM]$(NC) Opening waveform viewer..."
	@if command -v gtkwave >/dev/null 2>&1; then \
		gtkwave $(sim_vcd) & \
	else \
		echo "$(YELLOW)[WARNING]$(NC) GTKWave not found. Please install it to view waveforms."; \
		echo "$(YELLOW)[INFO]$(NC) VCD file available at: $(sim_vcd)"; \
	fi

$(sim_exe): $(src_dir)/$(proj_name).v $(src_dir)/$(proj_name)_sim.cpp
	@mkdir -p $(sim_dir)
	@echo "$(GREEN)[SIM]$(NC) Compiling with Verilator..."
	@verilator --cc --exe --build -Wno-UNUSED -Wno-UNDRIVEN -Wno-TIMESCALEMOD -Wno-STMTDLY \
		--no-timing --trace \
		--top-module $(proj_name) \
		-o $(notdir $(sim_exe)) \
		$(abspath $(src_dir)/$(proj_name).v) $(abspath $(src_dir)/$(proj_name)_sim.cpp) \
		--Mdir $(sim_dir)/obj_dir
	@cp $(sim_dir)/obj_dir/$(notdir $(sim_exe)) $(sim_exe)

$(sim_vcd): $(sim_exe)
	@echo "$(GREEN)[SIM]$(NC) Running simulation to generate VCD..."
	@cd $(sim_dir) && ./$(notdir $(sim_exe))

# ====== Report Targets ======
reports: timing_report util_report
	@echo "$(YELLOW)[REPORTS]$(NC) All reports generated successfully!"

timing_report: $(timing_rpt)
	@echo "$(YELLOW)[REPORT]$(NC) Timing report available at: $(timing_rpt)"
	@echo "$(YELLOW)[REPORT]$(NC) Summary:"
	@echo "  Critical Path Analysis:"
	@grep "Total path delay:" $(timing_rpt) 2>/dev/null || echo "  Timing analysis data available in report file"
	@echo "  Logic Levels:"
	@grep "Total number of logic levels:" $(timing_rpt) 2>/dev/null || echo "  Logic level data available in report file"

util_report: $(util_rpt)
	@echo "$(YELLOW)[REPORT]$(NC) Utilization report available at: $(util_rpt)"
	@echo "$(YELLOW)[REPORT]$(NC) Summary:"
	@grep -A 20 "Number of cells:" $(util_rpt) | grep -E "(SB_|ICESTORM_)" | head -10 2>/dev/null || echo "  Resource utilization data available in report file"

$(timing_rpt): $(build_dir)/$(proj_name).asc
	@echo "$(GREEN)[REPORT]$(NC) Generating standalone timing report..."
	@icetime -d up5k -P uwg30 -p $(pcf_file) -t -r $@ $<

$(util_rpt): $(build_dir)/$(proj_name).json
	@echo "$(GREEN)[REPORT]$(NC) Generating standalone utilization report..."
	@yosys -p "read_verilog $(src_dir)/top.v; synth_ice40 -top top $(SYNTH_ARGS); stat -top top" > $@ 2>&1
