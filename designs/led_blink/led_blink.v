module led_blink (
    input wire clk,      // clock input
    input wire rst_n,    // Active-low reset
    output reg led       // LED output
);

    localparam integer COUNT_MAX = 25_000_000 - 1;
    reg [25:0] counter;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            counter <= 0;
            led <= 0;
        end else begin
            if (counter == COUNT_MAX) begin
                counter <= 0;
                led <= ~led;
            end else begin
                counter <= counter + 1;
            end
        end
    end
endmodule
