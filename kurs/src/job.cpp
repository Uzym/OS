//
// Created by lyutoev on 27.12.22.
//

#include "job.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
    FILE *pipe;
    pipe = popen("./test_job 5454", "r");
    char psBuffer[128];
    while(fgets(psBuffer, 128, pipe)) {
        puts(psBuffer);
    }
    if (feof( pipe)) {
        printf( "\nProcess returned %d\n", pclose( pipe ) );
    } else {
        printf( "Error: Failed to read the pipe to the end.\n");
    }
}
