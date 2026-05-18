`include "VAR_8_3_TO_1.v"
`timescale 10ns/1ns
//
module ICARUS_TB;
	//
	integer file;
	reg clk;
	reg [7:0] in1;
	reg [7:0] in2;
	reg [7:0] in3;
	wire [7:0] out;
	//
	initial 
	begin
		clk <= 1;
		in1 <= 8'b0000_0000;
		in2 <= 8'b0000_0000;
		in3 <= 8'b0000_0000;
		//
		$dumpfile("vcd\\\icarus_tb.vcd"); 
		$dumpvars(0, ICARUS_TB); 
		//
		#8 $finish; 
	end 
	//
	always #1 clk <= !clk;// 50MHz
	//
	VAR_8_3_TO_1 var_8_3_to_1(in1,
	                          in2,
							  in3,
							  out);
	//
endmodule
