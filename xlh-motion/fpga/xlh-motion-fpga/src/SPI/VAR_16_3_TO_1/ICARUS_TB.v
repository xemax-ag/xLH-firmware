`include "VAR_16_3_TO_1.v"
`timescale 10ns/1ns
//
module ICARUS_TB;
	//
	integer file;
	reg clk;
	reg signed [15:0] in1;
	reg signed [15:0] in2;
	reg signed [15:0] in3;
	wire [15:0] out;
	//
	initial 
	begin
		clk <= 1;
		in1 <= 100;
		in2 <= -20000;
		in3 <= 30000;
		
		//
		$dumpfile("vcd\\\icarus_tb.vcd"); 
		$dumpvars(0, ICARUS_TB); 
		//
		#8 $finish; 
	end 
	//
	always #1 clk <= !clk;// 50MHz
	//
	VAR_16_3_TO_1 var_16_3_to_1(clk, 
	                            in1,
								in2,
								in3,
								out);
	//
endmodule
