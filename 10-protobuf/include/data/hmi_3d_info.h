#ifndef HMI_3D_INFO_H_
#define HMI_3D_INFO_H_

#include <stdint.h>

//protobuf头文件
#include "hmi_3d/mv_uss.pb.h"

#define MAX_HMI_3D_INFO_LEN 1024

#if 1

//界面信息
typedef struct
{
    uint64_t    lTimestamp_ms;       //时间戳:ms
    uint8_t     Hmi_apa_sel;         //点击进入APA；1-点击 
    uint8_t     Hmi_park_mode;       //泊车模式：1-泊入，2-泊出，3-自选；现代HPA仅支持泊入
    uint8_t     Hmi_start_park;      //开始泊车，进入guidance状态:1-开始
    uint8_t     Hmi_cancel_req;      //自动泊车取消请求：1-发出请求
    uint8_t     Hmi_Selected;        //车位或泊出方向已选择：1-已选择 
    uint8_t     Hmi_pause_req;       //自动泊车暂停请求：1-发出请求
    uint8_t     Hmi_resum_req;       //自动泊车恢复请求：1-发出请求     
    uint8_t     Hmi_tragParkConfSwt; //循迹倒车确认按键：1-发出请求  
    uint32_t    Hmi_parkslot_id;     //选择的泊入车位ID     
    uint32_t    Hmi_parkout_dir;     //选择的泊出方向,参考ApaParkOutDir
    float       Hmi_slot_points[8];  //选中的车位角点，主要用于自选     
    uint8_t Reserve[64-56];
}Hmi_test_info;

//slot
typedef struct
{
    uint64_t TimeMsec;            // 时间戳(ms)
    struct {
          uint8_t  APA_PrkgSlot_Sts;   // 车位状态 0x0: None 0x1: Valid 0x2: Selected 0x3: Invalid-No Barrier 0x4: Occupied by vehicles 0x5: Occupied by cone barrel 0x6: Occupied by Looks 0x7: Valid-Narrow 0x8: Selected-Narrow 0x9~0xF: Reserved
          uint8_t  APA_SlotACoorX;     // 车位A点X坐标 0x0: None 0x1~0x64: -100cm~-1cm 0x65: 0cm 0x66~0xC9: 1cm~100cm 0xCA~0xFE: Reserved 0xFF: Invalid
          uint8_t  APA_SlotACoorY;     // 车位A点Y坐标 0x0: None 0x1~0x96: -150cm~-1cm 0x97: 0cm 0x98~0xC9: 1cm~50cm 0xCA~0xFE: Reserved 0xFF: Invalid
          uint8_t  APA_SlotBCoorX;     // 车位B点X坐标 0x0: None 0x1~0x64: -100cm~-1cm 0x65: 0cm 0x66~0xC9: 1cm~100cm 0xCA~0xFE: Reserved 0xFF: Invalid
          uint8_t  APA_SlotBCoorY;     // 车位B点Y坐标 0x0: None 0x1~0x96: -150cm~-1cm 0x97: 0cm 0x98~0xC9: 1cm~50cm 0xCA~0xFE: Reserved 0xFF: Invalid
          uint8_t  APA_SlotCCoorX;     // 车位C点X坐标 0x0: None 0x1~0x96: -150cm~-1cm 0x97: 0cm 0x98~0xC9: 1cm~50cm 0xCA~0xFE: Reserved 0xFF: Invalid
          uint8_t  APA_SlotCCoorY;     // 车位C点Y坐标 0x0: None 0x1~0x96: -150cm~-1cm 0x97: 0cm 0x98~0xC9: 1cm~50cm 0xCA~0xFE: Reserved 0xFF: Invalid
          uint8_t  APA_SlotDCoorX;     // 车位D点X坐标 0x0: None 0x1~0x96: -150cm~-1cm 0x97: 0cm 0x98~0xC9: 1cm~50cm 0xCA~0xFE: Reserved 0xFF: Invalid
          uint8_t  APA_SlotDCoorY;     // 车位D点Y坐标 0x0: None 0x1~0x96: -150cm~-1cm 0x97: 0cm 0x98~0xC9: 1cm~50cm 0xCA~0xFE: Reserved 0xFF: Invalid
        //   uint8_t  bias_valid_direction;   //偏置可用方向   0-居中；1-支持居中，居左，居右
        //   uint8_t  APA_PrkgSlotLimit_Sts;  //挡轮杆位置    0x0:无; 0x1:AB边侧; 0x2:BC边侧; 0x3:CD边侧; 0x4:DA边侧
        //   uint8_t  reserved[5];
       } PrkgSlot[16];
} ApaAvapSlotOut;     

//PDC
typedef struct
{
    uint32_t nFrameId;
    uint32_t nRervered;
    uint64_t ITimeMsec;
} ZU2UssSector_FrameHeader_t;
typedef struct
{
    uint16_t PDC_Distance[32]; /* 12传感器各自对应一个Sector，侧面左右各2个虚拟Sector，剩余16个Setor预留 */
    uint16_t LAEB_Distance[32];
    uint8_t SensorStatus[12];
       uint8_t SystemStatus;
    uint8_t nRervered;
} ZU2UssSectorInfo_t;

typedef struct
{
    ZU2UssSector_FrameHeader_t FrameHead;
    ZU2UssSectorInfo_t SectorData;
} ZU2UssSectorOutputData_t;


#endif

#endif