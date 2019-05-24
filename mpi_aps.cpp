#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>

#define MASTER 0               /* ID da primeira tarefa*/
#define FROM_MASTER 1          /* Tipo de mensagem 1 */
#define FROM_WORKER 2          /* Tipo de mensagem 2 */

using namespace std::chrono;

//Função Para Prencher matriz com valores "aleatorios"
void generateMatrix(int *matrix, int n){
   for (int i = 0; i < n; i++)
         matrix[i] = rand() % 9 + 1;
}

//Função Para Printar a Matriz
void displayMatrix(int *matrix, int n) 
{ 
   std::cout << "Matrix:" << std::endl;
   for (int i = 0; i < n; i++){  
      std::cout << matrix[i] << " "; 
      
      if((i + 1) % (int)sqrt(n) == 0){
         std::cout << std::endl;
      }
   }
   printf("\n");
} 

// Função de calculo do cofactor
void getCofactor(int *matrix, int *temp, int p, int q, int n) 
{ 
   int i = 0, j = 0; 

   for (int row = 0; row < n; row++) 
   { 
      for (int col = 0; col < n; col++) 
      { 
         if (row != p && col != q) 
         {  
            temp[i* (n - 1) + j++] = matrix[row*n + col]; 

            if (j == n - 1) 
            { 
               j = 0; 
               i++; 
            } 
         } 
      } 
   } 
} 


//Função para calculo da determinante
int determinantOfMatrix(int *matrix, int n, int init, int end, int sig) 
{ 
   int D = 0; 
   
   if (n == 1) 
      return matrix[0]; 

   int temp[n * n];

   int sign = sig; 
   
   for (int f = init; f < end; f++) 
   { 
      getCofactor(matrix, temp, 0, f, n); 
      D += sign * matrix[f] * determinantOfMatrix(temp, n - 1, 0, n - 1, 1); 
      sign = -sign; 
   
   } 
   return D; 
} 
//Main
int main (int argc, char *argv[]) {
   int numtasks,               /* numero de tarefas na particao */
   	taskid,                  /* identificador da tarefa */
   	numworkers,              /* numero de processos escravos */
      rc;
   
   int n = atoi(argv[1]);
   int matrix[n*n];
   int det;
   int determinante = 0;

   //inicialização di MPI
   MPI_Init(&argc,&argv);
   MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
   MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
   
   //Verifica se há mais de duas tasks
   if (numtasks < 2 ) {
     printf("Necessario ao minimo dois processos MPI. Encerrando.\n");
     MPI_Abort(MPI_COMM_WORLD, rc);
     exit(1);
   }
   
   numworkers = numtasks - 1;
   int res[numworkers];
   
   /******MASTER******/
   if (taskid == MASTER) {
      printf("mpi_mm iniciou com %d processos.\n", numtasks);
      
      //Inicialização da Matriz
      generateMatrix(matrix, n*n);
      displayMatrix(matrix, n*n);

      //Inicialização contador de tempo
      auto start = high_resolution_clock::now(); 

      //Envio da matriz para os workers
      for(int i = 1; i < numtasks; i++)
         MPI_Send(matrix, n*n, MPI_INTEGER, i, 0, MPI_COMM_WORLD);

      //Recepção da determinante parcial
      for(int i = 1; i < numtasks; i++){
         MPI_Recv(&res[i - 1], 1, MPI_INTEGER, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         }

      //Soma das determinantes parciais     **for caso posteriormente haja IMPI, calculo poderia estar no for de cima
      for(int i = 0; i < numworkers; i++)
            determinante += res[i];

      //Resultado da Determinante
      std::cout << "Determinante: " << determinante << std::endl;
      
      //Tempo Total de execução
      auto end_time = duration_cast<duration<double>>(high_resolution_clock::now() - start).count();
      std::cout << "Tempo de Execução: " << end_time << " segundos" << std::endl ;
      

      return 0;
   }

   /******WORKERS******/
   else if(taskid > MASTER){
      
      //Recepção da Matriz
      MPI_Recv(matrix, n*n, MPI_INTEGER, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      
      //Faixa de operação do worker
      int init = (n/numworkers * (taskid - 1) );
      int end  = (n/numworkers * taskid);
      int sig;

      //sinal de inicialização da determinante
      if(init % 2 == 0 ) sig = 1; else sig = -1;

      if(taskid == numworkers) end = n;

      //Calculo da determinante parcial
      det = determinantOfMatrix(matrix, n, init, end, sig);
      
      //Envio da determinante parcial para o master
      MPI_Send(&det, 1, MPI_INTEGER, 0, 0, MPI_COMM_WORLD);
   }

   //Finalização do MPI
   MPI_Finalize();
}