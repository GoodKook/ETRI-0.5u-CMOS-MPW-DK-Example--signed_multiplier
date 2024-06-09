/*******************************************************************************
Poorman's Standard-Emulator
---------------------------
Vendor: GoodKook, goodkook@gmail.com
Associated Filename: sc_signed_mult8_TB.cpp
Purpose: Testbench for signed_mult8
Revision History: Jun. 1, 2024
*******************************************************************************/
#include "sc_signed_mult8_TB.h"

#include <iostream>
#include <iomanip>
#include <gsl/gsl_rng.h>

void sc_signed_mult8_TB::test_generator()
{
    gsl_rng *r = gsl_rng_alloc (gsl_rng_taus);

    for (int n=0; n<10000; n++)
    {
        ABin.write((int8_t)(gsl_rng_uniform (r)*256.0));
        ABSel.write(false);
        wait(clk.posedge_event());  // Load A

        ABin.write((int8_t)(gsl_rng_uniform (r)*256.0));
        ABSel.write(true);
        wait(clk.posedge_event());  // Load B
        
        ABSel.write(false);
        wait(clk.posedge_event());  // Multiply

        ABSel.write(true);
        wait(clk.posedge_event());  // Result
    }

    gsl_rng_free (r);
    sc_stop();
}

void sc_signed_mult8_TB::monitor()
{
    static int n = 0;
    int8_t  _A, _B;
    int16_t _RES_dut, _RES_mult;
#ifdef CO_EMULATION
    int16_t _RES_emu;
#endif

    while(true)
    {
        wait(clk.posedge_event());
        _A = (int8_t)ABin.read();
        
        wait(clk.posedge_event());
        _B = (int8_t)ABin.read();

        wait(clk.posedge_event());  // Multiply

        wait(clk.posedge_event());  // Result
        _RES_dut = (int16_t)RES_dut.read();
        _RES_mult = _A * _B;
#ifdef CO_EMULATION
        _RES_emu = RES_emu.read();
        printf("Iteration=%5d, A=%5d, B=%5d, RES=%7d[%7d]<%7d> ", n,
                    _A, _B, _RES_dut, _RES_mult, _RES_emu);
        if (_RES_dut!=_RES_emu)    cout << "Error ";
#else
        printf("Iteration=%5d, A=%5d, B=%5d, RES=%7d[%7d] ", n,
                    _A, _B, _RES_dut, _RES_mult);
        if (_RES_mult!=_RES_dut)    cout << "Error ";
#endif
        cout << "@" << sc_time_stamp() << endl;

        n++;
    }
}
