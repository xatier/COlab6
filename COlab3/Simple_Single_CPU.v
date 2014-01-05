//Subject:     CO project 2 - Simple Single CPU
//--------------------------------------------------------------------------------
//Version:     1
//--------------------------------------------------------------------------------
//Writer:
//----------------------------------------------
//Date:
//----------------------------------------------
//Description:
//--------------------------------------------------------------------------------
module Simple_Single_CPU(
        clk_i,
        rst_n
        );

//I/O port
input         clk_i;
input         rst_n;

//Internal Signles

wire [31:0] pc, pci, pcx, pco, pcn, pcb;
wire [31:0] instr;
wire [4:0]  regtow;
wire        regw, alusrc, branch, sign, alush, j, memr, memw;
wire [1:0]  mtor, btype, regd;
wire [2:0]  aluop;
wire [31:0] rdata1, rdata2, mdata;
wire [31:0] signex, SAex;
wire [31:0] sl2data;

wire [3:0]  aluctrl;
wire [2:0]  bonusctrl;
wire [31:0] src1, src2, aluresult, wdata;
wire        zero, cout, overflow, rzmux;
//Greate componentes

MUX_2to1 #(.size(32)) Mux_Return(
        .data0_i(pci),
        .data1_i(rdata1),
        .select_i(bonusctrl[1]),
        .data_o(pc)
        );

ProgramCounter PC(
        .clk_i(clk_i),
        .rst_n(rst_n),
        .pc_in_i(pc),
        .pc_out_o(pco)
        );

Adder Adder1(
        .src1_i(32'd4),
        .src2_i(pco),
        .sum_o(pcn)
        );

Instr_Memory IM(
        .pc_addr_i(pco),
        .instr_o(instr)
        );

MUX_4to1 #(.size(5)) Mux_Write_Reg(
        .data0_i(instr[20:16]),
        .data1_i(instr[15:11]),
        .data2_i(5'd31),
        .data3_i(5'd31),
        .select_i(regd),
        .data_o(regtow)
        );

Reg_File RF(
        .clk_i(clk_i),
        .rst_n(rst_n),
        .RSaddr_i(instr[25:21]),
        .RTaddr_i(instr[20:16]),
        .RDaddr_i(regtow),
        .RDdata_i(wdata),
        .RegWrite_i(regw),
        .RSdata_o(rdata1),
        .RTdata_o(rdata2)
        );

Decoder Decoder(
        .instr_op_i(instr[31:26]),
        .RegWrite_o(regw),
        .ALU_op_o(aluop),
        .ALUSrc_o(alusrc),
        .RegDst_o(regd),
        .Branch_o(branch),
        .sign_o(sign),
        .BranchType_o(btype),
        .Jump_o(j),
        .MemRead_o(memr),
        .MemWrite_o(memw),
        .MemtoReg_o(mtor)
        );

ALU_Ctrl AC(
        .funct_i(instr[5:0]),
        .ALUOp_i(aluop),
        .ALUCtrl_o(aluctrl),
        .BonusCtrl_o(bonusctrl),
        .ALUShift_o(alush)
        );

ShiftAmount_Extend SA(
        .data_i(instr[10:6]),
        .data_o(SAex)
        );

Sign_Extend SE(
        .data_i(instr[15:0]),
        .sign_i(sign),
        .data_o(signex)
        );

MUX_2to1 #(.size(32)) Mux_Src1(
        .data0_i(rdata1),
        .data1_i(SAex),
        .select_i(alush),
        .data_o(src1)
        );

MUX_2to1 #(.size(32)) Mux_ALUSrc(
        .data0_i(rdata2),
        .data1_i(signex),
        .select_i(alusrc),
        .data_o(src2)
        );

alu ALU(
        .rst_n(rst_n),
        .src1(src1),
        .src2(src2),
        .ALU_control(aluctrl),
        .bonus_control(bonusctrl),
        .result(aluresult),
        .zero(zero),
        .cout(cout),
        .overflow(overflow)
        );

MUX_4to1 #(.size(32)) Mux_WReg_Src(
        .data0_i(aluresult),
        .data1_i(mdata),
        .data2_i(signex),
        .data3_i(pcn),
        .select_i(mtor),
        .data_o(wdata)
        );

Data_Memory DM(
    .clk_i(clk_i),
    .addr_i(aluresult),
    .data_i(rdata2),
    .MemRead_i(memr),
    .MemWrite_i(memw),
    .data_o(mdata)
    );

MUX_4to1 #(.size(1)) Result_Zero_Mux(
        .data0_i(zero),
        .data1_i(~(zero | aluresult[31])),
        .data2_i(~aluresult[31]),
        .data3_i(~zero),
        .select_i(btype),
        .data_o(rzmux)
        );

Adder Adder2(
        .src1_i(pcn),
        .src2_i(sl2data),
        .sum_o(pcb)
        );

Shift_Left_Two_32 Shifter(
        .data_i(signex),
        .data_o(sl2data)
        );
        
MUX_2to1 #(.size(32)) Mux_PC_Source(
        .data0_i(pcn),
        .data1_i(pcb),
        .select_i((branch & rzmux)),
        .data_o(pcx)
        );

MUX_2to1 #(.size(32)) Mux_Jump(
        .data0_i(pcx),
        .data1_i({pcn[31:28], instr[25:0], 2'b00}),
        .select_i(j),
        .data_o(pci)
        );

endmodule


