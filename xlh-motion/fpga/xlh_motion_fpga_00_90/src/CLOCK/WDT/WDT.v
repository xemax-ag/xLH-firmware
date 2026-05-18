////////////////////////////////////////////////////////////////////////////////////////////
module WDT( 
	input CLK,
	input CLK_1kHz,
	input IN,
	input ENABLE_SPI,
	output reg RESET
	);
	//
	parameter DELAY_TIME = 16'd1000;
	parameter RESET_TIME = 16'd50;
	parameter ENABLE = 1'b1;
	//
	reg [15:0] counter;
	reg [0:0] status;
	//
	reg hri1; reg hri2; reg hri3; reg hri4;
	//
	wire 	imp1kHz;
	//wire 	imp1Hz_n;
	wire 	impIn;
	//wire 	impUc_n;	
	//
	assign	imp1kHz = (hri1 && !hri2);
	//assign	imp1kHz_n = (!hri1 && hri2);
	assign	impIn = (hri3 && !hri4);
	//assign	impIn_n = (!hri3 && hri4);
	//
	initial
	begin
		RESET <= 0;
		//
		counter <= 0;
		status <= 0;
		//
		hri1 <= 0; hri2 <= 0; hri3 <= 0; hri4 <= 0;	
	end
	//
	always	@(posedge CLK)
	begin
	hri1 <= CLK_1kHz;
	hri2 <= hri1;	
	hri3 <= IN;
	hri4 <= hri3;	
	//
	if (imp1kHz) 
		begin
		if (counter < 65535) counter <= counter + 1;
		end
	//
	if (impIn) 
		begin
		counter <= 1;
		status <= 0;
		end
	//
	if (counter >= DELAY_TIME) 
		begin
		status <= 1;
		end
	//
	if (counter >= (DELAY_TIME + RESET_TIME)) 
		begin
		status <= 0;
		end
	//
	if (status == 0) 
		begin
		RESET <= 0;
		end
	//
	if (status == 1)
		begin
		if (ENABLE && ENABLE_SPI) RESET <= 1;
		else RESET <= 0;
		end
	//	
	end
endmodule
////////////////////////////////////////////////////////////////////////////////////////////
