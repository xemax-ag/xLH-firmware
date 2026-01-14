module BIT8_TO_BYTE(
	input IN0,
	input IN1,
	input IN2,
	input IN3,
	input IN4,
	input IN5,
	input IN6,
	input IN7,
	output [7:0] OUT
	);
	//	
	assign OUT[0] = IN0;	
	assign OUT[1] = IN1;	
	assign OUT[2] = IN2;	
	assign OUT[3] = IN3;	
	assign OUT[4] = IN4;	
	assign OUT[5] = IN5;	
	assign OUT[6] = IN6;
	assign OUT[7] = IN7;
	//
endmodule
//
module BYTE_TO_BIT8(
	input [7:0] IN,
	output OUT0,
	output OUT1,
	output OUT2,
	output OUT3,
	output OUT4,
	output OUT5,
	output OUT6,
	output OUT7
	);
	//	
	assign OUT0 = IN[0];	
	assign OUT1 = IN[1];	
	assign OUT2 = IN[2];	
	assign OUT3 = IN[3];	
	assign OUT4 = IN[4];	
	assign OUT5 = IN[5];	
	assign OUT6 = IN[6];
	assign OUT7 = IN[7];
	//
endmodule