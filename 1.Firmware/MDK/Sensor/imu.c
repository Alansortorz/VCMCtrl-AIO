#include "imu.h"
#include "usart.h"

/**
  * @brief   写数据到MPU6050寄存器
  * @param
  * @retval
  */
void MPU6050_WriteReg(uint8_t reg_add,uint8_t reg_dat){
    i2c_Start();
    i2c_SendByte(MPU6050_SLAVE_ADDRESS);
    i2c_WaitAck();
    i2c_SendByte(reg_add);
    i2c_WaitAck();
    i2c_SendByte(reg_dat);
    i2c_WaitAck();
    i2c_Stop();
}

/**
  * @brief IIC连续写数据
  * @param addr-器件地址；reg-寄存器地址；len-写入数据的长度；buf-写入的数据
  *	@retval 0-操作成功；1-操作失败
  * @note 这里的器件就是陀螺仪
  */
uint8_t MPU_Write_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf){
    uint8_t i;
    i2c_Start();
    i2c_SendByte((addr<<1)|0);	//发送器件地址并且写命令
    if(i2c_WaitAck()){
        i2c_Stop();
        return 1;
    }
    i2c_SendByte(reg);			//写入寄存器地址
    i2c_WaitAck();
    for(i=0; i<len; ++i){
        i2c_SendByte(buf[i]);	//连续发送长度为len的数据
        if(i2c_WaitAck()){		//如果无应答，则写入数据失败
            i2c_Stop();
            return 1;
        }
    }
    i2c_Stop();
    return 0;
}

/**
  * @brief IIC连续读数据
  * @param addr-器件地址；reg-寄存器地址；len-写入数据长度；buf-写入的数据
  * @retval 0-成功；1-失败
  */
uint8_t MPU_Read_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf){
    i2c_Start();				//开启IIC总线通讯
    i2c_SendByte((addr<<1)|0);	//发送器件地址并且写命令
    if(i2c_WaitAck()){			//等待应答，若无应答，则关闭总线，函数终止
        i2c_Stop();
        return 1;
    }
    i2c_SendByte(reg);			//发送寄存器地址
    i2c_WaitAck();				//等待应答
    i2c_Start();
    i2c_SendByte((addr<<1)|1);
    i2c_WaitAck();
    while(len){
        if(len == 1)
            *buf = i2c_ReadByte(0);
        else
            *buf = i2c_ReadByte(1);
        len--;
        buf++;
    }
    i2c_Stop();					//IIC总线通讯停止
    return 0;
}

/**
  * @brief   从MPU6050寄存器读取数据
  * @param
  * @retval
  * @note char类型数据占1个字节(8位)，取值范围为-128-127
  */
void MPU6050_ReadData(uint8_t reg_add,char* Read,uint8_t num){
    unsigned char i;
    i2c_Start();
    i2c_SendByte(MPU6050_SLAVE_ADDRESS);//发送设备地址
    i2c_WaitAck();
    i2c_SendByte(reg_add);
    i2c_WaitAck();

    i2c_Start();
    i2c_SendByte(MPU6050_SLAVE_ADDRESS+1);
    i2c_WaitAck();

    for(i=0;i<(num-1);i++){
        *Read=i2c_ReadByte(1);//读取数据
        Read++;
    }
    *Read=i2c_ReadByte(0);
    i2c_Stop();
}

/**
  * @brief   初始化MPU6050芯片
  * @param
  * @retval
  */
void MPU6050_Init(void){
    int i=0,j=0;
    //在初始化之前要延时一段时间，若没有延时，则断电后再上电数据可能会出错
    for(i=0;i<1000;i++){
        for(j=0;j<1000;j++){
            ;
        }
    }
    MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);	    //解除休眠状态
    MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV , 0x07);	    //陀螺仪采样率，1KHz
    MPU6050_WriteReg(MPU6050_RA_CONFIG , 0x06);	        //低通滤波器的设置，截止频率是1K，带宽是5K
    MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG , 0x00);	//配置加速度传感器工作在2G模式，不自检
    MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);     //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
}

/**
  * @brief   读取MPU6050的ID
  * @param
  * @retval
  */
uint8_t MPU6050ReadID(void){
    char Re = 0;
    MPU6050_ReadData(MPU6050_RA_WHO_AM_I, &Re, 1);    //读器件地址
    if(Re != 0x68){
		printf("Imu Read ID Success\n");
        return 0;
    }
    else{
        printf("Imu Read ID Success\n");
        return 1;
    }
}

/**
  * @brief   读取MPU6050的加速度数据
  * @param
  * @retval
  */
void MPU6050ReadAcc(short *accData){
    char buf[6];//8位
    MPU6050_ReadData(MPU6050_ACC_OUT, buf, 6);
    //左移八位表示x2^8，即乘以256，buf[0]作为高八位，与后八位buf[1]做按位"或"运算
    //accData为short类型，16位，加速度量程为+-2g，实际输出值为+-16384(数据手册第13页)
    accData[0] = (buf[0] << 8) | buf[1];
    accData[1] = (buf[2] << 8) | buf[3];
    accData[2] = (buf[4] << 8) | buf[5];
}

/**
  * @brief   读取MPU6050的角加速度数据
  * @param
  * @retval
  */
void MPU6050ReadGyro(short *gyroData){
    char buf[6];
    MPU6050_ReadData(MPU6050_GYRO_OUT,buf,6);
    gyroData[0] = (buf[0] << 8) | buf[1];
    gyroData[1] = (buf[2] << 8) | buf[3];
    gyroData[2] = (buf[4] << 8) | buf[5];
}

/**
  * @brief   读取MPU6050的原始温度数据
  * @param
  * @retval
  */
void MPU6050ReadTemp(short *tempData){
    char buf[2];
    MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //读取温度值
    *tempData = (buf[0] << 8) | buf[1];
}

/**
  * @brief   读取MPU6050的温度数据，转化成摄氏度
  * @param
  * @retval
  */
void MPU6050_ReturnTemp(float *Temperature){
    short temp3;
    char buf[2];

    MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //读取温度值
    temp3= (buf[0] << 8) | buf[1];
    *Temperature=((double) temp3/340.0)+36.53;
}

