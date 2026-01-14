`include "STEPPER_PULSER.v"
`timescale 10ns/1ns
//
module ICARUS_TB;
	//
	integer file;
	reg clk;
	reg dir;
	reg [31:0] clk_cmp;
	wire dir_out;
	wire puls;
	//
	initial 
	begin
		clk <= 1;
		dir <= 0;
		clk_cmp <= 32'd0;
		//
		$dumpfile("vcd\\\icarus_tb.vcd"); 
		$dumpvars(0, ICARUS_TB); 
		//
		#50 clk_cmp <= 32'd400;
		
		//
		#10000 $finish; 
	end 
	//
	always #1 clk <= !clk;
	//
	STEPPER_PULSER stepper_pulser(clk,
								  dir,
								  clk_cmp,
								  dir_out,
								  out);
	//
	defparam stepper_pulser.CTR_PULS_LEN = 4;
	defparam stepper_pulser.CTR_LIMIT_MIN = 8;
	defparam stepper_pulser.CTR_LIMIT_MAX = 12;
endmodule

