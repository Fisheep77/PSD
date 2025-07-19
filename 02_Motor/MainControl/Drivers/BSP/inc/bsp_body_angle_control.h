#pragma once

// 系统参数
#define BODY_ANGLE_CONTROL_TIME    0.050f       // 控制周期(s)
#define BODY_ANGLE_CONTROL_TIME_MS 50           // 控制周期(ms)

// 电机速度环PID参数
#define BODY_ANGLE_PID_KP       0.5f
#define BODY_ANGLE_PID_KI       0.1f
#define BODY_ANGLE_PID_KD       0.0f
#define BODY_ANGLE_PID_MAX_OUT  10.0f
#define BODY_ANGLE_PID_MAX_IOUT 5.0f

void Body_AngleControlInit(void);
float Body_AngleControl(float _YawSet);

