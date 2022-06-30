#include "fir.h"

// Not optimzied code in Figure 2.1

void fir(d_stream& y, d_stream& x){
#pragma HLS INTERFACE mode=ap_ctrl_none port=return
#pragma HLS INTERFACE mode=axis register_mode=both port=y
#pragma HLS INTERFACE mode=axis register_mode=both port=x
#pragma HLS PIPELINE style=frp
    coef_t c[N] = {
        53, 0, -91, 0, 313, 500, 313, 0, -91, 0, 53
    };
    static data_t shift_reg[N];
#pragma HLS array_partition type=complete variable=shift_reg dim=1
    data_t_pack x_temp;
    acc_t acc = 0;
    int i;
    x >> x_temp;

// Algorithm
TDL:
    for (i = N - 1; i > 0;i--){
#pragma HLS unroll
        shift_reg[i] = shift_reg[i-1];
    }
    shift_reg[0] = x_temp.data;

MAC:
    for (i = N - 1; i >= 0;i--){
#pragma HLS unroll
        acc += shift_reg[i] * c[i];
    }
    
// Output Stream
    data_t_pack y_temp;
    y_temp.data = acc;
    y_temp.keep = -1;
    y_temp.last = x_temp.last;
    y << y_temp;
    if (x_temp.last){
Clear_Loop:
    	for (i = N - 1; i >= 0;i--){
#pragma HLS unroll
            shift_reg[i] = 0;
        }
    }
}

