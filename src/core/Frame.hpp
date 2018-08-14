#ifndef MPC_FRAME_HPP
#define MPC_FRAME_HPP

#include "FrameHeader.h"
#include "../utils/macros.h"
#include <string.h> // memcpy

#include<iostream>


template<FH_TYPE_LENGTH N>
class FrameTMP
{
private:
    /** address with type,
     * will keep moving forward until change page */
    FrameHeaderTMP<N> *frame_header;
public:
    /** default constructor */
    FrameTMP(void * fm) {
        set_address(fm);
    }
    /** setup basic frame address */
    void set_address(void * fm) {

        frame_header = (FrameHeaderTMP<N>*)fm;
    }
    /** get current address */
    void* get_address() const{
        return frame_header;
    }

    // get fields of header from frame
    FH_TYPE_STATUS  getStatus() const{
        return frame_header->status;
    }
    FH_TYPE_NANOTM  getNano() const{
        return frame_header->nano;
    }
    FH_TYPE_LENGTH  getFrameLength() const{
        return frame_header->length;
    }
    FH_TYPE_LENGTH  getHeaderLength() const {
        return BASIC_FRAME_HEADER_LENGTH<N>();
    }
    FH_TYPE_LENGTH  getDataLength() const{
        return getFrameLength()-getHeaderLength();
    }

    /** get address of data field */
    void* getData() const{
        return ADDRESS_ADD(frame_header, getHeaderLength());
    }

    void setNano(FH_TYPE_NANOTM nano_){
        frame_header->nano=nano_;
    }

    /** set data with length */
    void setData(const void * data, size_t dataLength){
        memcpy(ADDRESS_ADD(frame_header, BASIC_FRAME_HEADER_LENGTH<N>()), data, dataLength);
        setFrameLength(BASIC_FRAME_HEADER_LENGTH<N>() + dataLength);

    }
    /** mark status as written */
    void setStatusWritten(){
        getNextEntry()->status = MPC_FRAME_STATUS_RAW;
        setStatus(MPC_FRAME_STATUS_WRITTEN);
    }



    /** move the frame forward by length */
    FH_TYPE_LENGTH next(){
        FH_TYPE_LENGTH len = getFrameLength();
        frame_header = getNextEntry();
        return len;
    }

private:
    /** return address of next frame header */
    FrameHeaderTMP<N>* getNextEntry() const{
        return static_cast<FrameHeaderTMP<N>* >(ADDRESS_ADD(frame_header, getFrameLength()));
    }
    /** set status, internally used */
    void setStatus(FH_TYPE_STATUS status_){
        frame_header->status=status_;
    }
    /** set length, internally used */
    void setFrameLength(FH_TYPE_LENGTH length_){
        frame_header->length=length_;
    }
};

#define Frame FrameTMP<100>
#define FrameHeader FrameHeaderTMP<100>

DECLARE_PTR(Frame)

#endif
