`include "CLOCK_GENERATOR.v"
// http://verilogtutorial.blogspot.ch/2012/09/verilog-timescale-system-tasks.html
`timescale 10ns/1ns
//
module ICARUS_TB;
	integer file;
	reg clk;
	wire clock_5MHz;
	wire clock_1MHz;
	wire clock_500kHz;
	wire clock_100kHz;
	wire clock_50kHz;
	wire clock_10kHz;
	wire clock_5kHz;
	wire clock_1kHz;
	wire clock_500Hz;
	wire clock_100Hz;
	wire clock_50Hz;
	wire clock_10Hz;
	wire clock_5Hz;
	wire clock_1Hz;
	//
	initial 
		begin
			clk <= 1;
			//		
			$dumpfile("vcd\\\icarus_tb.vcd"); 
			$dumpvars(0, ICARUS_TB); 		
			//
			#100000 $finish; 
		end 
	//
	always #1 clk <= !clk;// 50MHz
	//
	CLOCK_GENERATOR clock_generator(clk,
									clock_5MHz,
									clock_1MHz,
									clock_500kHz,
									clock_100kHz,
									clock_50kHz,
									clock_10kHz,
									clock_5kHz,
									clock_1kHz,
									clock_500Hz,
									clock_100Hz,
									clock_50Hz,
									clock_10Hz,
									clock_5Hz,
									clock_1Hz
									);
	//
endmodule
/*
cd cd D:\Dropbox\ABBTS_LABOR_DOZ\MOTION_CONTROL\FPGA_MODUL_MAX1000_DIT_RET_UR\FPGA_MODUL_MAX1000_DIT_RET_UR_00_02\src\CLOCK\GENERATOR
C:\iverilog\bin\iverilog -s ICARUS_TB -o icarus_tb.o ICARUS_TB.v
C:\iverilog\bin\vvp icarus_tb.o
*/