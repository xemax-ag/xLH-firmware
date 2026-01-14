////////////////////////////////////////////////////////////////////////////////////////////
module DATA_IN_VAR(
	//
	input [7:0] FPGA_TO_UC_8BIT_01,
	input [7:0] FPGA_TO_UC_8BIT_02,
	input [7:0] FPGA_TO_UC_8BIT_03,
	input [7:0] FPGA_TO_UC_8BIT_04,
	input [7:0] FPGA_TO_UC_8BIT_05,
	input [7:0] FPGA_TO_UC_8BIT_06,
	input [7:0] FPGA_TO_UC_8BIT_07,
	input [7:0] FPGA_TO_UC_8BIT_08,
	//
	input [15:0] FPGA_TO_UC_16BIT_01,
	input [15:0] FPGA_TO_UC_16BIT_02,
	input [15:0] FPGA_TO_UC_16BIT_03,
	input [15:0] FPGA_TO_UC_16BIT_04,
	input [15:0] FPGA_TO_UC_16BIT_05,
	input [15:0] FPGA_TO_UC_16BIT_06,
	input [15:0] FPGA_TO_UC_16BIT_07,
	input [15:0] FPGA_TO_UC_16BIT_08,
	input [15:0] FPGA_TO_UC_16BIT_09,
	input [15:0] FPGA_TO_UC_16BIT_10,
	input [15:0] FPGA_TO_UC_16BIT_11,
	input [15:0] FPGA_TO_UC_16BIT_12,
	input [15:0] FPGA_TO_UC_16BIT_13,
	input [15:0] FPGA_TO_UC_16BIT_14,
	input [15:0] FPGA_TO_UC_16BIT_15,
	input [15:0] FPGA_TO_UC_16BIT_16,
	input [15:0] FPGA_TO_UC_16BIT_17,
	input [15:0] FPGA_TO_UC_16BIT_18,
	input [15:0] FPGA_TO_UC_16BIT_19,
	input [15:0] FPGA_TO_UC_16BIT_20,
	//
	input [31:0] FPGA_TO_UC_32BIT_01,
	input [31:0] FPGA_TO_UC_32BIT_02,
	input [31:0] FPGA_TO_UC_32BIT_03,
	input [31:0] FPGA_TO_UC_32BIT_04,
	//
	output [511:0] DATA
	);
	//
	UC_8BIT  uc_8bit_01(FPGA_TO_UC_8BIT_01[7:0],    DATA[007:000]);
	UC_8BIT  uc_8bit_02(FPGA_TO_UC_8BIT_02[7:0],    DATA[015:008]);
	UC_8BIT  uc_8bit_03(FPGA_TO_UC_8BIT_03[7:0],    DATA[023:016]);
	UC_8BIT  uc_8bit_04(FPGA_TO_UC_8BIT_04[7:0],    DATA[031:024]);
	UC_8BIT  uc_8bit_05(FPGA_TO_UC_8BIT_05[7:0],    DATA[039:032]);
	UC_8BIT  uc_8bit_06(FPGA_TO_UC_8BIT_06[7:0],    DATA[047:040]);
	UC_8BIT  uc_8bit_07(FPGA_TO_UC_8BIT_07[7:0],    DATA[055:048]);
	UC_8BIT  uc_8bit_08(FPGA_TO_UC_8BIT_08[7:0],    DATA[063:056]);
	//
	UC_16BIT uc_16bit_01(FPGA_TO_UC_16BIT_01[15:0], DATA[079:064]);
	UC_16BIT uc_16bit_02(FPGA_TO_UC_16BIT_02[15:0], DATA[095:080]);
	UC_16BIT uc_16bit_03(FPGA_TO_UC_16BIT_03[15:0], DATA[111:096]);
	UC_16BIT uc_16bit_04(FPGA_TO_UC_16BIT_04[15:0], DATA[127:112]);
	UC_16BIT uc_16bit_05(FPGA_TO_UC_16BIT_05[15:0], DATA[143:128]);
	UC_16BIT uc_16bit_06(FPGA_TO_UC_16BIT_06[15:0], DATA[159:144]);
	UC_16BIT uc_16bit_07(FPGA_TO_UC_16BIT_07[15:0], DATA[175:160]);
	UC_16BIT uc_16bit_08(FPGA_TO_UC_16BIT_08[15:0], DATA[191:176]);
	UC_16BIT uc_16bit_09(FPGA_TO_UC_16BIT_09[15:0], DATA[207:192]);
	UC_16BIT uc_16bit_10(FPGA_TO_UC_16BIT_10[15:0], DATA[223:208]);
	UC_16BIT uc_16bit_11(FPGA_TO_UC_16BIT_11[15:0], DATA[239:224]);
	UC_16BIT uc_16bit_12(FPGA_TO_UC_16BIT_12[15:0], DATA[255:240]);
	UC_16BIT uc_16bit_13(FPGA_TO_UC_16BIT_13[15:0], DATA[271:256]);
	UC_16BIT uc_16bit_14(FPGA_TO_UC_16BIT_14[15:0], DATA[287:272]);
	UC_16BIT uc_16bit_15(FPGA_TO_UC_16BIT_15[15:0], DATA[303:288]);
	UC_16BIT uc_16bit_16(FPGA_TO_UC_16BIT_16[15:0], DATA[319:304]);
	UC_16BIT uc_16bit_17(FPGA_TO_UC_16BIT_17[15:0], DATA[335:320]);
	UC_16BIT uc_16bit_18(FPGA_TO_UC_16BIT_18[15:0], DATA[351:336]);
	UC_16BIT uc_16bit_19(FPGA_TO_UC_16BIT_19[15:0], DATA[367:352]);
	UC_16BIT uc_16bit_20(FPGA_TO_UC_16BIT_20[15:0], DATA[383:368]);
	//
	UC_32BIT uc_32bit_01(FPGA_TO_UC_32BIT_01[31:0], DATA[415:384]);
	UC_32BIT uc_32bit_02(FPGA_TO_UC_32BIT_02[31:0], DATA[447:416]);
	UC_32BIT uc_32bit_03(FPGA_TO_UC_32BIT_03[31:0], DATA[479:448]);
	UC_32BIT uc_32bit_04(FPGA_TO_UC_32BIT_04[31:0], DATA[511:480]);
	//
	//Prozessabbild: 64 Bytes
