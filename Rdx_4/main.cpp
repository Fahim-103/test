#include <iostream>
using namespace std;

int main() {

    int e[4] = {7497,	2371,	2885,	5195};
    int wtt = 5756;
    int wq  = 7681;

    int a = e[0];
    int b = e[1];
    int c = e[2];
    int d = e[3];


    int t0 = (  (a + c)  +      (b + d)  +wq ) % wq ;
    int t1 = (  (a - c)  + (wtt*(b - d)) +wtt*wq ) % wq ;
    int t2 = (  (a + c)  -      (b + d)  +wq ) % wq ;
    int t3 = (  (a - c)  - (wtt*( b - d)) +wtt*wq ) % wq ;

    /* int t0 = (  (a + c)  +      (b + d)  )  ;
     //t0 -= (int) (t0/wq);
     int t1 = (  (a - c)  + (wtt*((b - d) +wq ) +wq) )  ;
     int t2 = (  (a + c)  -      (b + d) + wq  )  ;
     int t3 = (  (a - c)  - (wtt*(( b - d)) + wq ) );*/

    ///////////////////modulo operation///////////////
    int p = 323;
    int q = 3;

    int r = (int) p/q;

    int v = p - r*q + q;

    int w = v - (int)(v/q)*q;

    printf("%d %d \n", v, w);

    //////////////////////////////////////////////

    printf ("%d %d %d %d\n", t0, t1, t2, t3);



    return 0;
}
