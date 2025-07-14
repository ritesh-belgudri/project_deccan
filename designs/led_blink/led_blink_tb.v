// Testbench for led_blink module
`timescale 1ns / 1ps

module led_blink_tb;

    // Testbench signals
    reg clk;
    reg rst_n;
    wire led;
    
    // Instantiate the Unit Under Test (UUT)
    led_blink uut (
        .clk(clk),
        .rst_n(rst_n),
        .led(led)
    );

endmodule
