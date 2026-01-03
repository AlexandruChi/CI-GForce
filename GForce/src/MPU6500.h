/* MPU6500 Registers */

/* For definitions with the same name:
 * _r -> register
 * _b -> bit
 */

#ifndef MPU6500_Reg_H
#define MPU6500_Reg_H

#define MPU_ADDRESS_0 0x68 /* ADD = GND */
#define MPU_ADDRESS_1 0x69 /* ADD = VDD */

#define MPU_ADDRESS MPU_ADDRESS_0

// convert g to lsb
#define scale_2g_lsb 16384
#define scale_4g_lsb 8192
#define scale_8g_lsb 4096
#define scale_16g_lsb 2048

// convert lsb to g
#define scale_lsb_2g 0.000061035156
#define scale_lsb_4g 0.00012207031
#define scale_lsb_8g 0.00024414063
#define scale_lsb_16g 0.00048828125

// convert dps to lsb
#define scale_250dps_lsb 131
#define scale_500dps_lsb 65.5
#define scale_1000dps_lsb 32.8
#define scale_2000dps_lsb 16.4

// convert lsb to dps
#define scale_lsb_250dps 0.00763358778
#define scale_lsb_500dps 0.01526717557
#define scale_lsb_1000dps 0.03048780487
#define scale_lsb_2000dps 0.06097560975

#define Temp_Sensitivity 333.87
#define RoomTemp_Offset 0

#define TEMP_dC(X) ((((X) - RoomTemp_Offset) / Temp_Sensitivity) + 21)

#define FIFO_SIZE 512

/* Gyroscope Self-Test Registers */
#define SELF_TEST_X_GYRO 0
#define SELF_TEST_Y_GYRO 1
#define SELF_TEST_Z_GYRO 2

/* Accelerometer Self-Test Registers */
#define SELF_TEST_X_ACCEL 13
#define SELF_TEST_Y_ACCEL 14
#define SELF_TEST_Z_ACCEL 15

/* Gyro Offset Registers */
#define XG_OFFSET_H 19
#define XG_OFFSET_L 20
#define YG_OFFSET_H 21
#define YG_OFFSET_L 22
#define ZG_OFFSET_H 23
#define ZG_OFFSET_L 24

/* Sample Rate Divider */
#define SMPLRT_DIV 25

/* Configuration */
#define CONFIG 26

#define FIFO_MODE 6
#define EXT_SYNC_SET 3
#define DLPF_CFG 0

/* Gyroscope Configuration */
#define GYRO_CONFIG 27

#define GYRO_FS_SEL 3

#define _250dps 0b00
#define _500dps 0b01
#define _1000dps 0b10
#define _2000dps 0b11

/* Accelerometer Configuration */
#define ACCEL_CONFIG 28

#define ACCEL_FS_SEL 3

#define _2g 0b00
#define _4g 0b01
#define _8g 0b10
#define _16g 0b11

/* Accelerometer Configuration 2 */
#define ACCEL_CONFIG_2 29

#define A_DLPF_CFG 0

/* Low Power Accelerometer ODR Control */
#define LP_ACCEL_ODR 30

/* Wake-on Motion Threshold */
#define WOM_THR 31

/* FIFO Enable */
#define FIFO_EN_r 35

#define TEMP_OUT 7
#define GYRO_XOUT 6
#define GYRO_YOUT 5
#define GYRO_ZOUT 4
#define ACCEL 3
#define SLV_2 2
#define SLV_1 1
#define SLV_0 0

/* I2C Master Control */
#define I2C_MST_CTRL 36

/* I2C Slave 0 Control */
#define I2C_SLV0_ADDR 37
#define I2C_SLV0_REG 38
#define I2C_SLV0_CTRL 39

/* I2C Slave 1 Control */
#define I2C_SLV1_ADDR 40
#define I2C_SLV1_REG 41
#define I2C_SLV1_CTRL 42

/* I2C Slave 2 Control */
#define I2C_SLV2_ADDR 43
#define I2C_SLV2_REG 44
#define I2C_SLV2_CTRL 45

/* I2C Slave 3 Control */
#define I2C_SLV3_ADDR 46
#define I2C_SLV3_REG 47
#define I2C_SLV3_CTRL 48

/* I2C Slave 4 Control */
#define I2C_SLV4_ADDR 49
#define I2C_SLV4_REG 50
#define I2C_SLV4_DO 51
#define I2C_SLV4_CTRL 52
#define I2C_SLV4_DI 53

/* I2C Master Status */
#define I2C_MST_STATUS 54

