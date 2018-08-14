#include <iostream>
#include "communicator/Writer.h"
using std::cout;
using std::endl;

int main() {
    auto writer=Writer::create("/home/llx/Documents/trading/testjournal","testj5");
    for(int i=0;i<=100;++i) {
        usleep(10000000);
        writer->WriteFrame(static_cast<void *>(&i), sizeof(int));
    }

    return 0;
}