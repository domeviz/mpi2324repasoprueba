#include <iostream>
#include <mpi.h>
#include <vector>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, nprocs;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    const int matrixSize = 3;
    std::vector<std::vector<int>> localMatrix(matrixSize, std::vector<int>(matrixSize, 0));

    if (rank == 0) {
        // Proceso raíz inicializa la matriz
        for (int i = 0; i < matrixSize; ++i) {
            for (int j = 0; j < matrixSize; ++j) {
                localMatrix[i][j] = i * matrixSize + j;
            }
        }
    }

    // Transmitir la matriz desde el proceso raíz a todos los demás procesos
    MPI_Bcast(localMatrix.data()[0].data(), matrixSize * matrixSize, MPI_INT, 0, MPI_COMM_WORLD);

    // Cada proceso imprime la matriz recibida
    std::cout << "Proceso " << rank << " tiene la matriz:\n";
    for (int i = 0; i < matrixSize; ++i) {
        for (int j = 0; j < matrixSize; ++j) {
            std::cout << localMatrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    MPI_Finalize();
    return 0;
}
