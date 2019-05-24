#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>

using namespace std::chrono;

void generateMatrix(int *matrix, int n){
   for (int i = 0; i < n; i++)
         matrix[i] = rand() % 9 + 1;
}


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


int determinantOfMatrix(int *matrix, int n) 
{ 
   int D = 0; 
   
   if (n == 1) 
      return matrix[0]; 

   int temp[n * n];

   int sign = 1; 
   
   for (int f = 0; f < n; f++) 
   { 
      getCofactor(matrix, temp, 0, f, n); 
      D += sign * matrix[f] * determinantOfMatrix(temp, n - 1); 
      sign = -sign; 
   
   } 
   return D; 
} 


int main (int argc, char *argv[]) {
   int numtasks,               /* numero de tarefas na particao */
   	taskid,                  /* identificador da tarefa */
   	numworkers,              /* numero de processos escravos */
      rc;
   
   int n = atoi(argv[1]);
 
   int matrix[n*n];
   int det;
   int determinante = 0;
 
    generateMatrix(matrix, n*n);
    displayMatrix(matrix, n*n);
    auto start = high_resolution_clock::now(); 

    determinante = determinantOfMatrix(matrix, n);
    
    std::cout << "Determinante: " << determinante << std::endl;
      
    auto end_time = duration_cast<duration<double>>(high_resolution_clock::now() - start).count();
    std::cout << "Tempo de Execução: " << end_time << " segundos" << std::endl ;
}
      