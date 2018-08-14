#ifndef MPC_FRAMEHEADER_H
#define MPC_FRAMEHEADER_H


// data types of all fields in frame header
// which need to be fully controlled for data length.
typedef unsigned char    FH_TYPE_STATUS;
typedef long    FH_TYPE_NANOTM;
typedef int     FH_TYPE_LENGTH;
typedef int     FH_TYPE_ERR_ID;
typedef int     FH_TYPE_FRAME_ID;

//////////////////////////////////////////
/// (byte) JournalFrameStatus
//////////////////////////////////////////
#define MPC_FRAME_STATUS_RAW        0
#define MPC_FRAME_STATUS_WRITTEN    1


//////////////////////////////////////////
/// (byte) JournalFrameLastFlag
//////////////////////////////////////////
#define JOURNAL_FRAME_NOT_LAST          0 /**< false */
#define JOURNAL_FRAME_IS_LAST           1 /**< true */

template <FH_TYPE_LENGTH N>
struct FrameHeaderTMP
{
    /** JournalFrameStatus */
    volatile FH_TYPE_STATUS      status;
    /** nano time of the frame data */
    FH_TYPE_NANOTM      nano;
    /** total frame length (including header and errorMsg) */
    FH_TYPE_LENGTH      length;
    /*保留区域，用于用户自定义header数据*/
    char                preserve[N];
}__attribute__((packed));

/** length of frame header */
template <FH_TYPE_LENGTH N>
constexpr FH_TYPE_LENGTH BASIC_FRAME_HEADER_LENGTH(){
    return  sizeof(FrameHeaderTMP<N>);
}


#endif //YIJINJING_FRAMEHEADER_H
