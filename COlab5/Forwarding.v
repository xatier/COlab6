//////////////////////////////////////////////////////////////////////////////////
// Company:
// Engineer:
//
// Create Date:
// Design Name:
// Module Name:    Forwarding
// Project Name:
// Target Devices:
// Tool versions:
// Description:
//
// Dependencies:
//
// Revision:
// Revision
// Additional Comments:
//
//////////////////////////////////////////////////////////////////////////////////
module Forwarding(
            exmem_regw,
            memwb_regw,
            idex_regs,
            idex_regt,
            exmem_regd,
            memwb_regd,
            forwarda,
            forwardb
            );

input       exmem_regw;
input       memwb_regw;
input [4:0] idex_regs;
input [4:0] idex_regt;
input [4:0] exmem_regd;
input [4:0] memwb_regd;
output [1:0] forwarda;
output [1:0] forwardb;

reg [1:0] forwarda;
reg [1:0] forwardb;

always@(*) begin
    if(exmem_regw == 1'b1 && idex_regs == exmem_regd && exmem_regd != 5'd0)
        forwarda <= 2'b01;
    else if(memwb_regw == 1'b1 && idex_regs == memwb_regd && memwb_regd != 5'd0)
        forwarda <= 2'b10;
    else
        forwarda <= 2'b00;
end

always@(*) begin
    if(exmem_regw == 1'b1 && idex_regt == exmem_regd && exmem_regd != 5'd0)
        forwardb <= 2'b01;
    else if(memwb_regw == 1'b1 && idex_regt == memwb_regd && memwb_regd != 5'd0)
        forwardb <= 2'b10;
    else
        forwardb <= 2'b00;
end

endmodule
