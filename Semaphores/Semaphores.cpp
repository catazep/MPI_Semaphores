#include <iostream>
#include <windows.h>
#include <mpi.h>

using namespace std;

int main(int argc, char** argv) {

	MPI_Init(NULL, NULL);

	int world_rank, world_size;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	int semaphore, toggle = 0, leftSemaphore = 0;

	while (true)
	{

		if (world_rank == 0) {
			toggle = (toggle + 1) % 4;
		}

		// Broadcast all tasks
		MPI_Bcast(&toggle, 1, MPI_INT, 0, MPI_COMM_WORLD);
		if (toggle < 2)
		{
			if (world_rank % 2 == 0)
			{
				semaphore = toggle % 2;
				leftSemaphore = !semaphore;
			}
			else
			{
				semaphore = 0;
				leftSemaphore = 0;
			}
		}
		else
		{
			if (world_rank % 2 == 1)
			{
				semaphore = toggle % 2;
				leftSemaphore = !semaphore;
			}
			else
			{
				semaphore = 0;
				leftSemaphore = 0;
			}
		}
		MPI_Barrier(MPI_COMM_WORLD);
		// Wait all tasks
		cout << "Process " << world_rank << " is " << semaphore << " - " << leftSemaphore << endl;

		if (world_rank == 0)
		{
			cout << endl;
			if (toggle == 0 || toggle == 2)
			{
				Sleep(4500);
			}
			else if (toggle == 1 || toggle == 3)
			{
				Sleep(1500);
			}
		}
	}

	MPI_Finalize();
}