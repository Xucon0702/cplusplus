#ifndef HMI_TEST_DATA_H
#define HMI_TEST_DATA_H
#include <stdint.h>

#define HMI_TEST_DATA_VERSION "1.0.0" //2024-08-15

//数据协议
#define CRC_HEAD 0x6D76  //"mv"
#define PAYLOAD_LENGTH 4   //存放有效数据的实际长度的数据长度
#define TOTAL_HEAD_LENGTH 7
// #define CRC_TAIL 0x6D76

#define PAYLOAD_LENGTH_MAX 10240 //最大数据长度

//判断具体是什么数据
typedef enum : uint8_t{
    // HMI输出的信息
    HMI_OUT_INFO = 0X01, //Hmi_test_info
    //域控输出的信息
    TRANSPORT_PARK_INFO = 0X11, //车位信息
    TRANSPORT_OD_INFO = 0X12, // OD信息
    TRANSPORT_APA_STATE_INFO = 0X13, // APA状态机信息
    INFO_HEAD_MAX = 0XFF
} INFO_HEAD_E;

typedef enum  ApaParkOutDir_ {
    ParkOutVertHdLeAvl = 1, // 垂直向左前
    ParkOutVertHdRiAvl = 2, // 垂直向右前
    ParkOutForwardAvl = 3,  // 向前直出
    ParkOutParaLeAvl = 4,   // 水平向左
    ParkOutParaRiAvl = 5,   // 水平向右
    ParkOutVertTrLeAvl = 6, // 车尾向左后
    ParkOutVertTrRiAvl = 7, // 车尾向右后
    ParkOutBackAvl = 8      // 向后直出
} ApaParkOutDir;

#pragma pack(1)
//数据头结构
typedef struct Data_head_interaction_
{
    uint16_t    crc_head;       //校验头
    uint8_t     info_head;      //信息头
    uint32_t    payload_length; //有效数据实际长度
}Data_head_interaction;


#pragma pack(1)
//上位机测试信息
#define TRANSPORT_HMI_TEST_INFO "Transport_hmi_test_info"
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
    float       Hmi_slot_points[4];  //选中的车位角点，主要用于自选     
    uint8_t Reserve[64-40];
}Hmi_test_info;
#pragma pack()

//车位信息
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
          uint8_t  bias_valid_direction;   //偏置可用方向   0-居中；1-支持居中，居左，居右
          uint8_t  APA_PrkgSlotLimit_Sts;  //挡轮杆位置    0x0:无; 0x1:AB边侧; 0x2:BC边侧; 0x3:CD边侧; 0x4:DA边侧
          uint8_t  reserved[5];
       } PrkgSlot[16];
} ApaAvapSlotOut;     

//OD信息
typedef struct
{
    uint64_t  TimeMsec;            // 时间戳(ms)
    struct {
        uint8_t   AVAP_StcBarrierType;  // 静态障碍物类别 0x0: Not detected 0x1: 地锁 0x2: 限位杆 0x3: 限位块 0x4: 锥桶 0x5: 立柱 0x6: 水马 0x7: 禁停牌 0x8: 防撞柱 0x9~0xF: Reserved
        uint8_t   AVAP_StcBarrier_X;    // 静态障碍物X坐标 0x0: None 0x1~0x64: -100dm~-1dm 0x65: 0dm 0x66~0xC9: 1dm~100dm 0xCA~0xFE: Reserved 0xFF: Invalid
        uint8_t   AVAP_StcBarrier_Y;    // 静态障碍物Y坐标 0x0: None 0x1~0x96: -150dm~-1dm 0x97: 0cm 0x98~0xC9: 1dm~50dm 0xCA~0xFE: Reserved 0xFF: Invalid
        uint8_t   AVAP_StcBarrierDrct;  // 静态障碍物方向 0x0: 向上 0x1: 向下 0x2: 向左 0x3: 向右 0x4: 向左上 0x5: 向左下 0x6：向右上 0x7：向右下
      } AVAP_StcBarrier[16];//静态障碍物
    struct {  
        uint8_t   AVAP_DynBarrierType;  // 动态障碍物类别 0x0: Not detected 0x1: 小汽车 0x2: 行人 0x3: 二轮车 0x4: 巴士 0x5: 卡车 0x6~0x7: Reserved
        uint8_t   AVAP_DynBarrier_X;    // 动态障碍物X坐标 0x0: None 0x1~0x64: -100dm~-1dm 0x65: 0dm 0x66~0xC9: 1dm~100dm 0xCA~0xFE: Reserved 0xFF: Invalid
        uint8_t   AVAP_DynBarrier_Y;    // 动态态障碍物Y坐标 0x0: None 0x1~0x96: -150dm~-1dm 0x97: 0cm 0x98~0xC9: 1dm~50dm 0xCA~0xFE: Reserved 0xFF: Invalid
        uint8_t   AVAP_DynBarrierDrct;  // 动态障碍物方向 0x0: 向上 0x1: 向下 0x2: 向左 0x3: 向右 0x4: 向左上 0x5: 向左下 0x6：向右上 0x7：向右下
      } AVAP_DynBarrier[16]; //动态障碍物
    struct {        
        uint8_t   AVAP_Speedbump_A_X;   // 减速带A点X坐标 0x0: None 0x1~0x64: -100dm~-1dm 0x65: 0dm 0x66~0xC9: 1dm~100dm 0xCA~0xFE: Reserved 0xFF: Invalid
        uint8_t   AVAP_Speedbump_A_Y;   // 减速带A点Y坐标 0x0: None 0x1~0x96: -150dm~-1dm 0x97: 0cm 0x98~0xC9: 1dm~50dm 0xCA~0xFE: Reserved 0xFF: Invalid
        uint8_t   AVAP_Speedbump_B_X;   // 减速带B点X坐标 0x0: None 0x1~0x64: -100dm~-1dm 0x65: 0dm 0x66~0xC9: 1dm~100dm 0xCA~0xFE: Reserved 0xFF: Invalid
        uint8_t   AVAP_Speedbump_B_Y;   // 减速带B点Y坐标 0x0: None 0x1~0x96: -150dm~-1dm 0x97: 0cm 0x98~0xC9: 1dm~50dm 0xCA~0xFE: Reserved 0xFF: Invalid
      } AVAP_Speedbump[3];//减速带
} ApaAvapObjOut;        

//apa状态信息
typedef struct
{
    uint64_t  lTimestampMs;       //时间戳:ms
    uint8_t   Apa_active_request;  //Apa进入泊入阶段的请求：1-请求泊车
    uint8_t   reserved[3];
}Apa_to_top_info;

#endif