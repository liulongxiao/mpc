#include <iostream>
#include "communicator/Reader.h"
using std::cout;
using std::endl;

int main() {

    auto reader=Reader::create("testReader");
    reader->addJournal("/home/llx/Documents/trading/testjournal","testj5");
    while(true){
        auto frameptr= static_cast<FrameHeader *> (reader->readFrame());
        if(frameptr== nullptr)
            continue;
        else{
        long duration=getNanoTime()-frameptr->nano;
        std::cout<<"time duration:"<<duration <<std::endl;
        }
    }

    return 0;
}