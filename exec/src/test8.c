#include <math.h>

int main (int argc, char *argv[]){
int a, b, c, d;
float f1, f2, f3, f4;

//Int Operations one of each
a = 1; b = 2;
c = a + b; d = a * b;
a = d/b; b = d - a;
if (a == b)  c % a;
else a = b;

//FLOPS one of each
f1 = 1.5; f2 = 2.5;
f3 = f1 + f2; f4 = f1 * f2;
f1 = f4/f2; f2 = f4 - f1;
if (f1 == f2)  f1 =0;  // fmod(f3, f1);

return 0;
}
