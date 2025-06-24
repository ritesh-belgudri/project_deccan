Fully Open Source Verilog-to-Bitstream flow for iCE40 FPGAs Contains Yosys, IceStorm (IcePack/IceUnpack, IceBox, IceProg, IceTime, IcePLL , IceMulti and IceBRAM tools), and nextpnr (Place & Route Tools) Make files Automates the flow from Synthesis, Place & Route (PnR), bitstream generation and device programming

Target Board: ICE40 MDP (Mobile Development Platform)

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
    git clone https://gitea.iitdh.ac.in/EE24DP013/raigad_icestorm.git
    cd raigad_icestorm/tools/icestorm
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


References: 
[1] https://prjicestorm.readthedocs.io/en/latest/overview.html
[2] https://yosyshq.readthedocs.io/projects/yosys/en/latest/
[3] https://github.com/YosysHQ/nextpnr


About Raigad Fort: https://en.wikipedia.org/wiki/Raigad_Fort
