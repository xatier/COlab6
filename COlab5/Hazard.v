//////////////////////////////////////////////////////////////////////////////////
// Company:
// Engineer:
//
// Create Date:
// Design Name:
// Module Name:    Hazard
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
module Hazard
(
    memread,
    instr_i,
    idex_regt,
    branch,
    j,
    pcwrite,
    ifid_write,
    ifid_flush,
    idex_flush,
    exmem_flush
);

input       memread;
input [15:0]instr_i;
input [4:0] idex_regt;
input       branch;
input       j;
output      pcwrite;
output      ifid_write;
output      ifid_flush;
output      idex_flush;
output      exmem_flush;

reg [4:0] control_o;

assign {pcwrite, ifid_write, ifid_flush, idex_flush, exmem_flush} = control_o;

always@(*) begin
    case(branch)
        1'b1: control_o <= 5'b11111;
        1'b0: begin
            case(j)
                1'b1: control_o <= 5'b11110;
                1'b0: begin
                    if(memread == 1'b1 && (instr_i[9:5] == idex_regt || (instr_i[4:0] == idex_regt && instr_i[15:10] != 6'b001000)))
                        control_o <= 5'b00010;
                    else
                        control_o <= 5'b11000;
                end
            endcase
        end
    endcase
end

endmodule
