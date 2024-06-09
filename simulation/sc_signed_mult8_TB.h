/*******************************************************************************
Poorman's Standard-Emulator
---------------------------
Vendor: GoodKook, goodkook@gmail.com
Associated Filename: sc_signed_mult8_TB.h
Purpose: Testbench for signed_mult8
Revision History: Jun. 1, 2024
*******************************************************************************/
#ifndef _SC_SIGNED_MULT8_TB_H_
#define _SC_SIGNED_MULT8_TB_H_

#include <systemc.h>
#include "Vsigned_mult8.h"
#ifdef CO_EMULATION
#include "signed_mult8.h"
#endif

SC_MODULE(sc_signed_mult8_TB)
{
    sc_clock                clk;
    sc_signal<bool>         ABSel;
    // Verilator treats all Verilog's vector as <uint32_t>
    sc_signal<uint32_t>     ABin;
    sc_signal<uint32_t>     RES;
    
    // Exact DUT ports' vector width
    sc_signal<sc_uint<8> >  ABin_dut;
    sc_signal<sc_uint<16> > RES_dut;
    
    // Verilated DUT or Foreign Verilog
    Vsigned_mult8*   u_Vsigned_mult8;
    
#ifdef CO_EMULATION
    // Emulator DUT
    signed_mult8*    u_signed_mult8;
    sc_signal<sc_uint<16> > RES_emu;
#endif

    // Convert Verilator's ports to DUT's ports
    void conv_method()
    {
        ABin_dut.write((sc_uint<8>)ABin);
        RES_dut.write((sc_uint<16>)RES);
    }

    void test_generator();
    void monitor();
    
    sc_trace_file* fp;  // VCD file

    SC_CTOR(sc_signed_mult8_TB) :   // Constructor
        clk("clk", 100, SC_NS, 0.5, 0.0, SC_NS, false)
    {

        SC_THREAD(test_generator);
        sensitive << clk;

        SC_THREAD(monitor);
        sensitive << clk;

        SC_METHOD(conv_method);
        sensitive << ABin << RES;

        // DUT Instantiation
        u_Vsigned_mult8 = new Vsigned_mult8("u_Vsigned_mult8");
        // Binding
        u_Vsigned_mult8->clk(clk);
        u_Vsigned_mult8->ABin(ABin);
        u_Vsigned_mult8->ABSel(ABSel);
        u_Vsigned_mult8->RES(RES);

#ifdef CO_EMULATION
        u_signed_mult8 = new signed_mult8("u_signed_mult8");
        // Binding
        u_signed_mult8->clk(clk);
        u_signed_mult8->ABin(ABin_dut);
        u_signed_mult8->ABSel(ABSel);
        u_signed_mult8->RES(RES_emu);
#endif

        // VCD Trace
        fp = sc_create_vcd_trace_file("sc_signed_mult8_TB");
        fp->set_time_unit(100, SC_PS);
        sc_trace(fp, clk,       "clk");
        sc_trace(fp, ABSel,     "ABSel");
        sc_trace(fp, ABin_dut,  "ABin");
        sc_trace(fp, RES_dut,   "RES_dut");
#ifdef CO_EMULATION
        sc_trace(fp, RES_emu,   "RES_emu");
#endif
    }

    // Destructor
    ~sc_signed_mult8_TB()
    {}

};

#endif // _SC_SIGNED_MULT8_H_
