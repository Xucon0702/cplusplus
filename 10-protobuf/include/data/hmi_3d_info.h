#ifndef HMI_3D_INFO_H_
#define HMI_3D_INFO_H_

#include <cstdint>

//protobuf头文件
#include "hmi_3d/mv_uss.pb.h"
#include "hmi_3d_package.pb.h"
#include "hmi_3d/mv_slot.pb.h"
#include <atomic>

#define MAX_HMI_3D_INFO_LEN 1024

#define F_CONVERT_RATE (0.001)

#define MAX_QUEUE_SIZE    16

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

//OD
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
} MvApaAvapObjOut;        

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

//can
typedef struct _MV_CAN_CAR_INFO_256
{
    uint32_t nStructLen;            //本结构体的大小，用于上位机校验是否有兼容性问题的平台差异
    uint32_t nFrameId;              //帧号
    uint64_t lTimeMsec;             //时间戳(ms)
    int32_t nBrake;                 //刹车  1，刹车  0，未刹车
    int32_t nLLight;                //左转向灯 1、点亮 0、不亮
    int32_t nRLight;                //右转向灯 1、点亮 0、不亮
    float fVelocity;                //车速 
    int32_t nBrakeValue;            //刹车力度 用于制动
    float fAccelPedalPosition;      //油门力度
    float fLongiAccel;              //纵向加速度
    float fLateralAccel;            //横向加速度
    float fYawRate;                 //偏航角速度
    int32_t nTransmissionStatus;    //档位 0无效 1P 2R 3N 4D
    float fAlpha;                   //车轮转向角
    float fSteeingWheelAngle;       //方向盘转向角，顺时针为正，角度
    float fSteeingWheelRate;        //方向盘转向速率
    int32_t nParkBrake ;            //手刹 0 手刹未起作用 1 手刹起作用 2 错误 3 无效
    int32_t nWipeWasherSwitch;      //雨刮 0不工作 1低档 2中档 3高档 4间歇档
    int32_t nHazardLightSwitch;     //危险报警灯开关 0无效 1关 2开
    int32_t nHighLowBeamSwitch;     //近光远光灯开关 0无效 1近光 2远光 3错误 3无效
    uint16_t nFLWheelSpeedRC;       //左前轮速脉冲信号
    uint16_t nFRWheelSpeedRC;       //右前轮速脉冲信号
    uint16_t nRLWheelSpeedRC;       //左后轮速脉冲信号
    uint16_t nRRWheelSpeedRC;       //右后轮速脉冲信号
    uint8_t nDoorStat[4];           //[0-4]左前,右前,左后,右后,0关门 1开门
    int32_t nPulseDirection;        //轮速脉冲方向
    //add 11-29
    float   fWheelSpd;              //估计轮速
    float   x;
    float   y;
    float   yaw;                    //yaw
    uint8_t nBonnetSts;             // 引擎盖 0关闭 1打开
    uint8_t nTrunkSts;              // 后备箱 0关闭 1打开
    uint8_t nSRFSts;                // 天窗 0关闭 1打开
    uint8_t nReserve;
    /* 新增字段 */
    uint8_t BCM_KeySt;              // 上电检查状态
    uint8_t ACU_SVM_Button;         // 硬开关
    uint8_t ACU_VR_SVM_Switch;      // 语音开关
    uint8_t hazardLampSt;           // 双闪开关
    /* 新增字段 */
    uint8_t ExternalTemperature;         //车身外环境温度
    uint8_t ExternalTemperatureValidity; //车身外环境温度信号有效标志
    uint8_t nReserves[2];
    uint64_t lTimeMsecWheelPulse;        //轮脉冲时间戳(ms)
    uint64_t lTimeMsecWheelSpeed;        //车速时间戳(ms)
    uint64_t lTimeMsecSteerAngle;        //方向盘转角时间戳(ms)
    uint8_t        nFLWheelSpeedRC_Vld;             //左前轮速脉冲累计有效位Validty of signal 'ESP_WhlOdoEdges_FL'. [3 "Reserved" 2 "Invalid" 1 "Valid" 0 "Initializing"]
    uint8_t        nFRWheelSpeedRC_Vld;             //右前轮速脉冲累计有效位Validty of signal 'ESP_WhlOdoEdges_FR'.
    uint8_t        nRLWheelSpeedRC_Vld;             //左后轮速脉冲累计有效位Validty of signal 'ESP_WhlOdoEdges_RL'.
    uint8_t        nRRWheelSpeedRC_Vld;             //右后轮速脉冲累计有效位Validty of signal 'ESP_WhlOdoEdges_RR'.
    uint8_t nSteeingWheelRateVld;        //方向盘转速有效位
    uint8_t nBrakeVld;                   //制动踏板状态有效位
    uint8_t nWhlSpdVld_LF;               //左前轮速度有效位
    uint8_t        nAccelPedalPositionVld;             //驾驶员加速踏板开度有效位
    uint8_t        YRS_LgtSnsrSts;                     //纵向传感器的状态
    uint8_t        YRS_LatSnsrSts;                     //横向传感器的状态
    uint8_t        YRS_YawRateSnsrSts;                 //偏航率传感器的状态
    uint8_t        nSteeingWheelAngleVld;             //方向盘转角信号有效位
    uint8_t        nRvsLampOutpCmd;                 //倒车灯信号
    uint8_t nReserve1[3];
    uint64_t lTimeSec;                   //MCU时间戳(s)
    uint8_t        TrailHook;                         //拖车钩 0无效，1有效。默认值 ：0无效
    uint8_t        ExternalHumidity;                 //车身外环境湿度。预留。默认值 ：0
    uint8_t        ExternalHumidityValidity;         //车身外环境湿度有效标志0无效，1有效。预留。默认值 ：0无效
    uint8_t        nReserved[256-179];                 /*保留字段*/
}MvCanCarInfo;

