#include <iostream>
#include <mpi.h>
#include <vector>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, nprocs;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    const int matrixSize = 25;
    std::vector<std::vector<double>> matrix(matrixSize, std::vector<double>(matrixSize));

    // Inicializar la matriz en el proceso raíz (rank 0)

    // ...

    // Scatter la matriz a todos los procesos
    std::vector<double> localData(matrixSize);

    MPI_Scatter(matrix.data(), matrixSize, MPI_DOUBLE,
                localData.data(), matrixSize, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    // Operaciones específicas para cada proceso con la porción de la matriz

    MPI_Finalize();
    return 0;
}
