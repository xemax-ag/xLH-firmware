`include "DIG_INP_FILTER.v"
// http://verilogtutorial.blogspot.ch/2012/09/verilog-timescale-system-tasks.html
`timescale 10ns/1ns
//
module ICARUS_TB;
	integer file;
	reg clk;
	reg in;
	wire out;	
	//
	initial 
		begin
			clk <= 1;
			in <= 0;
			//		
			$dumpfile("vcd\\\icarus_tb.vcd"); 
			$dumpvars(0, ICARUS_TB); 		
			//
			#4 in <= 1;
			//
			#30 in <= 0;
			#4 in <= 1;
			#5 in <= 0;
			#5 in <= 1;
			//
			#30 in <= 0;
			//
			#30 in <= 1;
			#4 in <= 0;
			#5 in <= 1;
			#5 in <= 0;
			//
			#100 $finish; 
		end 
	//
	always #1 clk <= !clk;// 50MHz
	//
	DIG_INP_FILTER dig_inp_filter(clk,
						          in,
						          out);					
	defparam dig_inp_filter.FILTER = 5'd5;
	//
endmodule
/*
cd D:\Dropbox\ABBTS_LABOR_DOZ\MOTION_CONTROL\FPGA_MODUL_MAX1000_DIT_RET_UR\FPGA_MODUL_MAX1000_DIT_RET_UR_00_02\src\BASIS\VERILOG\GLUE_LOGIC
C:\iverilog\bin\iverilog -s ICARUS_TB -o icarus_tb.o ICARUS_TB.v
C:\iverilog\bin\vvp icarus_tb.o
*/