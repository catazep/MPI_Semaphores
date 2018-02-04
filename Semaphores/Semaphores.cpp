#include <iostream>
#include <windows.h>
#include <mpi.h>

using namespace std;

int main(int argc, char** argv) {
	
	MPI_Init(NULL, NULL);

	int world_rank, world_size;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	int semaphore, toggle = 0, duration = 0;

	while (true)
	{

		if (world_rank == 0) {
			Sleep(2000);
			toggle = (toggle + 1) % 2;
		}

		// Broadcast all tasks
		MPI_Bcast(&toggle, 1, MPI_INT, 0, MPI_COMM_WORLD);
		semaphore = (world_rank + toggle) % 2;
		MPI_Barrier(MPI_COMM_WORLD);
		// Wait all tasks
		cout << "Process " << world_rank << " is " << semaphore << endl;

		if (world_rank == 0)
		{
			cout << endl;
		}
	}

	MPI_Finalize();
}