Fully Open Source Verilog-to-Bitstream flow for FPGAs contains Yosys, IceStorm (IcePack/IceUnpack, IceBox, IceProg, IceTime, IcePLL , IceMulti and IceBRAM tools), and nextpnr (Place & Route Tools) Make files Automates the flow from Synthesis, Place & Route (PnR), bitstream generation and device programming

Example Design: Open source example design to run on the target patforms

Target Devices:
1. iCE40-UP5K
more devices support coming soon...

Target Platfroms: 
1. ICE40 MDP (Mobile Development Platform)
more platfroms coming soon...

Tools used:
* yosys [version: 0.33 (git sha1 2584903a060)]
* icestorm 
* nextpnr-ice40 [Version 0.6-3build5]

Installting tools:

OS: Ubuntu 24.04

Pre-requisite:
```    
sudo apt-get install build-essential clang bison flex libreadline-dev gawk tcl-dev libffi-dev git mercurial graphviz xdot pkg-config python3 libftdi-dev python3-dev libboost-all-dev cmake libeigen3-dev
```

1. Icestorm
    ```
    git clone https://github.com/rbelgudr/project_deccan.git
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


References: <br> 
[1] https://prjicestorm.readthedocs.io/en/latest/overview.html <br>
[2] https://yosyshq.readthedocs.io/projects/yosys/en/latest <br>
[3] https://github.com/YosysHQ/nextpnr <br>

About Project Deccan: This project is collection of open source tools and designs for various FPGAs devices and boards. This project aims to document the design flows for easily to use and learn fundamental of FPGA designs. 

About Deccan Plateau: The project name is inspired by largest plateau in Indian peninsula, refer to https://en.wikipedia.org/wiki/Deccan_Plateau
