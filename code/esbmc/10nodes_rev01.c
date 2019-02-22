//#include <math.h>

//#define MAXn 10 //máximo número de nodes
//#define MAXe 13 //máximo número de edges (arestas)

_Bool nondet_bool();

int nondet_int();
int tmp[13];


//universal to automated verification, passing DEFINES to global and declaring VERIFIER_error
extern void __VERIFIER_error() __attribute__ ((__noreturn__));




int main() {
  int tiph = nondet_int();
  int i, j, aux=0, softcost=0, comcost=0, fobj, cont, hmax=0;

  _Bool x[10]; 
  
  int s[10] = {10, 15, 7, 4, 9, 10, 15, 7, 4, 9};  //
  int h[10] = {5, 10, 3, 4, 8, 5, 10, 3, 4, 8}; //custo de hardware para cada nó
  int c[13] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5}; //custo de comunicação de cada aresta

  int E[13][10] = {{-1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
                     {-1, 0, 1, 0, 0, 0, 0, 0, 0, 0},
				     {-1, 0, 0, 0, 0, 1, 0, 0, 0, 0},
				     {0, -1, 0, 0, 1, 0, 0, 0, 0, 0},
				     {0, 0, -1, 0, 1, 0, 0, 0, 0, 0},
				     {0, 0, -1, 1, 0, 0, 0, 0, 0, 0},
				     {0, 0, 0, -1, 1, 0, 0, 0, 0, 0},
				     {0, 0, 0, 0, 0, -1, 1, 0, 0, 0},
				     {0, 0, 0, 0, 0, -1, 0, 1, 0, 0},
				     {0, 0, 0, 0, -1, 0, 0, 0, 1, 0},
				     {0, 0, 0, 0, 0, 0, -1, 0, 1, 0},
				     {0, 0, 0, 0, 0, 0, 0, -1, 1, 0},
				     {0, 0, 0, 0, 0, 0, 0, 0, -1, 1}};  //matriz incidente do Grafo (nó, arestas)


  for (cont=0; cont < 10; cont++) hmax=hmax + h[cont]; // acha o valor do particionamento com tudo em hw
  __VERIFIER_assume (tiph>= 0 && tiph<= hmax);  //hmax eh tudo em HW  

  for(i=0; i<10; i++) {
  		int ndet = nondet_bool();
		__VERIFIER_assume((ndet == 0) || (ndet == 1));
		x[i] = ndet;
  }

  //s(1-x)
  for(i=0; i<10; i++) {
    softcost += s[i]*(1-x[i]);
  }

  //Ex
  aux = 0;
  for(i=0; i<13; i++) {
    for(j=0; j<10; j++) {
      aux += E[i][j]*x[j];
    }
    tmp[i] = aux;
    aux = 0;
  }

  //c*|EX|
  for(i=0; i<13; i++) {
    comcost += c[i]*__ESBMC_abs(tmp[i]);
  }

  __VERIFIER_assume((softcost+comcost) <= 90);

  //função objetivo
  fobj=0;
  for(i=0; i<10; i++) {
    fobj += x[i]*h[i];
  }

  if (!(fobj < tiph)) { __VERIFIER_error(); }

  return 0;


}

//assume (exp);  MUST BE REPLACE BY:   if (!expr) { __VERIFIER_error(); }


/*
Solução ótima		
S0	hw	sw
0	60	0
23	52	14
45	38	43
67	30	55
90	0	90
*/
