Project Deccan: Open-Source Verilog-to-Bitstream Flow

Project Deccan ```
project_deccan
├── designs                 // Contains example designs, Makefile
│   ├── boards              // Contains board specific files (example: user guide, schematics, pinouts, etc...) 
│   │   └── lattice         // FPGA vendor name
│   │       └── ice40-mdp   // Target board name
│   ├── build               // Build artifacts (JSON, ASC, BIN files)
│   ├── sim                 // Simulation artifacts (VCD, executable files)
│   └── led_blink           // Example design directory: contains verilog sources, testbench 
└── tools                   // Contains open source tools and scripts that are reqiured for running the flow
    └── icestorm            // open source tools for ICE40 FPGA (Project IceStorm) 
```ogether Yosys, IceStorm, and nextpnr into a seamless pipeline for iCE40 FPGAs. It automates synthesis, place-&-route, bitstream generation, and device programming.

Features:
- End-to-end flow: Verilog → JSON → ASC → BIT
- Automatic build/ directory creation
- Multi-design support via a single Makefile
- Automated timing and resource utilization reporting integrated into build process
- Verilator-based simulation flow with waveform generation
- Customizable PCF injection, debug/release modes
- Colorized terminal output for better user experience
- Clean file naming without timestamps for consistent builds

Target Devices:
1. iCE40-UP5K <br>
more devices support coming soon...

Target Platforms: 
1. ICE40 MDP (Mobile Development Platform) <br>
more platforms coming soon...

Tools used:
* yosys [version: 0.33 (git sha1 2584903a060)]
* icestorm 
* nextpnr-ice40 [Version 0.6-3build5]
* verilator [for simulation]

Installing tools:

OS: Ubuntu 24.04 (tested on Window Subsystem for Linux - WSL)

Pre-requisite:
```    
sudo apt-get install build-essential clang bison flex libreadline-dev gawk tcl-dev libffi-dev git mercurial graphviz xdot pkg-config python3 libftdi-dev python3-dev libboost-all-dev cmake libeigen3-dev
```

1. Icestorm
    ```
    git clone --recurse-submodules https://github.com/rbelgudr/project_deccan.git
    cd project_deccan/tools/icestorm
    make -j$(nproc)
    sudo make install
    ```

2. Yosys
    ```
    sudo apt-get install yosys
    ```

3. nextpnr-ice40
    ```
    sudo apt-get install nextpnr-ice40
    ```

4. Verilator (for simulation)
    ```
    sudo apt-get install verilator
    ```

5. GTKWave (for waveform viewing - optional)
    ```
    sudo apt-get install gtkwave
    ```

Work in progress: Dockerfile to automate the tool and dependency installation

 
## Directory Structure:
The project directories are organized as below

```
project_deccan
├── designs                 // Contains example designs, Makefile
│   ├── boards              // Contains board specific files (example: user guide, schematics, pinouts, etc...) 
│   │   └── lattice         // FPGA vendor name
│   │       └── ice40-mdp   // Target board name
│   └── led_blink           // Example design directory: contains verilog sources, testbench 
└── tools                   // Contains open source tools and scripts that are reqiured for running the flow
    └── icestorm            // open source tools for ICE40 FPGA (Project IceStorm) 
```

Makefile Pipeline:

design.v
   │
   ▼
yosys → design.json + utilization.rpt
   │
   ▼
nextpnr-ice40 → design.asc
   │
   ▼
icetime → timing.rpt
   │
   ▼
icepack → design.bin


Usage:
- Clone the repo and install dependencies.
- Place your .v (and optionally .pcf) files under designs/.
- Navigate to the designs directory: `cd designs`
- Run one of the available make targets:

## Build Process:
The Makefile implements a comprehensive build flow with integrated reporting and simulation:

1. **Synthesis** (yosys): Converts Verilog to JSON netlist + generates utilization report
2. **Place & Route** (nextpnr-ice40): Maps design to FPGA resources, generates ASCII file
3. **Timing Analysis** (icetime): Analyzes timing performance, generates timing report
4. **Bitstream Generation** (icepack): Creates final bitstream file
5. **Simulation** (verilator): Compiles and runs functional simulation with waveform generation

