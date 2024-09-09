#ifndef HMI_BASE_PROTO_H_
#define HMI_BASE_PROTO_H_

#include <cstdint>

//数据协议
#define HMI_3D_PORT  (8099)
#define CRC_HEAD (0x6D76)  //"mv"
#define PAYLOAD_LENGTH 4   //存放有效数据的实际长度的数据长度
#define TOTAL_HEAD_LENGTH 8

//判断具体是什么数据
typedef enum : uint8_t{
    // HMI输出的信息
    HMI_OUT_INFO = 0X01, //Hmi_test_info
    //域控输出的信息
    TRANSPORT_PARK_INFO = 0X11, //车位信息
    TRANSPORT_OD_INFO = 0X12, // OD信息
    TRANSPORT_APA_STATE_INFO = 0X13, // APA状态机信息
    
    //HMI-3D输出的protobuf信息
    HMI_3D_OUTPUT_PROTO = 0X21,
    //transport输出的protobuf信息
    TRANSPORT_3D_PACKAGE_PROTO = 0X31,

    INFO_HEAD_MAX = 0XFF
} INFO_HEAD_E;











#endif