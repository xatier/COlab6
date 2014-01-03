`define CYCLE_TIME 20

module TestBench;
reg CLK, START;
integer handle1, handle2;

Pipe_CPU_1 cpu(
    .clk_i(CLK),
    .rst_n(START)
);

initial
begin
    $readmemb("lab6_test_data.txt", cpu.IM.instruction_file);
    CLK = 0;
    START = 0;
    handle1 = $fopen("ICACHE.txt");
    handle2 = $fopen("DCACHE.txt");

    #(`CYCLE_TIME)

    START = 1;
    #(`CYCLE_TIME*1000) begin
        $fclose(handle1);
        $fclose(handle2);
        $stop;
    end
end

always #(`CYCLE_TIME/2) CLK = ~CLK;

always@(posedge CLK) begin

    if(cpu.IM.instr_o != 32'd0)
        $fdisplay(handle1, "%h", cpu.IM.addr_i);
    else;

    if(cpu.DM.MemWrite_i || cpu.DM.MemRead_i)
        $fdisplay(handle2, "%h", cpu.DM.addr_i);
    else;

end


endmodule