All reports are automatically generated during the build process, ensuring you always have current analysis data.

## Available Make Targets:
- `make help` - Show available targets and usage examples
- `make info` - Display build configuration
- `make build` - Build the project (synthesis, place & route, bitstream)
- `make sim` - Run Verilator simulation
- `make sim_wave` - Run simulation and open waveform viewer
- `make reports` - Generate timing and utilization reports
- `make timing_report` - Generate timing analysis report
- `make util_report` - Generate resource utilization report
- `make prog` - Program bitstream to SRAM (volatile)
- `make prog_flash` - Flash bitstream to device (persistent)
- `make clean` - Remove build directory
- `make sim_clean` - Remove simulation directory
- `make all` - Run info, build, and prog targets

## Build Examples:
```bash
# Build default project (led_blink)
make build

# Build a specific project
make build PROJ_NAME=my_project

# Build in debug mode
make build MODE=debug

# Build specific project in debug mode
make build PROJ_NAME=uart_test MODE=debug

# Complete workflow (info + build + program)
make all

# Generate design reports
make reports

# Generate only timing report
make timing_report

# Generate only utilization report  
make util_report

# Generate reports for specific project
make reports PROJ_NAME=my_project

# Simulation examples
make sim PROJ_NAME=led_blink

# Run simulation with waveform viewer
make sim_wave PROJ_NAME=led_blink

# Clean build directory
make clean

# Clean simulation directory
make sim_clean
```

## Design Analysis & Reporting:
Project Deccan automatically generates comprehensive design reports during the build process to help you analyze timing performance and resource utilization.

### Report Types:
1. **Timing Report** (`*_timing.rpt`):
   - Critical path analysis with detailed gate-level timing breakdown
   - Maximum achievable frequency calculation
   - Logic level count and path delay analysis
   - Detailed net-by-net timing information
   - Generated using `icetime` tool for accurate timing estimates

2. **Utilization Report** (`*_utilization.rpt`):
   - Logic cell usage (LUTs, flip-flops)
   - Memory block utilization (BRAM, SPRAM)
   - DSP block usage
   - I/O pin utilization
   - Carry chain analysis

### Automatic Report Generation:
Reports are automatically generated during `make build` and stored in the `build/` directory:
- `build/{project_name}_timing.rpt` - Detailed timing analysis (generated using icetime)
- `build/{project_name}_utilization.rpt` - Resource usage statistics (generated using yosys)

The timing report is generated during the build process after place-and-route completion, ensuring you always have up-to-date timing information for your design.

### Report Summary Output:
When generating reports, the Makefile displays key metrics in the terminal:

**Timing Summary Example:**
```
[REPORT] Timing report available at: build/led_blink_timing.rpt
[REPORT] Summary:
  Critical Path Analysis:
Total path delay: 16.51 ns (60.57 MHz)
  Logic Levels:
Total number of logic levels: 27
```

**Utilization Summary Example:**
```
[REPORT] Utilization report available at: build/led_blink_utilization.rpt
[REPORT] Summary:
     SB_CARRY                       24
     SB_DFFER                        1
     SB_DFFR                        26
     SB_LUT4                        63
```

### Report Generation Examples:
```bash
# Generate all reports for current project
make reports

# Generate reports for specific project
make reports PROJ_NAME=uart_controller

# Generate only timing analysis
make timing_report

# Generate only resource utilization
make util_report

# Reports in debug mode (may show different utilization)
make reports MODE=debug
```

### Understanding the Reports:
- **Timing Reports**: 
  - Generated using `icetime` tool for topological timing analysis during the build process
  - Provides detailed gate-level timing breakdown of critical paths
  - Shows logic delays, interconnect delays, and setup times
  - Identifies exact logic cells and nets in the critical path
  - Helps optimize design for better timing performance
  - Automatically generated when building to ensure timing constraints are met
- **Utilization Reports**: 
  - Generated using `yosys` statistics during synthesis
  - Show how efficiently your design uses FPGA resources
  - Include LUT, flip-flop, memory, and I/O utilization
