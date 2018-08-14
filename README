# mmap 进程间通讯

本项目参考和借鉴了[功夫交易系统](https://github.com/taurusai/kungfu)的通讯机制，将功夫中的journal通讯剥离出来，做成了一个IPC的中间件，本项目完全特点是无附加的依赖，直接编译即可运行测试。


*** 此代码并没有经过充分的测试，bug在所难免，后台自动换页操作后续将会实现 ***

# 使用方法

### 启动service 服务

编译后会生成service 可执行程序，此程序对reader和writer进行管理，包括保证每个journal最多只能有一个writer。


### 构建writer

在本项目中，参考功夫，每一个writer只能有一个journal，并且每个journal只可以有一个进程进行写操作，writer构建必须在service启动以后才可以进行，writer构建时会自动尝试链接service并注册writer，如果失败则会抛出异常。以下是writer的构建示例，注意create函数第一个参数是mmap文件存放的文件夹，第二个参数是writer的名称，由于writer只可以有一个journal，所以writer名称和journal名称相同。（此文件在src/writer.cpp中）

```C++

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

```

### 构建reader

本项目中每一个reader可以有多个journal，每次读取时将寻找最近更新的journal的frame。注意由于reader可以拥有多个journal，所以创建reader时回向service注册reader，但是需要手动添加journal。每次readFrame后会自动进行刷新操作。（此文件在src/reader.cpp中）使用reader-readFrame会返回一个指针，当不存在数据时为 nullptr；


```C++

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
```

### 编译样例


```
cd src
mkdir build
cd build
cmake ..
make

```