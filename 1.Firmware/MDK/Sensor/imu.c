#include "imu.h"
#include "usart.h"

/**
  * @brief   д���ݵ�MPU6050�Ĵ���
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
  * @brief IIC����д����
  * @param addr-������ַ��reg-�Ĵ�����ַ��len-д�����ݵĳ��ȣ�buf-д�������
  *	@retval 0-�����ɹ���1-����ʧ��
  * @note �������������������
  */
uint8_t MPU_Write_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf){
    uint8_t i;
    i2c_Start();
    i2c_SendByte((addr<<1)|0);	//����������ַ����д����
    if(i2c_WaitAck()){
        i2c_Stop();
        return 1;
    }
    i2c_SendByte(reg);			//д��Ĵ�����ַ
    i2c_WaitAck();
    for(i=0; i<len; ++i){
        i2c_SendByte(buf[i]);	//�������ͳ���Ϊlen������
        if(i2c_WaitAck()){		//�����Ӧ����д������ʧ��
            i2c_Stop();
            return 1;
        }
    }
    i2c_Stop();
    return 0;
}

/**
  * @brief IIC����������
  * @param addr-������ַ��reg-�Ĵ�����ַ��len-д�����ݳ��ȣ�buf-д�������
  * @retval 0-�ɹ���1-ʧ��
  */
uint8_t MPU_Read_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf){
    i2c_Start();				//����IIC����ͨѶ
    i2c_SendByte((addr<<1)|0);	//����������ַ����д����
    if(i2c_WaitAck()){			//�ȴ�Ӧ������Ӧ����ر����ߣ�������ֹ
        i2c_Stop();
        return 1;
    }
    i2c_SendByte(reg);			//���ͼĴ�����ַ
    i2c_WaitAck();				//�ȴ�Ӧ��
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
    i2c_Stop();					//IIC����ͨѶֹͣ
    return 0;
}

/**
  * @brief   ��MPU6050�Ĵ�����ȡ����
  * @param
  * @retval
  * @note char��������ռ1���ֽ�(8λ)��ȡֵ��ΧΪ-128-127
  */
void MPU6050_ReadData(uint8_t reg_add,char* Read,uint8_t num){
    unsigned char i;
    i2c_Start();
    i2c_SendByte(MPU6050_SLAVE_ADDRESS);//�����豸��ַ
    i2c_WaitAck();
    i2c_SendByte(reg_add);
    i2c_WaitAck();

    i2c_Start();
    i2c_SendByte(MPU6050_SLAVE_ADDRESS+1);
    i2c_WaitAck();

    for(i=0;i<(num-1);i++){
        *Read=i2c_ReadByte(1);//��ȡ����
        Read++;
    }
    *Read=i2c_ReadByte(0);
    i2c_Stop();
}

/**
  * @brief   ��ʼ��MPU6050оƬ
  * @param
  * @retval
  */
void MPU6050_Init(void){
    int i=0,j=0;
    //�ڳ�ʼ��֮ǰҪ��ʱһ��ʱ�䣬��û����ʱ����ϵ�����ϵ����ݿ��ܻ����
    for(i=0;i<1000;i++){
        for(j=0;j<1000;j++){
            ;
        }
    }
    MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);	    //�������״̬
    MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV , 0x07);	    //�����ǲ����ʣ�1KHz
    MPU6050_WriteReg(MPU6050_RA_CONFIG , 0x06);	        //��ͨ�˲��������ã���ֹƵ����1K��������5K
    MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG , 0x00);	//���ü��ٶȴ�����������2Gģʽ�����Լ�
    MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);     //�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
}

/**
  * @brief   ��ȡMPU6050��ID
  * @param
  * @retval
  */
uint8_t MPU6050ReadID(void){
    char Re = 0;
    MPU6050_ReadData(MPU6050_RA_WHO_AM_I, &Re, 1);    //��������ַ
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
  * @brief   ��ȡMPU6050�ļ��ٶ�����
  * @param
  * @retval
  */
void MPU6050ReadAcc(short *accData){
    char buf[6];//8λ
    MPU6050_ReadData(MPU6050_ACC_OUT, buf, 6);
    //���ư�λ��ʾx2^8��������256��buf[0]��Ϊ�߰�λ������λbuf[1]����λ"��"����
    //accDataΪshort���ͣ�16λ�����ٶ�����Ϊ+-2g��ʵ�����ֵΪ+-16384(�����ֲ��13ҳ)
    accData[0] = (buf[0] << 8) | buf[1];
    accData[1] = (buf[2] << 8) | buf[3];
    accData[2] = (buf[4] << 8) | buf[5];
}

/**
  * @brief   ��ȡMPU6050�ĽǼ��ٶ�����
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
  * @brief   ��ȡMPU6050��ԭʼ�¶�����
  * @param
  * @retval
  */
void MPU6050ReadTemp(short *tempData){
    char buf[2];
    MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //��ȡ�¶�ֵ
    *tempData = (buf[0] << 8) | buf[1];
}

/**
  * @brief   ��ȡMPU6050���¶����ݣ�ת�������϶�
  * @param
  * @retval
  */
void MPU6050_ReturnTemp(float *Temperature){
    short temp3;
    char buf[2];

    MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //��ȡ�¶�ֵ
    temp3= (buf[0] << 8) | buf[1];
    *Temperature=((double) temp3/340.0)+36.53;
}

