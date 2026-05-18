`include "WDT.v"
// http://verilogtutorial.blogspot.ch/2012/09/verilog-timescale-system-tasks.html
`timescale 10ns/1ns
//
module ICARUS_TB;
	integer file;
	reg clk;
	reg clk_1kHz;
	reg in; 
	reg enable_spi;
	wire out;
	reg wdt_en;
	//
	initial 
		begin
			clk <= 1;
			clk_1kHz <= 1;
			in <= 1;
			enable_spi <= 1;
			wdt_en <= 1;
			//
			$dumpfile("vcd\\\icarus_tb.vcd"); 
			$dumpvars(0, ICARUS_TB); 	
			//
			#2000 wdt_en <= 0;			
			#2000 $finish; 
		end 
	//
	always #1 clk <= !clk;
	always #10 clk_1kHz <= !clk_1kHz;/* real #25000 */
	always #50 in <= !in;
	//
	WDT wdt(clk, clk_1kHz, (in && wdt_en), enable_spi, out);
	defparam wdt.DELAY_TIME = 16'd50;
	defparam wdt.RESET_TIME = 16'd5;
endmodule

/*
cd D:\Dropbox\ABBTS_LABOR_DOZ\MOTION_CONTROL\FPGA_MODUL_MAX1000_DIT_RET_UR\FPGA_MODUL_MAX1000_DIT_RET_UR_00_03\src\CLOCK\WDT
C:\iverilog\bin\iverilog -s ICARUS_TB -o icarus_tb.o ICARUS_TB.v
C:\iverilog\bin\vvp icarus_tb.o
*/