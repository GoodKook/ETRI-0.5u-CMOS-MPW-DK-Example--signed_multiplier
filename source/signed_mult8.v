//
// 8-bit signed multiplier
//

module signed_mult8(ABin, ABSel, RES, clk);
    input                   clk;
    input signed  [ 7:0]    ABin;
    input                   ABSel;
    output signed [15:0]    RES;

    reg  signed [15:0]   RES;
    
    reg  signed [ 7:0]   A, B;
    wire signed [15:0]   M;
    
    always @(posedge clk)
    begin
        if (ABSel==1'b0)
            A <= ABin;
        else
            B <= ABin;

        RES <= M;
    end

    signed_multiplier  #(
                            .WIDTHA(8),
                            .WIDTHB(8)
                        )
                        u_signed_multiplier
                        (
                            .A(A),
                            .B(B),
                            .RES(M)
                        );

endmodule
