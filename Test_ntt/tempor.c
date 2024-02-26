#include <stdio.h>
//#include "ntt_calc.h"
#include "ntt_fwd_inv.h"

int main() {

    //static test_case_t tests = {.m = 10, .q = 0x10001, .w = 33, .w_inv = 1986, .n_inv.op = 65473};
    static test_case_t tests = {.m = 10, .q = 12289, .w = 11, .w_inv = 5586, .n_inv.op = 12277};
   // static test_case_t tests = {.m = 10, .q = 12289, .w = 10302, .w_inv = 8974, .n_inv.op = 12277};
   // static test_case_t tests = {.m = 4, .q = 257, .w = 32, .w_inv = 2305, .n_inv.op = 755};
    _init_test(&tests);  //test_case parameter initiation

    const test_case_t *t;
    t = &tests;
    /**************************************************************************/

    ////prepare two input vector
    int s = t ->n;
    uint64_t a_in[s]; uint64_t * a_i = a_in;
    uint64_t a[s];  // a for total calculation
    for (int i = 0; i<(s); i++){
        a_in[i] = i;
        a[i] = i;
    }

    printf("Printing input\n");

    for (int i = 0; i < t->n; i++) {
        printf("%llu ", a_in[i]);
    }
    printf("\n");
    //// generate Forward NTT -- a
    fwd_ntt_ref_harvey(a, t->n, t->q, t->w_powers, t->w_powers_con, t->m);

    printf("Printing Forward NTT of a bit_rev\n");
    for (int i = 0; i < t->n; i++) {
        printf("%llu ", a[i]);
    }
    printf("\n");

    //generate Inverse NTT
    inv_ntt_ref_harvey(a, t->n, t->q, t->n_inv, WORD_SIZE, t->w_inv_powers,
                       t->w_inv_powers_con, t->m);


    //////////////////////////////////////////////////////////

    printf("Printing Reverse NTT of a bit_rev\n");

    for (int i = 0; i < t->n; i++) {
        printf("%llu ", a[i]);
    }

    printf("\n");

    ///////////////// stage0 256 times 4-elem FFT   in the block of 1024 tw[1024] 256-256-256-256   /////////////////////////////////////////////////
    uint64_t p = t->q;
    uint64_t temp1[1024];
    uint64_t temp2[1024];

    uint64_t s0[4];
    uint64_t w_powers_n0[4];
    uint64_t w_powers_con_n0[4];

    w_powers_n0[0] = t->w_powers[0];
    w_powers_n0[1] = t->w_powers[256];
    w_powers_n0[2] = t->w_powers[512];
    w_powers_n0[3] = t->w_powers[768];

    w_powers_con_n0[0] =  t->w_powers_con[0];
    w_powers_con_n0[1] =  t->w_powers_con[256];
    w_powers_con_n0[2] =  t->w_powers_con[512];
    w_powers_con_n0[3] =  t->w_powers_con[768];

    uint64_t tw0[1024];
    for (int i = 0; i < 256; i++){

        tw0[i]     = t->w_powers[0*i];
        tw0[i+256] = t->w_powers[1*i];
        tw0[i+512] = t->w_powers[2*i];
        tw0[i+768] = t->w_powers[3*i];
    }

    //printf("Printing tw_0\n");

   // for (int i = 0; i < t->n; i++) {
       // printf("%llu, ", tw0[i]);
    //}

   // printf("\n");


    for (int i =0; i < 256; i++){

        s0[0] = a_in[0+i];
        s0[1] = a_in[256+i];
        s0[2] = a_in[512+i];
        s0[3] = a_in[768+i];

        fwd_ntt_ref_harvey(s0, 4, t->q, w_powers_n0, w_powers_con_n0, 2);

        temp1[0+i]   = s0[0];
        temp1[256+i] = s0[1];
        temp1[512+i] = s0[2];
        temp1[768+i] = s0[3];

    }

    for (int i = 0; i < 1024; i++){
        temp2[i] = (temp1[i] * tw0[i] ) % p;
       // printf("%llu ", temp2[i]);
    }

    //////////////////////////////stage1  64 * 4 times 4-elem FFT  in the block of 256  tw[256] //////////////////////
    uint64_t tw1[256];
    for (int i = 0; i < 64; i++){

        tw1[i]     = t->w_powers[0*i];
        tw1[i+64]  = t->w_powers[1*i*4];
        tw1[i+128] = t->w_powers[2*i*4];
        tw1[i+192] = t->w_powers[3*i*4];
    }

   // printf("\n");

    for (int j = 0; j < 4; j++){
        for (int i =0; i < 64; i++){

            s0[0] = temp2[0+i+j*256];
            s0[1] = temp2[64+i+j*256];
            s0[2] = temp2[128+i+j*256];
            s0[3] = temp2[192+i+j*256];

            fwd_ntt_ref_harvey(s0, 4, t->q, w_powers_n0, w_powers_con_n0, 2);

            temp1[0+i+j*256]   = s0[0];
            temp1[64+i+j*256]  = s0[1];
            temp1[128+i+j*256] = s0[2];
            temp1[192+i+j*256] = s0[3];
        }
    }


    for (int j = 0; j < 4; j++){
        for (int i = 0; i < 256; i++){
            temp2[i + j*256] = (temp1[i+ j*256] * tw1[i] ) % p;
           // printf("%llu ", temp2[i]);
        }

    }

    //////////////////////stage2   16 * 4 * 4 = 16 * 16 times 4-elem FFT  in the block of 64    ////////////////

    uint64_t tw2[64];

    for (int i = 0; i < 16; i++){
        tw2[i]    = t->w_powers[0*i];
        tw2[i+16] = t->w_powers[1*i*4*4];
        tw2[i+32] = t->w_powers[2*i*4*4];
        tw2[i+48] = t->w_powers[3*i*4*4];
    }


   // printf("\n");

    for (int k = 0; k<4; k++){

        for (int j = 0; j < 4; j++){

            for (int i =0; i < 16; i++){

                s0[0] = temp2[ 0 + i + j*64+ k*256];
                s0[1] = temp2[16 + i + j*64+ k*256];
                s0[2] = temp2[32 + i + j*64+ k*256];
                s0[3] = temp2[48 + i + j*64+ k*256];

                fwd_ntt_ref_harvey(s0, 4, t->q, w_powers_n0, w_powers_con_n0, 2);

                temp1[ 0 + i + j*64+ k*256] = s0[0];
                temp1[16 + i + j*64+ k*256] = s0[1];
                temp1[32 + i + j*64+ k*256] = s0[2];
                temp1[48 + i + j*64+ k*256] = s0[3];

            }

        }

    }

    //// tw multiplication
    for (int k = 0; k < 4; k++){
        for (int j = 0; j < 4; j++){
            for (int i = 0; i < 64; i++){
                temp2[i + j*64 + k * 256] = (temp1[i + j*64 + k * 256] * tw1[i] ) % p;
               // printf("%llu ", temp2[i]);
            }
        }
    }

    ///////////////////////////   4* 4 * 4 * 4 = 64 * 4 times 4-elem FFT  in the block of 16    ////////////////

    uint64_t tw3[16];

    for (int i = 0; i < 4; i++){
        tw3[i]    = t->w_powers[0*i];
        tw3[i+4]  = t->w_powers[1*i*4*4*4];
        tw3[i+8]  = t->w_powers[2*i*4*4*4];
        tw3[i+12] = t->w_powers[3*i*4*4*4];
    }


    for (int l = 0; l < 4; l++){
        for (int k = 0; k<4; k++){
            for (int j = 0; j < 4; j++){
                for (int i =0; i < 4; i++){

                    s0[0] = temp2[ 0 + i + j * 16 + k * 64 + l * 256];
                    s0[1] = temp2[ 4 + i + j * 16 + k * 64 + l * 256];
                    s0[2] = temp2[ 8 + i + j * 16 + k * 64 + l * 256];
                    s0[3] = temp2[12 + i + j * 16 + k * 64 + l * 256];

                    fwd_ntt_ref_harvey(s0, 4, t->q, w_powers_n0, w_powers_con_n0, 2);

                    temp1[ 0 + i + j * 16 + k * 64 + l * 256] = s0[0];
                    temp1[ 4 + i + j * 16 + k * 64 + l * 256] = s0[1];
                    temp1[ 8 + i + j * 16 + k * 64 + l * 256] = s0[2];
                    temp1[12 + i + j * 16 + k * 64 + l * 256] = s0[3];

                }
            }
        }
    }

    //// tw multiplication
    for (int l = 0; l < 4; l++){          ////// 256 block
        for (int k = 0; k < 4; k++){      ////// 64 block
            for (int j = 0; j < 4; j++){  ////// 16 block
                for (int i = 0; i < 16; i++){
                    temp2[i + j*16 + k * 64 + l * 256] = (temp1[i + j*16 + k * 64 + l * 256] * tw3[i] ) % p;
                   // printf("%llu ", temp2[i]);
                }
            }
        }
    }


    /////////////// final stage4 256 times 4 elem side-by-side /////////////////

   for (int i = 0; i < 256 ; i++ ) {

       s0[0] = temp2[ 0 + 4 * i];
       s0[1] = temp2[ 1 + 4 * i];
       s0[2] = temp2[ 2 + 4 * i];
       s0[3] = temp2[ 3 + 4 * i];

       fwd_ntt_ref_harvey(s0, 4, t->q, w_powers_n0, w_powers_con_n0, 2);

       temp1[ 0 + i ]  =   s0[0];
       temp1[ 256 + i ] = s0[1];
       temp1[ 512 + i ] = s0[2];
       temp1[ 768 + i ] = s0[3];
   }

    printf("Printing calculation\n");
   for (int i = 0; i < 1024; i++){

       printf("%llu ", temp1[i]);
   }

    return 0;
}
