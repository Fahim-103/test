#include <iostream>
using namespace std;

uint32_t   mdl (uint64_t   t, uint32_t   w, uint32_t   q) {

    __uint128_t  w_con = (uint64_t ) 1 << 32;
    printf ("%llu \n", w_con );
    w_con = w_con/65537;   //// store this 255
    printf ("w_con/q: %u \n", w_con );


    //uint32_t  tt = w*t - q * ((w_con*t) >> 16);

    uint64_t  tt = t - q * ((65535 * t) >> 32);   //////////////////////////


    return tt;

}


int main() {

    uint32_t   e[4] = {4,3,2,1};
    uint32_t   wtt = 32;
    uint32_t   wq  = 65537;

    uint32_t   a = e[0];
    uint32_t   b = e[1];
    uint32_t   c = e[2];
    uint32_t   d = e[3];


    uint32_t   t0 = (  (a + c)  +      (b + d)  )  ;

    uint32_t   t1 = (  (a - c)  + (wtt*(b - d)) )  + wq*(wtt+1);

    uint32_t   tn = t1 % wq;
    uint32_t   tnn = mdl(t1, 1, wq);
    printf(" %d %d %d\n", t1, tn, tnn);

    uint32_t   t2 = (  (a + c)  -      (b + d)  ) + 2*wq ;

    uint32_t   tn2 = t2 % wq;
    uint32_t   tnn2 = mdl(t2, 1, wq);
    printf(" %d %d %d\n", t2, tn2, tnn2);


    uint32_t   t3 = (  (a - c)  - (wtt*( b - d))  ) + wq*(wtt+1);

    uint32_t   tn3 = t3 % wq;
    uint32_t   tnn3 = mdl(t3, 1, wq);
    printf(" %d %d %d\n", t3, tn3, tnn3);


    /* uint32_t   t0 = (  (a + c)  +      (b + d)  )  ;
     //t0 -= (int) (t0/wq);
     uint32_t   t1 = (  (a - c)  + (wtt*((b - d) +wq ) +wq) )  ;
     uint32_t   t2 = (  (a + c)  -      (b + d) + wq  )  ;
     uint32_t   t3 = (  (a - c)  - (wtt*(( b - d)) + wq ) );*/

    ///////////////////modulo operation///////////////


    //////////////////////////////////////////////

    printf ("%d %d %d %d\n", t0, t1, t2, t3);





    return 0;
}
