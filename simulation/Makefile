#
## Poorman's Standard-Emulator by GoodKook, goodkook@gmail.com
#	Makefile for Co-Simulation of sample project "signed_mult8"
#

VERILOG_SRCS = ../source/signed_mult8.v \
               ../source/signed_multiplier.v
SC_SRCS      = sc_main.cpp \
               sc_signed_mult8_TB.cpp
SC_TOP_H     = sc_signed_mult8_TB.h

VERILATOR    = verilator
CFLAGS       = -std=c++17
CFLAGS1      = -g
#VFLAGS       = -DENT_ENP_CONTROL

LDFLAGS      = -lgsl

TOP_MODULE   = signed_mult8
TARGET       = V$(TOP_MODULE)
TARGET_DIR   = obj_dir

export SYSTEMC			= /usr/local/systemc-3.0.0
export SYSTEMC_HOME		= $(SYSTEMC)
export SYSTEMC_INCLUDE	= $(SYSTEMC_HOME)/include
export SYSTEMC_LIBDIR	= $(SYSTEMC_HOME)/lib-linux64
export LD_LIBRARY_PATH	:=$(LD_LIBRARY_PATH):$(SYSTEMC_LIBDIR)
export CXX				= clang++
export CXXFLAGS			= -std=c++17

all :
	@echo 'Makefile for "signed_mult8" Co-Simulation: Verilog RTL(*) under SystemC Testbench'
	@echo '  Poorman`s Std. Co-Emulation(PSCE), GoodKook, goodkook@gmail.com'
	@echo ''
	@echo '    make build'
	@echo '        Build executable of "signed_mult8" with Verilated RTL and SystemC TB'
	@echo ''
	@echo '    make run'
	@echo '        Simulate "signed_mult8"'
	@echo ''
	@echo '    make wave'
	@echo '        View SystemC generated waveform'
	@echo ''
	@echo '    make clean'
	@echo '        Clean working folder'
	@echo ''

build : $(TARGET_DIR)/$(TARGET)

$(TARGET_DIR)/$(TARGET) : $(VERILOG_SRCS) $(SC_SRCS) $(SC_TOP_H)
	$(VERILATOR) --sc -Wall --top-module $(TOP_MODULE) --exe --build \
		-CFLAGS $(CFLAGS) \
		-CFLAGS $(CFLAGS1) \
		-LDFLAGS $(LDFLAGS) \
		$(VFLAGS) \
		$(VERILOG_SRCS) $(SC_SRCS)

lint : $(VERILOG_SRCS)
	$(VERILATOR) --sc -Wall --top-module $(TOP_MODULE) $(VERILOG_SRCS)

run : $(TARGET_DIR)/$(TARGET)
	./$(TARGET_DIR)/$(TARGET)

wave : sc_signed_mult8_TB.vcd
	gtkwave sc_signed_mult8_TB.vcd --save=sc_signed_mult8_TB.gtkw &

clean :
	rm -rf $(TARGET_DIR)
	rm -f *.vcd

debug : $(TARGET_DIR)/$(TARGET)
	ddd $(TARGET_DIR)/$(TARGET)

