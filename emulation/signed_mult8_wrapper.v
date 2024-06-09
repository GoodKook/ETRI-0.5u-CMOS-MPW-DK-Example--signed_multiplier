//
// Poorman's Standard-Emulator by GoodKook, goodkook@gmail.com
//  Co-Emulation warapper for the "signed_mult8"
//

`ifdef EMU_MONITOR
module signed_mult8_wrapper(Din_emu, Dout_emu, Addr_emu, load_emu, get_emu, clk_emu, clk_dut, clk_LED);
`else
module signed_mult8_wrapper(Din_emu, Dout_emu, Addr_emu, load_emu, get_emu, clk_emu, clk_dut);
`endif
    input  [7:0]    Din_emu;
    output [7:0]    Dout_emu;
    input  [2:0]    Addr_emu;
    input           load_emu, get_emu, clk_emu;
    input           clk_dut;
`ifdef EMU_MONITOR
    output clk_LED;
    // Monitoring emulation process by blinking LED
    reg [3:0] counter;
    always @(posedge clk_dut)
    begin
        counter <= counter + 1;
    end
    assign clk_LED = counter[3];
`endif
    // Std. Emulation wrapper: Stimulus & Output capture for DUT
    parameter   NUM_STIM_ARRAY  = 2,
                NUM_OUT_ARRAY   = 2;
    reg [7:0]   stimIn[0:NUM_STIM_ARRAY-1];
    reg [7:0]   vectOut[0:NUM_OUT_ARRAY-1];
    reg [7:0]   Dout_emu;
    
    // DUT interface: registered input
    reg         ABSel;
    reg [ 7:0]  ABin;
    // DUT interface: output wire. DUT's output will be captured
    wire [15:0] RES;

    // Emulation Transactor ---------------------------------------------
    //  stimIn[0] = ABin
    //  stimIn[1] = {-------|ABSel}
    // vectOut[0] = RES[ 7:0]
    // vectOut[1] = RES[15:8]
    always @(posedge clk_emu)
    begin
        if (load_emu)       // Set input stimulus to DUT
        begin
            ABin  <= stimIn[0];
            ABSel <= stimIn[1][0];
        end
        else if (get_emu)   // Capure output from DUT
        begin
            vectOut[0] <= RES[ 7:0];
            vectOut[1] <= RES[15:8];
        end
        else
        begin
            stimIn[Addr_emu] <= Din_emu;    // stimulus: Host -> DUT
            Dout_emu <= vectOut[Addr_emu];  // output vector: DUT->Host
        end
    end

    // DUT --------------------------------------------------------------
    signed_mult8 u_signed_mult8 (
        .clk(clk_dut),
        .ABin(ABin),
        .ABSel(ABSel),
        .RES(RES));
endmodule
