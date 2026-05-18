module PWM(
	input CLK,
	input CLK_1KHZ,
	input [15:0] CMP,
	output reg OUT);
	//
	parameter TIME_VOLTAGE_INCREASE = 16'd1500;
	//
	reg [15:0] ctr;
	reg [15:0] dc_temp;
	reg [15:0] ctr_1khz;
	reg hri1; reg hri2;
	//
	wire clk_1KHZ_re;
	//
	assign clk_1KHZ_re = (hri1 && !hri2);
	//
	initial
	begin
		OUT <= 0;
		ctr <= 16'h0;
		dc_temp <= 16'h0;
		ctr_1khz <= 16'h0;
	end
	//
	always @(posedge CLK)
	begin
		//
		hri1 <= CLK_1KHZ;
		hri2 <= hri1;	
		//
		if (CMP > 16'h0800)
			begin
			if (clk_1KHZ_re && (ctr_1khz < TIME_VOLTAGE_INCREASE))
				ctr_1khz <= ctr_1khz + 1;
			end
		else
			ctr_1khz <= 0;
		if (ctr_1khz >= TIME_VOLTAGE_INCREASE)
			dc_temp <= 16'h0800;
		//
		ctr <= ctr + 1;
		if (ctr == 16'h0FFE)
			begin
			dc_temp <= CMP;
			ctr <= 0;
			end
		//
		if (ctr < dc_temp)
			OUT <= 1;
		else
			OUT <= 0;
		//
	end
endmodule