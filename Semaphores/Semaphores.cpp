#include <iostream>
#include <windows.h>
#include <mpi.h>
#include<fstream>

using namespace std;

int main(int argc, char** argv) {

	MPI_Init(NULL, NULL);

	int world_rank, world_size;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	ofstream file;
	int toggle = 0;
	int *semaphores = NULL;
	int semaphore[2];

	if (world_rank == 0) {
		semaphores = (int *)malloc(world_rank * 2 * sizeof(int));
		for (int i = 0; i < world_rank; i++)
			semaphores[i] = 2 * i + 1;

		printf("Processor %d has data: ", world_rank);
		for (int i = 0; i < world_rank; i++)
			printf("%d ", semaphores[i]);
		printf("\n");
	}

	while (true)
	{

		if (world_rank == 0) {
			toggle = (toggle + 1) % 4;
		}

		// Broadcast all tasks
		MPI_Bcast(&toggle, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Scatter(semaphores, 2, MPI_INT, &semaphore, 2, MPI_INT, 0, MPI_COMM_WORLD);

		if (toggle < 2)
		{
			if (world_rank % 2 == 0)
			{
				semaphore[0] = toggle % 2;
				semaphore[1] = !semaphore[0];
			}
			else
			{
				semaphore[0] = 0;
				semaphore[1] = 0;
			}
		}
		else
		{
			if (world_rank % 2 == 1)
			{
				semaphore[0] = toggle % 2;
				semaphore[1] = !semaphore[0];
			}
			else
			{
				semaphore[0] = 0;
				semaphore[1] = 0;
			}
		}

		MPI_Gather(&semaphore, 2, MPI_INT, semaphores, 2, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);
		// Wait all tasks


		if (world_rank == 0)
		{
			file.open("semaphores.txt");

			for (size_t i = 0; i < 8; i++)
			{
				file << semaphores[i];
				cout << semaphores[i];
			}

			file.close();
			cout << endl;

			if (toggle == 0 || toggle == 2)
			{
				Sleep(2000);
			}
			else if (toggle == 1 || toggle == 3)
			{
				Sleep(8000);
			}
		}
	}

	MPI_Finalize();
}