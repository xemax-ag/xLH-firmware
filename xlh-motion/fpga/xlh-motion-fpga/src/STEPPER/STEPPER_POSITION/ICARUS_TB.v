`include "STEPPER_POSITION.v"
`timescale 10ns/1ns
//
module ICARUS_TB;
	//
	integer file;
	reg clk;
	reg dir;
	reg puls;
	reg reset;
	wire [31:0] position;
	//
	initial 
	begin
		clk <= 1;
		dir <= 0;
		reset <= 0;
		puls <= 1;
		//
		$dumpfile("vcd\\\icarus_tb.vcd"); 
		$dumpvars(0, ICARUS_TB); 
		//
		#10 puls <= 1; #10 puls <= 0;
		#10 puls <= 1; #10 puls <= 0;
		#10 puls <= 1; #10 puls <= 0;
		#10 puls <= 1; #10 puls <= 0;
		#10 puls <= 1; #10 puls <= 0;
		
		#10 dir <= 1;
		#10 puls <= 1; #10 puls <= 0;
		#10 puls <= 1; #10 puls <= 0;
		#10 puls <= 1; #10 puls <= 0;
		#10 puls <= 1; #10 puls <= 0;
		#10 puls <= 1; #10 puls <= 0;
		#10 puls <= 1; #10 puls <= 0;
		#10 puls <= 1; #10 puls <= 0;
		#10 puls <= 1; #10 puls <= 0;
		#10 puls <= 1; #10 puls <= 0;
		
		#10 reset <= 1;
		//
		#32768 $finish; 
	end 
	//
	always #1 clk <= !clk;
	//
	STEPPER_POSITION stepper_position(clk,
	                                  dir,
			                          puls,
			                          reset,
					                  position);
	//
endmodule

