`include "VAR_12_TO_16_BIT.v"
`timescale 10ns/1ns
//
module ICARUS_TB;
	//
	integer file;
	reg clk;
	reg [11:0] in;
	wire [15:0] out;
	//
	initial 
	begin
		clk <= 1;
		in <= 0;
		//
		$dumpfile("vcd\\\icarus_tb.vcd"); 
		$dumpvars(0, ICARUS_TB); 
		//
		#10   in <= 1000;
		#20   in <= 4000;
		//
		#30 $finish; 
	end 
	//
	always #1 clk <= !clk;// 50MHz
	//
	VAR_12_TO_16_BIT var_12_to_16_bit(clk, in, out);
	//
endmodule
