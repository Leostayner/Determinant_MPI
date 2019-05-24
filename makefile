CXX    = mpic++
FLAGS  = 
PROGS  = mpi_aps

all : $(PROGS)

mpi_aps : mpi_aps.cpp 
	$(CXX) $(FLAGS) -o $@ $^ 

clean:
	rm -f $(PROGS)
	
exe : exe.o
	$(CXX) $(FLAGS) -o $@ $^ 