endmodule
////////////////////////////////////////////////////////////////////////////////////////////
module DATA_OUT_VAR(
	//
	input [511:0] DATA,
	//
	output [7:0] UC_TO_FPGA_8BIT_01,
	output [7:0] UC_TO_FPGA_8BIT_02,
	output [7:0] UC_TO_FPGA_8BIT_03,
	output [7:0] UC_TO_FPGA_8BIT_04,
	output [7:0] UC_TO_FPGA_8BIT_05,
	output [7:0] UC_TO_FPGA_8BIT_06,
	output [7:0] UC_TO_FPGA_8BIT_07,
	output [7:0] UC_TO_FPGA_8BIT_08,
	output [7:0] UC_TO_FPGA_8BIT_09,
	output [7:0] UC_TO_FPGA_8BIT_10,
	output [7:0] UC_TO_FPGA_8BIT_11,
	output [7:0] UC_TO_FPGA_8BIT_12,
	output [7:0] UC_TO_FPGA_8BIT_13,
	output [7:0] UC_TO_FPGA_8BIT_14,
	output [7:0] UC_TO_FPGA_8BIT_15,
	output [7:0] UC_TO_FPGA_8BIT_16,
	output [7:0] UC_TO_FPGA_8BIT_17,
	output [7:0] UC_TO_FPGA_8BIT_18,
	output [7:0] UC_TO_FPGA_8BIT_19,
	output [7:0] UC_TO_FPGA_8BIT_20,
	output [7:0] UC_TO_FPGA_8BIT_21,
	output [7:0] UC_TO_FPGA_8BIT_22,
	output [7:0] UC_TO_FPGA_8BIT_23,
	output [7:0] UC_TO_FPGA_8BIT_24,
	//
	output [15:0] UC_TO_FPGA_16BIT_01,
	output [15:0] UC_TO_FPGA_16BIT_02,
	output [15:0] UC_TO_FPGA_16BIT_03,
	output [15:0] UC_TO_FPGA_16BIT_04,
	output [15:0] UC_TO_FPGA_16BIT_05,
	output [15:0] UC_TO_FPGA_16BIT_06,
	output [15:0] UC_TO_FPGA_16BIT_07,
	output [15:0] UC_TO_FPGA_16BIT_08,
	output [15:0] UC_TO_FPGA_16BIT_09,
	output [15:0] UC_TO_FPGA_16BIT_10,
	output [15:0] UC_TO_FPGA_16BIT_11,
	output [15:0] UC_TO_FPGA_16BIT_12,
	//
	output [31:0] UC_TO_FPGA_32BIT_01,
	output [31:0] UC_TO_FPGA_32BIT_02,
	output [31:0] UC_TO_FPGA_32BIT_03,
	output [31:0] UC_TO_FPGA_32BIT_04
	);
	//
	UC_8BIT uc_8bit_01(DATA[007:000],   UC_TO_FPGA_8BIT_01[7:0]);
	UC_8BIT uc_8bit_02(DATA[015:008],   UC_TO_FPGA_8BIT_02[7:0]);
	UC_8BIT uc_8bit_03(DATA[023:016],   UC_TO_FPGA_8BIT_03[7:0]);
	UC_8BIT uc_8bit_04(DATA[031:024],   UC_TO_FPGA_8BIT_04[7:0]);
	UC_8BIT uc_8bit_05(DATA[039:032],   UC_TO_FPGA_8BIT_05[7:0]);
	UC_8BIT uc_8bit_06(DATA[047:040],   UC_TO_FPGA_8BIT_06[7:0]);
	UC_8BIT uc_8bit_07(DATA[055:048],   UC_TO_FPGA_8BIT_07[7:0]);
	UC_8BIT uc_8bit_08(DATA[063:056],   UC_TO_FPGA_8BIT_08[7:0]);
	UC_8BIT uc_8bit_09(DATA[071:064],   UC_TO_FPGA_8BIT_09[7:0]);
	UC_8BIT uc_8bit_10(DATA[079:072],   UC_TO_FPGA_8BIT_10[7:0]);
	UC_8BIT uc_8bit_11(DATA[087:080],   UC_TO_FPGA_8BIT_11[7:0]);
	UC_8BIT uc_8bit_12(DATA[095:088],   UC_TO_FPGA_8BIT_12[7:0]);
	UC_8BIT uc_8bit_13(DATA[103:096],   UC_TO_FPGA_8BIT_13[7:0]);
	UC_8BIT uc_8bit_14(DATA[111:104],   UC_TO_FPGA_8BIT_14[7:0]);
	UC_8BIT uc_8bit_15(DATA[119:112],   UC_TO_FPGA_8BIT_15[7:0]);
	UC_8BIT uc_8bit_16(DATA[127:120],   UC_TO_FPGA_8BIT_16[7:0]);
	UC_8BIT uc_8bit_17(DATA[135:128],   UC_TO_FPGA_8BIT_17[7:0]);
	UC_8BIT uc_8bit_18(DATA[143:136],   UC_TO_FPGA_8BIT_18[7:0]);
	UC_8BIT uc_8bit_19(DATA[151:144],   UC_TO_FPGA_8BIT_19[7:0]);
	UC_8BIT uc_8bit_20(DATA[159:152],   UC_TO_FPGA_8BIT_20[7:0]);
	UC_8BIT uc_8bit_21(DATA[167:160],   UC_TO_FPGA_8BIT_21[7:0]);
	UC_8BIT uc_8bit_22(DATA[175:168],   UC_TO_FPGA_8BIT_22[7:0]);
	UC_8BIT uc_8bit_23(DATA[183:176],   UC_TO_FPGA_8BIT_23[7:0]);
	UC_8BIT uc_8bit_24(DATA[191:184],   UC_TO_FPGA_8BIT_24[7:0]);
	//
	UC_16BIT uc_16bit_01(DATA[207:192], UC_TO_FPGA_16BIT_01[15:0]);
	UC_16BIT uc_16bit_02(DATA[223:208], UC_TO_FPGA_16BIT_02[15:0]);
	UC_16BIT uc_16bit_03(DATA[239:224], UC_TO_FPGA_16BIT_03[15:0]);
	UC_16BIT uc_16bit_04(DATA[255:240], UC_TO_FPGA_16BIT_04[15:0]);
	UC_16BIT uc_16bit_05(DATA[271:256], UC_TO_FPGA_16BIT_05[15:0]);
	UC_16BIT uc_16bit_06(DATA[287:272], UC_TO_FPGA_16BIT_06[15:0]);
	UC_16BIT uc_16bit_07(DATA[303:288], UC_TO_FPGA_16BIT_07[15:0]);
	UC_16BIT uc_16bit_08(DATA[319:304], UC_TO_FPGA_16BIT_08[15:0]);
	UC_16BIT uc_16bit_09(DATA[335:320], UC_TO_FPGA_16BIT_09[15:0]);
	UC_16BIT uc_16bit_10(DATA[351:336], UC_TO_FPGA_16BIT_10[15:0]);
	UC_16BIT uc_16bit_11(DATA[367:352], UC_TO_FPGA_16BIT_11[15:0]);
	UC_16BIT uc_16bit_12(DATA[383:368], UC_TO_FPGA_16BIT_12[15:0]);
	//
	UC_32BIT uc_32bit_01(DATA[415:384], UC_TO_FPGA_32BIT_01[31:0]);
	UC_32BIT uc_32bit_02(DATA[447:416], UC_TO_FPGA_32BIT_02[31:0]);
	UC_32BIT uc_32bit_03(DATA[479:448], UC_TO_FPGA_32BIT_03[31:0]);
	UC_32BIT uc_32bit_04(DATA[511:480], UC_TO_FPGA_32BIT_04[31:0]);
	//
	//Prozessabbild: 64 Bytes
