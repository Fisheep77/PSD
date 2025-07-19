#include "main.h"

PID_TypeDef Body_Angle_PID;            //底盘跟随角度pid

void Body_AngleControlInit( void )
{
    //底盘速度环pid值
    const static float body_angle_pid[3] = {BODY_ANGLE_PID_KP, BODY_ANGLE_PID_KI, BODY_ANGLE_PID_KD};

    //初始化角度PID
    PID_init(&Body_Angle_PID, PID_DELTA, body_angle_pid, BODY_ANGLE_PID_MAX_OUT, BODY_ANGLE_PID_MAX_IOUT );
}


/*
*********************************************************************************************************
*   函 数 名: BM50_VelControl
*   功能说明: 电机速度控制(PID算法)
*   形    参: _MotorVelRPM : 目标转速(rad/s)
*   返 回 值: 无
*********************************************************************************************************
*/
float Body_AngleControl(float _YawSet)
{
    float set = _YawSet;        //目标角度
    float ref = Yaw;    //实际角度

    float out = PID_calc(&Body_Angle_PID, ref, set ); //增量PID计算之后的输出

    return out;
}
