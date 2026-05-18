module VAR_8_3_TO_1(
	input [7:0] IN1,
	input [7:0] IN2,
	input [7:0] IN3,
	output [7:0] OUT);
	
	assign OUT = IN1 | IN2 | IN3;
	
endmodule