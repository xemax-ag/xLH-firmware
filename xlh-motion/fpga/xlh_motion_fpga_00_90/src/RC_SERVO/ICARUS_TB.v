`include "RC_SERVO.v"
// http://verilogtutorial.blogspot.ch/2012/09/verilog-timescale-system-tasks.html
`timescale 10ns/1ns
//
module ICARUS_TB;
	//
	integer file;
	reg clk;
	reg clk_ctr;
	reg [15:0] value;
	wire out;
	//
	initial 
	begin
		clk <= 1;
		clk_ctr <= 1;
		value <= 11'd5;
		//
		$dumpfile("vcd\\\icarus_tb.vcd"); 
		$dumpvars(0, ICARUS_TB); 
		//
		#32768 $finish; 
	end 
	//
	always #1 clk <= !clk;
	always #10 clk_ctr <= !clk_ctr;
	//
	RC_SERVO rc_servo(clk,
	                  clk_ctr,
			          value,
			          out);
					  
	defparam rc_servo.CTR_LIMIT = 10;
endmodule

