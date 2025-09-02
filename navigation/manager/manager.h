#include<iostream>
#include<ext/stdio_filebuf.h>
#include<unistd.h>
#include<fstream>
using namespace std;
static __gnu_cxx::stdio_filebuf<char>** buffers;
static iostream** streams;
void fd_to_stream (char* fds[], int n) {
    buffers=new __gnu_cxx::stdio_filebuf<char>*[n];
    streams=new iostream*[n];
    for (int i=0; i<n; i++) {
        if (i%2==0) buffers[i]=new __gnu_cxx::stdio_filebuf<char>(atoi(fds[i]),ios_base::in);
        else buffers[i]=new __gnu_cxx::stdio_filebuf<char>(atoi(fds[i]),ios_base::out);
        streams[i]=new iostream(buffers[i]);
    }
}
#define in(ind) (*(streams[2*(ind)]))
#define out(ind) (*(streams[2*(ind)+1]))