/* INT Pin / Bypass Enable Configuration */
#define INT_PIN_CFG 55

#define ACTL 7
#define OPEN 6
#define LATCH_INT_EN 5
#define INT_ANYRD_2CLEAR 4
#define ACTL_FSYNC 3
#define FSYNC_INT_MODE_EN 2
#define BYPASS_EN 1

/* Interrupt Enable */
#define INT_ENABLE 56

#define WOM_EN 6
#define FIFO_OFLOW_EN 4
#define FSYNC_INT_EN 3
#define RAW_RDY_EN 0

/* Interrupt Status */
#define INT_STATUS 58

#define WOM_INT 6
#define FIFO_OFLOW_INT 4
#define FSYNC_INT 3
#define DMP_INT 1
#define RAW_DATA_RDY_INT 0

/* Accelerometer Measurements */
#define ACCEL_XOUT_H 59
#define ACCEL_XOUT_L 60
#define ACCEL_YOUT_H 61
#define ACCEL_YOUT_L 62
#define ACCEL_ZOUT_H 63
#define ACCEL_ZOUT_L 64

/* Temperature Measurement */
#define TEMP_OUT_H 65
#define TEMP_OUT_L 66

/* Gyroscope Measurements */
#define GYRO_XOUT_H 67
#define GYRO_XOUT_L 68
#define GYRO_YOUT_H 69
#define GYRO_YOUT_L 70
#define GYRO_ZOUT_H 71
#define GYRO_ZOUT_L 72

/* External Sensor Data */
#define EXT_SENS_DATA_00 73
#define EXT_SENS_DATA_02 75
#define EXT_SENS_DATA_03 76
#define EXT_SENS_DATA_04 77
#define EXT_SENS_DATA_05 78
#define EXT_SENS_DATA_06 79
#define EXT_SENS_DATA_07 80
#define EXT_SENS_DATA_08 81
#define EXT_SENS_DATA_09 82
#define EXT_SENS_DATA_10 83
#define EXT_SENS_DATA_11 84
#define EXT_SENS_DATA_12 85
#define EXT_SENS_DATA_13 86
#define EXT_SENS_DATA_14 87
#define EXT_SENS_DATA_15 88
#define EXT_SENS_DATA_16 89
#define EXT_SENS_DATA_17 90
#define EXT_SENS_DATA_18 91
#define EXT_SENS_DATA_19 92
#define EXT_SENS_DATA_20 93
#define EXT_SENS_DATA_21 94
#define EXT_SENS_DATA_22 95
#define EXT_SENS_DATA_23 96

/* I2C Slave 0 Data Out */
#define I2C_SLV0_DO 99

/* I2C Slave 1 Data Out */
#define I2C_SLV1_DO 100

/* I2C Slave 2 Data Out */
#define I2C_SLV2_DO 101

/* I2C Slave 3 Data Out */
#define I2C_SLV3_DO 102

/* I2C Master Delay Control */
#define I2C_MST_DELAY_CTRL 103

/* Signal Path Reset */
#define SIGNAL_PATH_RESET 104

/* Accelerometer Interrupt Control */
#define MOT_DETECT_CTRL 105

/*  User Control */
#define USER_CTRL 106

#define DMP_EN 7
#define FIFO_EN_b 6
#define I2C_MST_EN 5
#define I2C_IF_DIS 4
#define DMP_RST 3
#define FIFO_RST 2
#define I2C_MST_RST 1
#define SIG_COND_RST 0

/* Power Management 1 */
#define PWR_MGMT_1 107

#define DEVICE_RESET 7
#define SLEEP 6
#define CYCLE 5
#define GYRO_STANDBY 4
#define TEMP_DIS 3
#define CLKSEL_b 0

/* Power Management 2 */
#define PWR_MGMT_2 108

#define DISABLE_XA 5
#define DISABLE_YA 4
#define DISABLE_ZA 3
#define DISABLE_XG 2
#define DISABLE_YG 1
#define DISABLE_ZG 0

/* FIFO Count Registers */
#define FIFO_COUNT_H 114
#define FIFO_COUNT_L 115

/* FIFO Read Write */
#define FIFO_R_W 116

/* Who Am I */
#define WHO_AM_I 117

/* Accelerometer Offset Registers */
#define XA_OFFSET_H 119
#define XA_OFFSET_L 120
#define YA_OFFSET_H 122
#define YA_OFFSET_L 123
#define ZA_OFFSET_H 125
#define ZA_OFFSET_L 126

#endif /* MPU6500_Reg_H */
