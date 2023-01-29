#ifndef __MJD_BMP280_H__
#define __MJD_BMP280_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "bmp280.h"


#define BMP280_I2C_MASTER_FREQ_HZ         (100 * 1000)  
#define BMP280_I2C_MASTER_RX_BUF_DISABLE  (0)           
#define BMP280_I2C_MASTER_TX_BUF_DISABLE  (0)          
#define BMP280_I2C_MASTER_INTR_FLAG_NONE  (0)


#define BMP280_I2C_BUFFER_LEN (8)
#define BMP280_DATA_INDEX     (1)
#define BMP280_ADDRESS_INDEX  (2)


typedef struct {
    bool manage_i2c_driver;
    i2c_port_t i2c_port_num;
    uint8_t i2c_slave_addr;
    gpio_num_t i2c_scl_gpio_num;
    gpio_num_t i2c_sda_gpio_num;
    u8 bmp280_work_mode;
    u8 bmp280_filter_coefficient;
} mjd_bmp280_config_t;


#define MJD_BMP280_CONFIG_DEFAULT() { \
    .manage_i2c_driver = true,  \
    .i2c_port_num = I2C_NUM_0,  \
    .i2c_slave_addr = 0x76,  \
    .bmp280_work_mode = BMP280_ULTRA_LOW_POWER_MODE,  \
    .bmp280_filter_coefficient = BMP280_FILTER_COEFF_OFF  \
};

typedef struct {
    double pressure_hpascal;
    double temperature_celsius;
} mjd_bmp280_data_t;


esp_err_t mjd_bmp280_init(const mjd_bmp280_config_t* config);
esp_err_t mjd_bmp280_deinit(const mjd_bmp280_config_t* config);
esp_err_t mjd_bmp280_read_forced(const mjd_bmp280_config_t* config, mjd_bmp280_data_t* data);

#ifdef __cplusplus
}
#endif

#endif 