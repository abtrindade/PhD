// 
// Function that calculates the natural logarithmic of base e for a number x - Alessandro Trindade July 2018
// 
// log(m) base e = 2[ (m-1/m+1) + (1/3) * (m-1/m+1)^3 + (1/5) * (m-1/m+1)^5 + ... ]
//
// To get an approximation of log (all depends on the precision demanded by the code)
// OBS: The natural logarithm function ln(x) is defined only for x>0. So the natural logarithm of a negative number is undefined.

// Function logESBMC of natural base e
#include <stdio.h>
#include <math.h>

//IMPORTANT: to declare precision here
#define precisionLN 0.00000001


double logESBMC (double x){

double tty;
double ty = (x-1)/(x+1);
double p = ty * ty;
int i = 1;
double logx = 0;
do {
  logx += ty / i;
  tty = ty;
  ty *= p;
  i += 2;
} while ((tty/(i-2) - ty/i) > 1e-6);


	logx = 2*logx ;
//	printf("\n ln (%g) = %g \n", x, logx);
	return logx;

}

//main code just for test
int main(void) {
	double cont;
	for (cont=0; cont <= 9; cont=cont+0.1){ //test positive numbers
    		printf("num=%lf, sum=%lf, log=%lf\n",cont, logESBMC(cont),log(cont));
		printf ("----\n");
	}
    	return 0;
}






