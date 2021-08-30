    #include <stdio.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <fcntl.h>   
    #include <sys/wait.h>

int main(int argc, char const *argv[])
{
    int id, id2, status ;
    pid_t pid = getpid();
    printf ( "My PID (Parent): %d\n", pid);
    
    id = fork() ;
    if ( id == 0 )
    {
        //curl --range 0-19999999 -o part1 http://10.8.19.24/ProxyData/access.log-20161108.gz
        // curl --range 20000000 -o part1 http://10.8.19.24/ProxyData/access.log-20161108.gz
        
        char* ar[]={"curl","--range","0-19999999","-o", "part1","http://10.8.19.24/ProxyData/access.log-20161108.gz",NULL};
        execvp("curl",ar);
        printf("Downloading in Child process\n");
        sleep(10);
        printf ( "Child exits... download complete");
    } else {
        id2 = fork() ;
        if ( id2 == 0 )
        {
        char* ar[]={"curl","--range","20000000-","-o", "part2","http://10.8.19.24/ProxyData/access.log-20161108.gz",NULL};
        execvp("curl",ar);
        }
        else {
        printf("Parent waiting\n");
        sleep(20);
        waitpid(id,&status,1);
        printf ( "Child with id %d exits: status=%d",id,status);
 
        waitpid(id2,&status,1);

        int fd = open("part3.gz", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        dup2(fd, 1);   // make stdout go to file
        close(fd);

        printf ( "Child with id %d exits: status=%d",id2,status);
        char* ar[]={"/bin/cat","part1","part2",NULL};
        execvp("/bin/cat",ar);
        
        printf ( "Parent exits");
        }
    }
    
    return 0;
}