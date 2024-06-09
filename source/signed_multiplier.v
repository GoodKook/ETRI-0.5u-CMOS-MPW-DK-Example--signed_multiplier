//
// File: N-bit signed_multiplier
//

module signed_multiplier #
    (   parameter WIDTHA = 8,
        parameter WIDTHB = 8)
    (   input signed  [WIDTHA-1:0] A,
        input signed  [WIDTHB-1:0] B,
        output signed [WIDTHA+WIDTHB-1:0] RES);

    assign RES = A * B;

endmodule
