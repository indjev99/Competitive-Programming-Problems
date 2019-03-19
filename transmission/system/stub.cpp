#include "transmission.h"
#include <stdlib.h>
#include <cstdio>

static int n,d,m2;
static int processNo;

static std::vector<bool> data;
static std::vector<bool> message;
static std::vector<bool> corruptedMessage;
static std::vector<bool> reconstructedData;

int main(int argc, char **argv)
{
    FILE *fifo_in, *fifo_out;
    fifo_in = fopen(argv[2],"r");
    fifo_out = fopen(argv[1],"w");
    processNo = atoi(argv[3]);

    int x;

    if (processNo == 0) // transmit
    {
        fscanf(fifo_in,"%d %d",&n,&d);
        data.resize(n);
        for(int i=0;i<n;++i)
        {
            fscanf(fifo_in,"%d",&x);
            data[i]=x;
        }
        message=transmit(data,d);
        fprintf(fifo_out,"%d\n",message.size());
        for(int i=0;i<message.size();++i)
        {
            if (i) fprintf(fifo_out," ");
            fprintf(fifo_out,"%d",(int)message[i]);
        }
        fprintf(fifo_out,"\n");
        fflush(fifo_out);
    }
    else // receive
    {
        fscanf(fifo_in,"%d %d %d",&n,&d,&m2);
        corruptedMessage.resize(m2);
        for (int i=0;i<m2;++i)
        {
            fscanf(fifo_in,"%d",&x);
            corruptedMessage[i]=x;
        }
        reconstructedData=receive(corruptedMessage,n,d);
        fprintf(fifo_out,"%d\n",reconstructedData.size());
        for(int i=0;i<reconstructedData.size();++i)
        {
            if (i) fprintf(fifo_out," ");
            fprintf(fifo_out,"%d",(int)reconstructedData[i]);
        }
        fprintf(fifo_out,"\n");
        fflush(fifo_out);
    }
    fclose(fifo_in);
    fclose(fifo_out);
    return 0;
}
