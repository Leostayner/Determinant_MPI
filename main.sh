#!/bin/bash
echo "Ordem da Matrix Quadrada:" 
read var_ord

echo "Numero de Processos MPI:"
read var_pro

mpirun -n $var_pro -hostfile hf mpi_aps $var_ord