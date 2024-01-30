#include <iostream>
#include <mpi.h>
#include <vector>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, nprocs;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    const int matrixSize = 4;
    std::vector<std::vector<int>> matrix(matrixSize, std::vector<int>(matrixSize));

    if (rank == 0) {
        // Proceso raíz inicializa la matriz
        for (int i = 0; i < matrixSize; ++i) {
            for (int j = 0; j < matrixSize; ++j) {
                matrix[i][j] = i * matrixSize + j;
            }
        }
        
        // Enviar la matriz a los otros procesos
        for (int destRank = 1; destRank < nprocs; ++destRank) {
            MPI_Send(matrix.data()[0].data(), matrixSize * matrixSize, MPI_INT, destRank, 0, MPI_COMM_WORLD);
        }
    } else {
        // Procesos no raíces reciben la matriz
        MPI_Recv(matrix.data()[0].data(), matrixSize * matrixSize, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Operaciones específicas para cada proceso con la matriz

    MPI_Barrier(MPI_COMM_WORLD); // Sincronización de todos los procesos

    if (rank != 0) {
        // Procesos no raíces envían sus resultados al proceso raíz
        MPI_Send(matrix.data()[0].data(), matrixSize * matrixSize, MPI_INT, 0, 1, MPI_COMM_WORLD);
    } else {
        // Proceso raíz recibe resultados de los procesos no raíces
        for (int srcRank = 1; srcRank < nprocs; ++srcRank) {
            MPI_Recv(matrix.data()[0].data(), matrixSize * matrixSize, MPI_INT, srcRank, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            // Procesar o imprimir los resultados
            std::cout << "Matriz recibida del proceso " << srcRank << ":\n";
            for (int i = 0; i < matrixSize; ++i) {
                for (int j = 0; j < matrixSize; ++j) {
                    std::cout << matrix[i][j] << " ";
                }
                std::cout << std::endl;
            }
        }
    }

    MPI_Finalize();
    return 0;
}