endmodule
////////////////////////////////////////////////////////////////////////////////////////////
module UC_32BIT(
	input [31:0] IN,
	output [31:0] OUT
	);
	//
	UC_8BIT teensy_8bit_1(IN[31:24], OUT[31:24]);
	UC_8BIT teensy_8bit_2(IN[23:16], OUT[23:16]);
	UC_8BIT teensy_8bit_3(IN[15:08], OUT[15:08]);
	UC_8BIT teensy_8bit_4(IN[07:00], OUT[07:00]);
	//
endmodule
//
module UC_16BIT(
	input [15:0] IN,
	output [15:0] OUT
	);
	//
	UC_8BIT teensy_8bit_1(IN[15:08], OUT[15:08]);
	UC_8BIT teensy_8bit_2(IN[07:00], OUT[07:00]);
	//
endmodule
//
module UC_8BIT(
	input [7:0] IN,
	output [7:0] OUT
	);
	//	
	assign OUT[0] = IN[7];	
	assign OUT[1] = IN[6];	
	assign OUT[2] = IN[5];	
	assign OUT[3] = IN[4];	
	assign OUT[4] = IN[3];	
	assign OUT[5] = IN[2];	
	assign OUT[6] = IN[1];
	assign OUT[7] = IN[0];
	//
endmodule
////////////////////////////////////////////////////////////////////////////////////////////
