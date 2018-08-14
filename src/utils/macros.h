//
// Created by llx on 2018/8/8.
//

#ifndef MPC_MACROS_H
#define MPC_MACROS_H
#include<memory>
/** fast type convert for moving address forward */
#define ADDRESS_ADD(x, delta) (void*)((uintptr_t)x + delta)

#define DECLARE_PTR(X) typedef std::shared_ptr<X> X##Ptr; /**< define smart ptr */

#define FORWARD_DECLARE_PTR(X) class X; DECLARE_PTR(X)      /**< forward defile smart ptr */

#define MPC_JOURNAL_NAME_PREFIX "Journal"
#define MPC_JOURNAL_NAME_ENDING "jrnl"

#define  __FRAME_HEADER_VERSION__ 1

#define SYSTEM_JOURNAL_DIR "/home/llx/Documents/trading/system"
#define SYSTEM_JOURNAL_NAME "SYSTEM_JOURNAL"


#endif //MPC_MACROS_H
