//#include <math.h>

#define MAXn 10 //máximo número de nodes
#define MAXe 13 //máximo número de edges (arestas)

_Bool nondet_bool();
int nondet_int();

int tmp[MAXe];

int main() {
  int i, j, k, aux=0, softcost=0, comcost=0;
  _Bool x[MAXn]; 
  
int s[MAXn] = {10, 15, 7, 4, 9, 10, 15, 7, 4, 9};  //
int h[MAXn] = {5, 10, 3, 4, 8, 5, 10, 3, 4, 8}; //custo de hardware para cada nó
int c[MAXe] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5}; //custo de comunicação de cada aresta

int E[MAXe][MAXn] = {{-1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
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


 
  for(i=0; i<MAXn; i++) {
    x[i] = nondet_bool();
  }

  //s(1-x)
  for(i=0; i<MAXn; i++) {
    softcost += s[i]*(1-x[i]);
  }

  //Ex
  aux = 0;
  for(i=0; i<MAXe; i++) {
    for(j=0; j<MAXn; j++) {
      aux += E[i][j]*x[j];
    }
    tmp[i] = aux;
    aux = 0;
  }

  //c*|EX|
  for(i=0; i<MAXe; i++) {
    comcost += c[i]*__ESBMC_abs(tmp[i]);
  }

  //aqui é o valor de S0 = 0, 23, 45, 67, 90
  __ESBMC_assume((softcost+comcost) <= 23);

  //função objetivo
  aux=0;
  for(i=0; i<MAXn; i++) {
    aux += x[i]*h[i];
  }
  
  assert(aux > 0);

  return 0;
}

/*
Solução ótima		
S0	hw	sw
0	60	0
23	52	14
45	38	43
67	30	55
90	0	90
*/