//APA状态机信息
typedef struct
{    
    uint64_t  lTimestampMs;            // 时间戳(ms)
    uint8_t   AVAP_APAFunDispSts;      // APA功能显示状态 0x0: Not Active 0x1: Background Scanning 0x2: Active Scanning 0x3: Selecting 0x4: Confirming 0x5: Manoeuvering 0x6: Paused 0x7: Finishing 0x8: Ended/Cancel 0x9: Failured 0xA~0xF: Reserved 
    uint8_t   AVAP_InfoMsgs;           // APA 系统提示信息 0x0: None 0x1 ~ 0x19: Message1 ~ Message25 0x1A~0x3F: Reserved
    uint8_t   AVAP_PrkgPauseMsgs;      // APA 功能暂停原因 0x0: NOne 0x1 ~ 
    uint8_t   APA_CnclMsgs;            // APA 功能取消原因 0x0: None 0x1 - 0xF: message1 - message15
    uint8_t   APA_FctUnavlMsgs;        // APA 功能不能激活的原因 0x0: None 0x1 ~ 0xC: Message1 ~ Message12 0xD~0xF: Reserved    
    uint8_t   Reserved[3];
}ApaStateToHmiTestInfo;    

//下一步位置框和单步路径
typedef struct _Vector2d
{
    float x;
    float y;
}Vector2d;
typedef struct _LocationPoint_pps
{
    float x;
    float y;
    float yaw;
}LocationPoint_pps;
typedef struct _TargetTrack {
    Vector2d point;
    float yaw;
    float curvature;
    float speed;
    int nDetectStates;
} TargetTrack; 
typedef struct _PlanToHdmi
{
    uint32_t  uCmd;                        //0:无效 1:泊入 2:泊出
    uint32_t  uStatue;                //[b23 - b16]泊车总状态       0:泊车成功 1:失败
                            //[b15 - b 8]规划状态：                 0:泊车到位 1:规划成功 2:规划失败
                                //[b 7 - b 0]控制状态：           0:处于运动状态 1:静止状态 2:泊车完成 3:无跟踪目标点数据 4:目标点索引超限 5:跟踪偏离轨迹超限 6:障碍物逼停等待超时
                            //7:障碍物前后逼停 8:减速带停止 9:控制模式错误
    uint32_t  uTotalStep;                //总进度
    uint32_t  uCurrentStep;        //当前进度
    uint32_t  uDisStep;                //小进度,当前段剩余距离,单位5cm
    uint32_t  uPlanOutDetectSts;//泊出道路通畅
    float px[4];
    float py[4];
}PlanToHdmi;
typedef struct
{    
    uint64_t  lTimeMsec;
    PlanToHdmi  tPlanToHdmi;
    uint32_t validNum; //有效路径段数
    TargetTrack  aTracksPlan[400];//当前段的所有路点数据,间隔5cm
    LocationPoint_pps  syncDr;     //泊入算法更新使用的dr
}ApaPlanTrackInfo;   

