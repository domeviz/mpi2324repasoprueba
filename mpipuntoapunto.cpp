#include <iostream>
#include <vector>
#include <mpi.h>
#include <cmath>

//Metodos adicionales (Calculo, lectura, etc...)
//vector para cantidad de datos
int sumar (int *tmp, int n) {
    //ejemplo
    int suma=0;
    for(int i=0;i<n;i++){
        suma=suma+tmp[i];
    }
    return suma;
}


int main (int argc, char** argv){

    MPI_Init(&argc, &argv);
    int rank, nprocs;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    //std::printf("Rank %d of %d procs\n",rank,nprocs);
    
    //Valores generales para todos los rank
    int xd [100];
    /* * */

    if (rank==0)
    {
        //Inicializar
        /*     */
        //leer datos de un archivo externo
        //inicializar un vector solo en el 0
        for(int i=0;i<100;i++){
            xd[i]=i;
        }

        //ENVIAR
        for (int id_rank = 1; id_rank < nprocs; id_rank++){
            int start=id_rank*25;
            // MPI_Send( & , , MPI_, id_rank , 0 , MPI_COMM_WORLD);
            MPI_Send( &xd[start] ,25 , MPI_INT, id_rank , 0 , MPI_COMM_WORLD);
        }

        //Procesar
        int promedio[4];
        int suma=sumar(xd,25);
        promedio[0]=suma;

        //RECIBIR
        for (int id_rank = 1; id_rank < nprocs; id_rank++){
            // MPI_Recv( & ,  , MPI_ , id_rank , 0 , MPI_COMM_WORLD , MPI_STATUS_IGNORE);
            MPI_Recv( &promedio[id_rank], 1 , MPI_INT , id_rank , 0 , MPI_COMM_WORLD , MPI_STATUS_IGNORE);
        }

        //Agrupar
        /*     */
        int resultado=sumar(promedio,4);
        std::printf("SUMA TOTAL: %d\n", resultado);
    }
    else
    {
        //GENERALIZACION DE TODOS LOS RANKS
        //RECIBIR
        // MPI_Recv(  ,  , MPI_ , 0 , 0 , MPI_COMM_WORLD , MPI_STATUS_IGNORE);
        MPI_Recv( xd, 25 , MPI_INT , 0 , 0 ,MPI_COMM_WORLD , MPI_STATUS_IGNORE);
        //Procesar
        int promedio=0;
        promedio=sumar(xd,25);
        
        //ENVIAR
        // MPI_Send( & ,  , MPI_ , 0 , 0 , MPI_COMM_WORLD);
        MPI_Send( &promedio , 1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD);
    }
    

    MPI_Finalize();
    return 0;
}