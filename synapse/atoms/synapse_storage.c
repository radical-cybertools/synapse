
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define CHUNKSIZE  (1024 * 1024)  /* 2^20 */

int main (int argc, char** argv)
{
    if ( argc < 3 )
    {
        return -1;
    }

    /* storage target */
    char*  tgt  =        argv[1];
    off_t  n    = atol  (argv[2]) * CHUNKSIZE;
    int    fd   = open  (tgt, O_CREAT | O_TRUNC | O_WRONLY, S_IRWXU);

    if ( fd < 0 )
    {
        perror ("open failed");
        return -2;
    }

    /* clear disk cache */
  //(void) syncfs (fd); 
    (void) sync ();

    off_t tot = 0;
    char* buf = malloc (CHUNKSIZE);

    while ( tot < (n+1) )
    {
        size_t wret = write (fd, buf, CHUNKSIZE);
  
        if ( wret != CHUNKSIZE )
        {
            perror ("write failed");
            return -4;
        }
  
        tot += CHUNKSIZE;
    }
  
    /* clear disk cache */
  //(void) syncfs (fd); 
    (void) sync ();

    free  (buf);
    close (fd);
  
    (void) unlink (tgt);

    return (0);
}