- **Debug vs Release**: Debug mode may use additional resources due to different synthesis options

### Detailed Timing Analysis:
The `icetime` tool provides comprehensive timing analysis including:
- Gate-level timing breakdown (LogicCell40, InMux, LocalMux delays)
- Carry chain timing analysis for arithmetic operations
- Net-by-net timing information with signal names
- Total logic levels in the critical path
- Accurate frequency calculations based on physical delays
- Integrated into the build process for automatic timing verification

### Build Process Integration:
The Makefile automatically integrates timing analysis into the build flow:
1. **Synthesis**: Yosys generates utilization statistics
2. **Place & Route**: nextpnr-ice40 performs placement and routing
3. **Timing Analysis**: icetime analyzes the placed and routed design
4. **Bitstream Generation**: icepack creates the final bitstream

This ensures that every build includes comprehensive timing and utilization analysis.

## Simulation Flow:
Project Deccan includes a complete simulation flow using Verilator for functional verification of your designs.

### Simulation Features:
- **Verilator-based simulation**: Fast, cycle-accurate simulation
- **Automatic testbench compilation**: Seamless integration with Makefile
- **Waveform generation**: VCD files for signal analysis
- **GTKWave integration**: Automatic waveform viewer launch
- **Project-specific simulation**: Support for multiple designs

### Simulation Process:
1. **Testbench Creation**: Write `*_tb.v` testbench files in your design directory
2. **Compilation**: Verilator compiles the design and testbench
3. **Execution**: Simulation runs and generates VCD waveform files
4. **Analysis**: View waveforms in GTKWave or other VCD viewers

### Simulation Examples:
```bash
# Run simulation for led_blink design
make sim PROJ_NAME=led_blink

# Run simulation and open waveform viewer
make sim_wave PROJ_NAME=led_blink

# Clean simulation artifacts
make sim_clean
```

### Simulation Output:
- `sim/{project_name}_sim` - Compiled simulation executable
- `sim/{project_name}.vcd` - VCD waveform file
- `sim/obj_dir/` - Verilator compilation artifacts

### Testbench Requirements:
- Design must include a C++ simulation wrapper: `{project_name}_sim.cpp`
- Wrapper handles clock generation, reset sequencing, and VCD generation
- VCD files can be viewed with GTKWave or other waveform viewers
- Simulation provides functional verification of the design logic

### Verilator Installation:
Verilator is a fast cycle-accurate simulator that converts Verilog to C++ for high-performance simulation.

**Installation:**
```bash
sudo apt-get install verilator
```

**Optional - GTKWave for waveform viewing:**
```bash
sudo apt-get install gtkwave
```

### Complete Development Flow:
1. **Design**: Write your Verilog modules
2. **Simulate**: Verify functionality with `make sim`
3. **Synthesize**: Generate netlist with `make build`
4. **Analyze**: Review timing and utilization reports
5. **Program**: Flash to hardware with `make prog`

The simulation flow provides functional verification before committing to hardware synthesis and programming.

### Example Simulation Output:
```
Starting LED blink simulation...
Reset released at cycle 20
Cycle 1000: led=0
Cycle 2000: led=0
...
Testing reset functionality...
Reset test PASSED: led=0
Simulation completed successfully
Total cycles: 11020
Final LED state: 0
```

- Find .bin files in build/ directory (named as {project_name}.bin)
- Program your device using the prog or prog_flash targets


References: <br> 
[1] https://prjicestorm.readthedocs.io/en/latest/overview.html <br>
[2] https://yosyshq.readthedocs.io/projects/yosys/en/latest <br>
[3] https://github.com/YosysHQ/nextpnr <br>
[4] https://www.veripool.org/verilator/<br>

About Project Deccan: This project is collection of open source tools and designs for various FPGAs devices and boards. This project aims to document the design flows for easily to use and learn fundamental of FPGA designs. 

About Deccan Plateau: The project name is inspired by largest plateau in Indian peninsula, refer to https://en.wikipedia.org/wiki/Deccan_Plateau

