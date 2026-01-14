:: NPP exec
:: cd "$(FULL_CURRENT_PATH)" 
:: _build.bat


C:\iverilog\bin\iverilog -s ICARUS_TB -o icarus_tb.o ICARUS_TB.v
C:\iverilog\bin\vvp icarus_tb.o