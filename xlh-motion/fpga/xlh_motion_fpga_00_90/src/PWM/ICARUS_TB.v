`include "PWM.v"
`timescale 10ns/1ns
//
module ICARUS_TB;
	integer file;
	reg clk;
	reg [15:0] cmp;
	//
	wire out;
	//
	initial 
		begin
		clk <= 1;
		cmp <= 16'h100;
		//		
		$dumpfile("vcd\\\icarus_tb.vcd"); 
		$dumpvars(0, ICARUS_TB); 		
		//
		//
		#80000 $finish; 
		end 
	//
	always #1 clk <= !clk;// 50MHz
	//
	PWM pwm(clk, cmp, out);
	//
endmodule
/*
cd "$(FULL_CURRENT_PATH)" 
build.bat
*/