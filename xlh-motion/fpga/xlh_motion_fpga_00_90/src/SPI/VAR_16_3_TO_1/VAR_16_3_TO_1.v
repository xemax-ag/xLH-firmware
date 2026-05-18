module VAR_16_3_TO_1(
	input CLK,
	input signed [15:0] IN1,
	input signed [15:0] IN2,
	input signed [15:0] IN3,
	output reg signed [15:0] OUT
	);
	//
	reg signed [15:0] temp;
	reg [2:0] state;
	//
	initial
	begin
		OUT <= 0;
		temp <= 0;
		state <= 0;
	end	
	//
	always	@(posedge CLK)
	begin
		//
		if (state == 0) 
			begin
			temp <= IN1 + IN2;
			state <= state + 1;
			end
		//
		if (state == 1) 
			begin
			OUT <= temp;
			OUT <= temp + IN3;
			state <= 0;
			end
		//
		if (state > 1) 
			begin
			state <= 0;
			end
		//
	end
endmodule