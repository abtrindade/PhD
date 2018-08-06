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
#define precisionLN 0.000000001

//code here from https://stackoverflow.com/questions/3343395/value-of-natural-logarithm
double logESBMC (double x){
	double logx, tty, ty;
	int i;
	i=1; 
	logx = 0 ;
	ty = (x-1)/(x+1) ;
	do{
	   	logx = logx + ty / i;
    		tty = ty ;
    		ty = (ty * ((x-1)/(x+1)) * ((x-1)/(x+1)));
    		i = i + 2 ;
	} while((tty - ty) > precisionLN);
	logx = 2*logx ;
	return logx;

}

//main code just for test
int main(void) {
	double cont;
	for (cont=0; cont <= 10; cont=cont+0.1){ //test positive numbers
    		printf("num=%lf, sum=%lf, log=%lf\n",cont, logESBMC(cont),log(cont));
		printf ("----\n");
	}
    	return 0;
}