//规划的全路径
#define MAX_PATHS_SIZE  18
#define MAX_PATH_PER_SIZE  200
typedef struct {
    float fX;
    float fY;
    float fCurv;
}ApaPathPoint;

typedef struct {
  uint32_t validNum; //每段有效路点个数
  ApaPathPoint  aPoints[MAX_PATH_PER_SIZE];
}PathPointsInfo;
typedef struct
{
    uint64_t  lTimeMsec;
    uint8_t uint8_APATotalStep;
    uint8_t uint8_APACurStep;
    uint8_t uint8_APAPathGear;
    uint8_t reserved;
    uint16_t uint16_APAPathLen;
    uint16_t uint16_APARmPathLen;
    uint16_t uint16_StopDistance;
    uint16_t validNum; //有效路径段数
    PathPointsInfo  Array_APAPathPoints[MAX_PATHS_SIZE]; //优化为vector
} PlanFullPath;
#endif

//整包数据缓存
typedef struct
{
    std::atomic<uint32_t> nReadId;
	std::atomic<uint32_t> nWriteId;
	ApaStateToHmiTestInfo aApaStateInfo[MAX_QUEUE_SIZE];
}ApaStateInfoQueue;
typedef struct
{
    std::atomic<uint32_t> nReadId;
	std::atomic<uint32_t> nWriteId;
	MvCanCarInfo aCanInfo[MAX_QUEUE_SIZE];
}CanInfoQueue;
typedef struct
{
    std::atomic<uint32_t> nReadId;
	std::atomic<uint32_t> nWriteId;
	MvApaAvapObjOut aOdInfo[MAX_QUEUE_SIZE];
}OdInfoQueue;
typedef struct
{
    std::atomic<uint32_t> nReadId;
	std::atomic<uint32_t> nWriteId;
	PlanFullPath aPlanFullPath[MAX_QUEUE_SIZE];
}PlanFullPathQueue;
typedef struct
{
    std::atomic<uint32_t> nReadId;
	std::atomic<uint32_t> nWriteId;
	ApaPlanTrackInfo aPlanTrack[MAX_QUEUE_SIZE];
}PlanTrackQueue;
typedef struct
{
    std::atomic<uint32_t> nReadId;
	std::atomic<uint32_t> nWriteId;
	ApaAvapSlotOut aSlotInfo[MAX_QUEUE_SIZE];
}SlotInfoQueue;
typedef struct
{
    std::atomic<uint32_t> nReadId;
	std::atomic<uint32_t> nWriteId;
	ZU2UssSectorOutputData_t aUssPdcInfo[MAX_QUEUE_SIZE];
}UssInfoQueue;

//Caching is done to reduce the use of locks
typedef struct
{
    ApaStateInfoQueue tApaStateInfoQueue;
    CanInfoQueue tCanInfoQueue;
    SlotInfoQueue tSlotInfoQueue;
    OdInfoQueue tOdInfoQueue;
    UssInfoQueue tUssInfoQueue;
    PlanTrackQueue tPlanTrackQueue;
    PlanFullPathQueue tPlanFullPathQueue;    
}Hmi3dBufQueue;

typedef struct
{
    ApaStateToHmiTestInfo tApaStateInfo;
    MvCanCarInfo tMvCanCarInfo;
    ApaAvapSlotOut  tApaAvapSlotOut;
    MvApaAvapObjOut tMvApaAvapObjOut;
    ZU2UssSectorOutputData_t tUssPdc;
    ApaPlanTrackInfo tPlanTrackInfo;
    PlanFullPath tPlanFullPath;
}MvHmi3dInfo;

#endif