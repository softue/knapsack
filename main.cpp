/*
 
    Algoritmo Genético para resolver o problema da mochila (knapsack problem)
 
    Copyright 2012 Rômulo Mendes Figueiredo <romulomendesfigueiredo@gmail.com>
    Created on 03/15/2012 16:28
  
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <cstdlib>
#include <iostream>
#include <time.h> // srand
#include <stdio.h>

using namespace std;


FILE *fp;
    
// Configuração do AG
int tam_populacao;
int num_obj = 13;
int cromossomo[1000][13];
int novo_cromossomo[1000][13];
double fitness[1000];
int idx_melhor;
double melhor;
int geracao;


// Restrição
int capacidade = 60;

// Características dos objetos
int peso[13] =  {10, 20, 30, 40, 10, 15, 2, 3, 20, 21, 43, 1,  100};
int valor[13] = { 6,  7,  9,  5, 10,  7, 4, 3,  6,  7, 99, 9, 2000};


double normalizar(int min) {
    
    //int min = 0; // minimum value of the fitness
    int total = 0; // sum of all fitness
    double acumulado = 0;
    double media = 0;
    
    // menor valor passa a ser 0
    for (int i=0; i<tam_populacao; i++) {
        fitness[i] = fitness[i] - min;
        total = total + fitness[i];
    }
    
    media = total/tam_populacao;
    
    // calcula a propabilidade proporcional ao fitness e acumula o valor
    for (int i=0; i<tam_populacao; i++) {
        //if (i!=1) acumulado = acumulado + fitness[i-1];
        fitness[i] = ((fitness[i]/total) + acumulado);
        acumulado = fitness[i];
        //debug cout << "fitness acumulado " << i << ": " << fitness[i] << ". acu: " << acumulado << "\n";
    }
    
    return media;
    
}

void substitui_populacao() {
    
    //// substitui a população antiga
    for (int i=0; i<tam_populacao; i++) {
        if (i!=idx_melhor) // elitismo
            for (int no=0; no<num_obj; no++) 
                cromossomo[i][no] = novo_cromossomo[i][no];
    }    
    
}

void crossover(int pai, int mae) {
    
    int cp = rand() % num_obj;
    int tmp;
    
    // troca de material genetico ate o crossover point (cp)
    for (int i=0; i<cp; i++) {
        tmp = cromossomo[pai][i];
        novo_cromossomo[pai][i] = novo_cromossomo[mae][i];
        novo_cromossomo[mae][i] = tmp;
    }
    
}


void selecao() {
    
    double sorteia;
    
    // sorteia n individuos para proxima geracao
    for (int n=0; n<tam_populacao; n++) {
        sorteia = rand() % 101; // sorteia entre 0-100;
        sorteia = sorteia/100;
        //debug cout << sorteia << ": ";
        
        // faz uma busca no valor acumulado
        for (int i=0; i<tam_populacao; i++) {
            if (fitness[i]>=sorteia) {
                // pega todas as respostas do cromossomo i
                for (int no=0; no<num_obj; no++) {
                    novo_cromossomo[n][no] = cromossomo[i][no];
                    //debug cout << novo_cromossomo[n][no];
                }
                //debug cout << "\n";
                break;
            }
            if (i%2!=0) {
                crossover(i, i-1);
            }
        }
    }
}

 int avaliar() {
    
    int soma_peso;
    int best = 0;
    int menor;
    float media=0;
    float soma=0;

    for (int i=0; i<tam_populacao; i++) {
       
        fitness[i] = 0;
        soma_peso = 0;
        for (int g=0; g<num_obj; g++) {
            // fitness = soma de todos os itens na sacola
            fitness[i] = fitness[i] + valor[g] * cromossomo[i][g];
            // calcula o peso total
            soma_peso =  soma_peso + peso[g] * cromossomo[i][g];
            //peso[0]*cromossomo[i][0] +  peso[1]*cromossomo[i][1] +  peso[2]*cromossomo[i][2] +  peso[3]*cromossomo[i][3] +   peso[4]*cromossomo[i][4];
            //valor[0]*cromossomo[i][0] + valor[1]*cromossomo[i][1] + valor[2]*cromossomo[i][2] + valor[3]*cromossomo[i][3] + valor[4]*cromossomo[i][4];   
        }
        // se ultrapassar a capacidade da mochila, aplica penalidade
        if (soma_peso>capacidade) fitness[i] = -soma_peso;
        // acumula o melhor resultado de fitness
        if (i==0) {
            best=fitness[0]; 
            idx_melhor = 0;
            menor = fitness[0];
        } else if (fitness[i]>best) {
            best=fitness[i];
            idx_melhor = i;
        }
        soma = soma + fitness[i];
        // guarda menor valor para normalizar posteriormente
        if (fitness[i]<menor) menor = fitness[i];
        //debug cout << fitness[i] <<"\n";
    }
    media = soma/tam_populacao;
  
    //fputs(str,fp); //Grave a string, caracter a caracter
    melhor = best;
    // o que fazer se o melhor for negativo? antes de normalizar
    normalizar(menor);
    
    //string str;
    fprintf(fp, "%d\t%d\t%f\n", geracao, best, media);
    cout << "Best: " << best;
    cout << ". Média: " << media;
    cout << ". Menor: " << menor << "\n";
   
}

void iniciar_populacao(int tp) {
    
    tam_populacao = tp;
    
    for (int i=0; i<tam_populacao; i++) {
        for (int g=0; g<num_obj; g++) {
            cromossomo[i][g] = rand() % 2;
            //debug cout <<cromossomo[i][g];
        }
        //debug cout << "\n";
    }
}

void mutacao(double taxa) {
    
    int muda;
    
    for (int i=0; i<tam_populacao; i++) {
        if (i!=idx_melhor) // nao faz mutacao no melhor dos melhores (elitismo)
        if (rand() % 101<=taxa) {
            muda = rand() % num_obj;
            if (cromossomo[i][muda]==0) cromossomo[i][muda]=1; else cromossomo[i][muda]=0;
            //cout << "Mutacao: " << i << "=" << muda;
        }
    }
    
}


int main(int argc, char** argv) {
    
    // se algum item individual ultrapassa o valor da mochila, usar schema
    
    srand ( time(NULL) ); // initialize random seed
    
    fp=fopen("dados2.dat","w");    
    
    iniciar_populacao(50);
    avaliar();
    
    for (geracao=1; geracao<=1000; geracao++) {
        cout << "Geração: " << geracao << ": ";
        selecao(); // na seleção já é feito o crossover
        substitui_populacao();
        mutacao(0.4);
        avaliar();
        //if (melhor==125) break;
    }
    cout << "Melhor resultado em " << geracao << " gerações.";
    for (int g=0; g<num_obj; g++) cout << cromossomo[idx_melhor][g];
    cout << "\n";
    
    
    fclose(fp);

    execlp("gnuplot","gnuplot","plot",NULL);    
    
    return 0;
}
