`include "SPI.v"
// http://verilogtutorial.blogspot.ch/2012/09/verilog-timescale-system-tasks.html
`timescale 10ns/1ns
//
module ICARUS_TB;
	// SPI_HOST
	integer file;
	reg clk;
	reg spi_cs;
	reg spi_clk;
	reg spi_mosi;
	reg [511:0] data_tx;
	reg reset;
	wire spi_host_master;
	wire spi_cs_out;
	wire spi_miso;
	wire [511:0] data_rx;	
	//
	// https://learn.sparkfun.com/tutorials/serial-peripheral-interface-spi
	//
	initial 
		begin
			clk <= 1;		
			// SPI_HOST
			spi_cs <= 1;
			spi_clk <= 0;
			spi_mosi <= 0;
			data_tx <= 512'h0000_1234;	
			reset <= 0;
			//
			$dumpfile("vcd\\\icarus_tb.vcd"); 
			$dumpvars(0, ICARUS_TB); 
			//
			#40 spi_cs <= 0;
			// DATEN BYTE 0
			#20 spi_mosi <= 1;/* 0 */ #40 spi_mosi <= 0;
			#40 spi_mosi <= 0;/* 1 */ #40 spi_mosi <= 0;
			#40 spi_mosi <= 0;/* 2 */ #40 spi_mosi <= 0;
			#40 spi_mosi <= 0;/* 3 */ #40 spi_mosi <= 0;
			#40 spi_mosi <= 0;/* 4 */ #40 spi_mosi <= 0;
			#40 spi_mosi <= 0;/* 5 */ #40 spi_mosi <= 0;
			#40 spi_mosi <= 0;/* 6 */ #40 spi_mosi <= 0;
			#40 spi_mosi <= 1;/* 7 */ #40 spi_mosi <= 0;
			// DATEN BYTE 1
			#20 spi_mosi <= 0;/* 0 */ #40 spi_mosi <= 0;
			#40 spi_mosi <= 0;/* 1 */ #40 spi_mosi <= 0;
			#40 spi_mosi <= 0;/* 2 */ #40 spi_mosi <= 0;
			#40 spi_mosi <= 0;/* 3 */ #40 spi_mosi <= 0;
			#40 spi_mosi <= 0;/* 4 */ #40 spi_mosi <= 0;
			#40 spi_mosi <= 0;/* 5 */ #40 spi_mosi <= 0;
			#40 spi_mosi <= 0;/* 6 */ #40 spi_mosi <= 0;
			#40 spi_mosi <= 0;/* 7 */ #40 spi_mosi <= 0;
			//
			#20 spi_mosi <= 0;
			//
			#300 spi_cs <= 1;
			//
			//#50 reset <= 1;
			//#5 reset <= 0;
			#80 $finish; 
		end 
	//
	always #1 clk <= !clk;
	always #40 
	begin
		if (!spi_cs) spi_clk <= !spi_clk;
		else spi_clk <= 0;
	end
	//
	SPI spiHost(clk, 
	            spi_cs, 
		        spi_clk, 
			    spi_mosi, 
			    data_tx,
			    reset,
			    spi_miso, 
			    data_rx
			    );
	defparam spiHost.SPI_FILTER = 2;
endmodule
//
/*
cd "$(FULL_CURRENT_PATH)"
build.bat
*/