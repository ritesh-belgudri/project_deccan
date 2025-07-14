`timescale 1ns / 1ps

module led_blink (
    input wire clk,      // clock input
    input wire rst_n,    // Active-low reset
    output reg led       // LED output
);

    localparam [25:0] COUNT_MAX = 26'd25_000_000 - 1;
    reg [25:0] counter;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            counter <= 26'd0;
            led <= 1'b0;
        end else begin
            if (counter == COUNT_MAX) begin
                counter <= 26'd0;
                led <= ~led;
            end else begin
                counter <= counter + 1;
            end
        end
    end
endmodule
