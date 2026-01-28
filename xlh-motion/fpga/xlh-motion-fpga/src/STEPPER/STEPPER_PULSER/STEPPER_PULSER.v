module STEPPER_PULSER (
	input CLK,
	input CLK_CHANGE,
	input EN,
	input DIR,
	input INVERTED,
	input IN_MOTION,
	input [31:0] CLK_CMP,
	output reg DIR_OUT,
	output reg PULS);
	//
	parameter CTR_PULS_LEN = 32'd1000;
	parameter CTR_LIMIT_MIN = 32'd2000;
	parameter CTR_LIMIT_MAX = 32'd5000000;
	//
	reg clk_cmp_change_enable; 

	reg [31:0] ctr;
	reg [31:0] clk_cmp_temp;
	//
	wire clk_change_re;
	wire clk_change_fe;
	//
	initial
		begin
			DIR_OUT <= 0;
			PULS <= 0;
			//
			clk_cmp_change_enable <= 0;
			ctr <= 0;
			clk_cmp_temp <= 0;
			//hri1 <= 0; hri2 <= 0;
		end
	//
	always @(posedge CLK)
	begin
	//
	if ((CLK_CMP >= CTR_LIMIT_MIN) && (CLK_CMP <= CTR_LIMIT_MAX))
		clk_cmp_change_enable <= 1;
	else
		clk_cmp_change_enable <= 0;
	//
	if (clk_cmp_change_enable)
			clk_cmp_temp <= CLK_CMP - 1;
	//
	if (IN_MOTION)
		begin
		ctr <= ctr + 1;
		if (ctr >= clk_cmp_temp)
			ctr <= 0;
		end
	else
		if (PULS)
			ctr <= ctr + 1;
		else
			ctr <= 0;
	//
	if ((ctr < CTR_PULS_LEN) && (ctr > 0))
		begin
		PULS <= 1;
		//
		if (!INVERTED) DIR_OUT <= DIR;
		else DIR_OUT <= !DIR;
		end
	else
		PULS <= 0;
	//
	end
endmodule