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
	parameter DELTA_CLK_CMP = 32'd100;
	//
	reg clk_cmp_change_enable; 

	reg [31:0] ctr;
	reg [31:0] clk_cmp_calc;
	reg [31:0] clk_cmp_temp;
	//reg hri1; reg hri2;
	//
	wire clk_change_re;
	wire clk_change_fe;
	//
	//assign clk_change_re = (hri1 && !hri2);
	//assign clk_change_fe = (!hri1 && hri2);
	//
	initial
		begin
			DIR_OUT <= 0;
			PULS <= 0;
			//
			clk_cmp_change_enable <= 0;
			ctr <= 0;
			clk_cmp_calc <= 0;
			clk_cmp_temp <= 0;
			//hri1 <= 0; hri2 <= 0;
		end
	//
	always @(posedge CLK)
	begin
	/*
	//
	hri1 <= CLK_CHANGE;
	hri2 <= hri1;	
	//
	if (clk_change_re)
		begin
		if (CLK_CMP > clk_cmp_calc)
			if ((CLK_CMP - clk_cmp_calc) <= DELTA_CLK_CMP)
				clk_cmp_calc <= CLK_CMP;
			else
				clk_cmp_calc <= clk_cmp_calc + DELTA_CLK_CMP;
		//
		if (CLK_CMP < clk_cmp_calc)
			if ((clk_cmp_calc - CLK_CMP) <= DELTA_CLK_CMP)
				clk_cmp_calc <= CLK_CMP;
			else
				clk_cmp_calc <= clk_cmp_calc - DELTA_CLK_CMP;
		end
	//
	*/
	clk_cmp_calc <= CLK_CMP;
	//
	if ((clk_cmp_calc >= CTR_LIMIT_MIN) && (clk_cmp_calc <= CTR_LIMIT_MAX))
		clk_cmp_change_enable <= 1;
	else
		clk_cmp_change_enable <= 0;
	//
	if (clk_cmp_change_enable)
			clk_cmp_temp <= clk_cmp_calc - 1;
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