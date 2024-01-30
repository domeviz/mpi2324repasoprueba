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

        // Operación específica para procesos no raíces con el vector recibido
        for (int i = 0; i < vectorSize; ++i) {
            data[i] *= 2;  // Ejemplo de operación: duplicar cada elemento
        }

        // Enviar el vector modificado al proceso raíz
        MPI_Send(data.data(), vectorSize, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        // Proceso raíz recibe los vectores modificados de los procesos no raíces
        for (int srcRank = 1; srcRank < nprocs; ++srcRank) {
            MPI_Recv(data.data(), vectorSize, MPI_INT, srcRank, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // Procesar o imprimir los vectores recibidos
            std::cout << "Vector recibido del proceso " << srcRank << ": ";
            for (int i = 0; i < vectorSize; ++i) {
                std::cout << data[i] << " ";
            }
            std::cout << std::endl;
        }
    }

    MPI_Finalize();
    return 0;
}
