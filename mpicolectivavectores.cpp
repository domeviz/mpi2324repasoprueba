#include <iostream>
#include <mpi.h>
#include <vector>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, nprocs;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    const int vectorSize = 100;
    std::vector<int> globalData(vectorSize);

    if (rank == 0) {
        // Proceso raíz inicializa el vector
        for (int i = 0; i < vectorSize; ++i) {
            globalData[i] = i;
        }
    }

    // Distribuir el vector desde el proceso raíz a todos los procesos
    std::vector<int> localData(vectorSize / nprocs);
    MPI_Scatter(globalData.data(), vectorSize / nprocs, MPI_INT,
                localData.data(), vectorSize / nprocs, MPI_INT,
                0, MPI_COMM_WORLD);

    // Operación específica para cada proceso con la porción del vector
    for (int i = 0; i < vectorSize / nprocs; ++i) {
        localData[i] *= 2;  // Ejemplo de operación: duplicar cada elemento
    }

    // Recopilar los datos modificados en el proceso raíz
    MPI_Gather(localData.data(), vectorSize / nprocs, MPI_INT,
               globalData.data(), vectorSize / nprocs, MPI_INT,
               0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Proceso raíz imprime el vector modificado
        std::cout << "Vector modificado:\n";
        for (int i = 0; i < vectorSize; ++i) {
            std::cout << globalData[i] << " ";
        }
        std::cout << std::endl;
    }

    MPI_Finalize();
    return 0;
}
