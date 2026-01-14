module VAR_12_TO_16_BIT (
	input CLK,
	input [11:0] IN,
	output reg [15:0] OUT
	);
	//	
	initial
	begin
		OUT <= 0;
	end	
	//
	always	@(posedge CLK)
	begin
	//OUT[11:0] <= 1234;//IN[11:0];
	OUT[11:0] <= IN[11:0];
	end
endmodule