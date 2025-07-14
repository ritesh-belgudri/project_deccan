// C++ wrapper for Verilator testbench
#include <iostream>
#include <memory>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "Vled_blink.h"

int main(int argc, char** argv) {
    // Initialize Verilator
    Verilated::commandArgs(argc, argv);
    
    // Create instance of our module
    std::unique_ptr<Vled_blink> tb(new Vled_blink);
    
    // Initialize trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    tb->trace(tfp, 99);
    tfp->open("led_blink.vcd");
    
    // Initialize signals
    tb->clk = 0;
    tb->rst_n = 0;
    
    std::cout << "Starting LED blink simulation..." << std::endl;
    
    // Run simulation
    int time = 0;
    int clock_cycles = 0;
    int max_cycles = 10000;
    
    // Apply reset for a few cycles
    for (int i = 0; i < 20; i++) {
        tb->clk = 0;
        tb->eval();
        tfp->dump(time++);
        
        tb->clk = 1;
        tb->eval();
        tfp->dump(time++);
        clock_cycles++;
    }
    
    // Release reset
    tb->rst_n = 1;
    std::cout << "Reset released at cycle " << clock_cycles << std::endl;
    
    // Run main simulation
    while (clock_cycles < max_cycles && !Verilated::gotFinish()) {
        tb->clk = 0;
        tb->eval();
        tfp->dump(time++);
        
        tb->clk = 1;
        tb->eval();
        tfp->dump(time++);
        clock_cycles++;
        
        // Print progress periodically
        if (clock_cycles % 1000 == 0) {
            std::cout << "Cycle " << clock_cycles << ": led=" << (int)tb->led << std::endl;
        }
    }
    
    // Test reset functionality
    std::cout << "Testing reset functionality..." << std::endl;
    tb->rst_n = 0;
    for (int i = 0; i < 20; i++) {
        tb->clk = 0;
        tb->eval();
        tfp->dump(time++);
        
        tb->clk = 1;
        tb->eval();
        tfp->dump(time++);
        clock_cycles++;
    }
    
    // Verify reset worked
    if (tb->led == 0) {
        std::cout << "Reset test PASSED: led=" << (int)tb->led << std::endl;
    } else {
        std::cout << "Reset test FAILED: led=" << (int)tb->led << std::endl;
    }
    
    // Release reset and run a bit more
    tb->rst_n = 1;
    for (int i = 0; i < 1000; i++) {
        tb->clk = 0;
        tb->eval();
        tfp->dump(time++);
        
        tb->clk = 1;
        tb->eval();
        tfp->dump(time++);
        clock_cycles++;
    }
    
    // Close trace
    tfp->close();
    delete tfp;
    
    // Final cleanup
    tb->final();
    
    std::cout << "Simulation completed successfully" << std::endl;
    std::cout << "Total cycles: " << clock_cycles << std::endl;
    std::cout << "Final LED state: " << (int)tb->led << std::endl;
    
    return 0;
}
