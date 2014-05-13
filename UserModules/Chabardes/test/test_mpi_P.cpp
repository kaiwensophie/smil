#include <DProcessing.h>

using namespace smil;

int main (int argc, char* argv[]) {

    // Communication canal ...
    MPI_Comm intraP, inter_StoP, inter_PtoR;
    // Ranks ...
    int rank_inP;
    // World count ...
    int nbrP = 16;
    // Service name ...
    char service_StoP[] = "smil_mpi_StoP", service_PtoR[] = "smil_mpi_PtoR";
    // Port names ...
    char port_StoP[MPI_MAX_PORT_NAME] = {0}, port_PtoR[MPI_MAX_PORT_NAME] = {0};
    // MPI Implementation specific information on how to establish an address. 
    MPI_Info info = MPI_INFO_NULL;

    MPI_Init (&argc, &argv);

    MPI_Comm_size (MPI_COMM_WORLD, &nbrP);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank_inP);  


    if (rank_inP == 0) {
            
        // Not implemented on either mpich2 nor openmpi... using "mpiexec -host <ip_address> <bin>"
        /*
        MPI_Info_create (&info) ;
        MPI_Info_set (info, "ip_port", argv[1]);
        MPI_Info_set (info, "ip_address", argv[2]);
        */

        MPI_Open_port (info, port_StoP);
        cout << "Opened a sender port at : " << port_StoP << "." << endl;
        MPI_Publish_name (service_StoP, info, port_StoP);
        
        MPI_Open_port (info, port_PtoR);
        cout << "Opened a receiver port at : " << port_PtoR << "." << endl;
        MPI_Publish_name (service_PtoR, info, port_PtoR);


    }
    MPI_Barrier (MPI_COMM_WORLD);

    MPI_Bcast (port_StoP, MPI_MAX_PORT_NAME, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast (port_PtoR, MPI_MAX_PORT_NAME, MPI_CHAR, 0, MPI_COMM_WORLD);

    MPI_Comm_accept (port_StoP, info, 0, MPI_COMM_WORLD, &inter_StoP);
    MPI_Comm_accept (port_PtoR, info, 0, MPI_COMM_WORLD, &inter_PtoR);

    MPI_Barrier (MPI_COMM_WORLD) ;
    if (rank_inP == 0) {
        MPI_Close_port (port_StoP);
        MPI_Close_port (port_PtoR);
        MPI_Unpublish_name (service_StoP, info, port_StoP);
        MPI_Unpublish_name (service_PtoR, info, port_StoP);
    }

    cout << "process #" << rank_inP << " terminates..." << endl;
   
    MPI_Barrier (MPI_COMM_WORLD); 
    MPI_Comm_disconnect (&inter_StoP);
    MPI_Comm_disconnect (&inter_PtoR);
    MPI_Finalize ();

    return 0;
} 
