module 	CLOCK_GENERATOR(
	input CLK, 
	output reg CLOCK_5MHz,
	output reg CLOCK_1MHz,
	output reg CLOCK_500kHz,	
	output reg CLOCK_100kHz,
	output reg CLOCK_50kHz,	
	output reg CLOCK_10kHz,	
	output reg CLOCK_5kHz,	
	output reg CLOCK_1kHz,
	output reg CLOCK_500Hz,
	output reg CLOCK_100Hz,
	output reg CLOCK_50Hz,
	output reg CLOCK_10Hz,
	output reg CLOCK_5Hz,
	output reg CLOCK_1Hz);
//
// ToDo: Optimierung der Bitbreite
reg	[31:0]counter5M;
reg	[31:0]counter1M;
reg	[31:0]counter500k;
reg	[31:0]counter100k;
reg	[31:0]counter50k;
reg	[31:0]counter10k;
reg	[31:0]counter5k;
reg	[31:0]counter1k;
reg	[31:0]counter100;
reg	[31:0]counter500;
reg	[31:0]counter50;
reg	[31:0]counter10;
reg	[31:0]counter5;
reg	[31:0]counter1;
//
initial begin
	counter5M <= 1;
 	counter1M <= 1;
	counter500k <= 1;
 	counter100k <= 1;
	counter50k <= 1;
	counter10k <= 1;
	counter5k <= 1;
	counter1k <= 1;
	counter500 <= 1;
	counter100 <= 1;
	counter10 <= 1;
	counter1 <= 1;
	CLOCK_5MHz <= 0;
	CLOCK_1MHz <= 0;
	CLOCK_500kHz <= 0;
	CLOCK_100kHz <= 0;
	CLOCK_50kHz <= 0;
	CLOCK_10kHz <= 0;
	CLOCK_5kHz <= 0;
	CLOCK_1kHz <= 0;
	CLOCK_500Hz <= 0;
	CLOCK_100Hz <= 0;
	CLOCK_50Hz <= 0;
	CLOCK_10Hz <= 0;
	CLOCK_5Hz <= 0;
	CLOCK_1Hz <= 0;
 end
//
always @(posedge CLK) begin
	//
	counter5M <= counter5M + 1;
	counter1M <= counter1M + 1;
	counter500k <= counter500k + 1;
	counter100k <= counter100k + 1;
	counter50k <= counter50k + 1;
	counter10k <= counter10k + 1;
	counter5k <= counter5k + 1;
 	counter1k <= counter1k + 1;
	counter500 <= counter500 + 1;
	counter100 <= counter100 + 1;
	counter50 <= counter50 + 1;
	counter10 <= counter10 + 1;
	counter5 <= counter5 + 1;
	counter1 <= counter1 + 1;
	//
	if(counter5M == 5) begin
		counter5M <= 1;
		CLOCK_5MHz <= !CLOCK_5MHz;
	end
	//
	if(counter1M == 25) begin
		counter1M <= 1;
		CLOCK_1MHz <= !CLOCK_1MHz;
	end
	//
	if(counter500k == 50) begin
		counter500k <= 1;
		CLOCK_500kHz <= !CLOCK_500kHz;
	end		
	//
	if(counter100k == 250) begin
		counter100k <= 1;
		CLOCK_100kHz <= !CLOCK_100kHz;
	end	
	//
	if(counter50k == 500) begin
		counter50k <= 1;
		CLOCK_50kHz <= !CLOCK_50kHz;
	end	
	//
	if(counter10k == 2500) begin
		counter10k <= 1;
		CLOCK_10kHz <= !CLOCK_10kHz;
	end	
	//
	if(counter5k == 5000) begin
		counter5k <= 1;
		CLOCK_5kHz <= !CLOCK_5kHz;
	end	
	//
	if(counter1k == 25000) begin
		counter1k <= 1;
		CLOCK_1kHz <= !CLOCK_1kHz;
	end		
	//
	if(counter500 == 50000) begin
		counter500 <= 1;
		CLOCK_500Hz <= !CLOCK_500Hz;
	end	
	//
	if(counter100 == 250000) begin
		counter100 <= 1;
		CLOCK_100Hz <= !CLOCK_100Hz;
	end	
	//
	if(counter50 == 500000) begin
		counter50 <= 1;
		CLOCK_50Hz <= !CLOCK_50Hz;
	end	
	//
	if(counter10 == 2500000) begin
		counter10 <= 1;
		CLOCK_10Hz <= !CLOCK_10Hz;
	end	
	//
	if(counter5 == 5000000) begin
		counter5 <= 1;
		CLOCK_5Hz <= !CLOCK_5Hz;
	end		
	//
	if(counter1 == 25000000) begin
		counter1 <= 1;
		CLOCK_1Hz <= !CLOCK_1Hz;
	end	
	//
end
//
endmodule

