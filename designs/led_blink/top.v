// (c) 2025 IIT Dharwad
// Top file for Lattice ICE40 UP5K MDP evaluation Board

`include "led_blink.v"

module top(
	input osc_clk, 
	input PB_2,
	output wire LED_2
	);

led_blink led0(
    	.clk (osc_clk),      // 50 MHz clock input
    	.rst_n (PB_2),    // Active-low reset
    	.led (LED_2)       // LED output
);

endmodule
