    #include <stdio.h>
    #include <sys/types.h>
    #include <unistd.h>

    int main(){
    int id ;
    id = fork() ;
    if ( id == 0 ) {
        printf ("\nCHILD::PID1: %d, PID2: %d", getpid(),getppid());
    }else {
        printf ( "\nPARENT::PID1: %d, PID2: %d", id, getpid()); 
    } 
    return 0;
}
/**
Parent :: Child’s PID: 10104, Parent’s PID: 10103
Child:: Child’s PID: 10104, Parent’s PID: 2442
**/
