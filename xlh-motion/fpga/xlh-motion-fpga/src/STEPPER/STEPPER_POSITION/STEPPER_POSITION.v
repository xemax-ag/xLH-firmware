module STEPPER_POSITION (
	input CLK,
	input DIR,
	input PULS,
	input EN,
	input RESET,
	input INVERTED,
	output reg signed [31:0] POSITION);
	//
	reg hri1; reg hri2; reg hri3; reg hri4;
	//
	//wire puls_re;
	wire puls_fe;
	//
	//assign puls_re = (hri1 && !hri2);
	assign puls_fe = (!hri1 && hri2);	
	//
	initial
		begin
			POSITION <= 0;
			hri1 <= 0; hri2 <= 0;
		end
	//
	always @(posedge CLK)
	begin
	//
	hri1 <= PULS;
	hri2 <= hri1;	
	//
	if (!INVERTED) 
		begin
		if (puls_fe && DIR)       
			POSITION <= POSITION + 1;
		else if (puls_fe && !DIR) 
			POSITION <= POSITION - 1;
		else if (RESET || !EN)    
			POSITION <= 0;
		end
	else
		begin
		if (puls_fe && DIR)       
			POSITION <= POSITION - 1;
		else if (puls_fe && !DIR) 
			POSITION <= POSITION + 1;
		else if (RESET || !EN)    
			POSITION <= 0;
		end
	//	
	end
endmodule