#include <iostream>
#include <mpi.h>
#include <vector>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, nprocs;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    const int vectorSize = 100;
    std::vector<int> data(vectorSize);

    if (rank == 0) {
        // Proceso raíz inicializa y envía datos a otros procesos
        for (int i = 0; i < vectorSize; ++i) {
            data[i] = i;
        }

        for (int destRank = 1; destRank < nprocs; ++destRank) {
            MPI_Send(data.data(), vectorSize, MPI_INT, destRank, 0, MPI_COMM_WORLD);
        }
    } else {
        // Procesos no raíces reciben datos
        MPI_Recv(data.data(), vectorSize, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Operaciones específicas para cada proceso con el vector

    MPI_Finalize();
    return 0;
}
