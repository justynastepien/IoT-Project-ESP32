#include "mjd.h"
#include "mjd_bmp280.h"


static const char TAG[] = "mjd_bmp280";

;
static uint32_t bmp280_current_i2c_port;


void BMP280_delay_millisec(u32 millisec) {
    
    sys_delay_ms(millisec * 1000);
}


s8 BMP280_I2C_bus_write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt) {
    s32 f_retval = BMP280_INIT_VALUE;
    esp_err_t esp_retval;


    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg_addr, true);
    i2c_master_write(cmd, reg_data, cnt, true);
    i2c_master_stop(cmd);
    esp_retval = i2c_master_cmd_begin(bmp280_current_i2c_port, cmd, RTOS_DELAY_100MILLISEC);
    if (esp_retval == ESP_OK) {
        f_retval = SUCCESS;  
    } else {
        ESP_LOGE(TAG, "ERROR. BMP280_I2C_bus_write(): i2c_master_cmd_begin() err %i", esp_retval);
        f_retval = ERROR;  
    }
    i2c_cmd_link_delete(cmd);



    return (s8) f_retval;
}


s8 BMP280_I2C_bus_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt) {
    s32 f_retval = BMP280_INIT_VALUE;
    esp_err_t esp_retval;


    i2c_cmd_handle_t cmd;

    cmd = i2c_cmd_link_create();

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg_addr, true);

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_READ, true);
    if (cnt > 1) {
        i2c_master_read(cmd, reg_data, cnt - 1, I2C_MASTER_ACK);
    }
    i2c_master_read_byte(cmd, reg_data + cnt - 1, I2C_MASTER_NACK);
    i2c_master_stop(cmd);

    esp_retval = i2c_master_cmd_begin(bmp280_current_i2c_port, cmd, RTOS_DELAY_100MILLISEC);
    if (esp_retval == ESP_OK) {
        f_retval = SUCCESS; 
    } else {
        ESP_LOGE(TAG, "ERROR. BMP280_I2C_bus_write(): 2nd i2c_master_cmd_begin() err %i", esp_retval);
        f_retval = ERROR;  
    }
    i2c_cmd_link_delete(cmd);



    return (s8) f_retval;
}



esp_err_t mjd_bmp280_init(const mjd_bmp280_config_t* config) {
    ESP_LOGD(TAG, "%s()", __FUNCTION__);

    esp_err_t f_retval = ESP_OK;
    i2c_cmd_handle_t cmd;
    s32 com_rslt;

    if (config->manage_i2c_driver == true) {

        i2c_config_t i2c_conf = { 0 };
        i2c_conf.mode = I2C_MODE_MASTER;
        i2c_conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
        i2c_conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
        i2c_conf.master.clk_speed = BMP280_I2C_MASTER_FREQ_HZ;
        i2c_conf.scl_io_num = config->i2c_scl_gpio_num;
        i2c_conf.sda_io_num = config->i2c_sda_gpio_num;

        f_retval = i2c_param_config(config->i2c_port_num, &i2c_conf);
        if (f_retval != ESP_OK) {
            ESP_LOGE(TAG, "ABORT. i2c_param_config() error (%i)", f_retval);

            goto cleanup;
        }

        f_retval = i2c_driver_install(config->i2c_port_num, I2C_MODE_MASTER, BMP280_I2C_MASTER_RX_BUF_DISABLE,
        BMP280_I2C_MASTER_TX_BUF_DISABLE,
        BMP280_I2C_MASTER_INTR_FLAG_NONE);
        if (f_retval != ESP_OK) {
            ESP_LOGE(TAG, "ABORT. i2c_driver_install() error (%i)", f_retval);

            goto cleanup;
        }
    }


    bmp280_current_i2c_port = config->i2c_port_num;

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (config->i2c_slave_addr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_stop(cmd);
    f_retval = i2c_master_cmd_begin(config->i2c_port_num, cmd, RTOS_DELAY_1SEC);
    if (f_retval != ESP_OK) {
        ESP_LOGE(TAG, "ABORT. i2c_master_cmd_begin() I2C slave NOT working or wrong I2C slave address - error (%i)", f_retval);

        goto cleanup;
    }
    i2c_cmd_link_delete(cmd);


    struct bmp280_t bmp280 = { .bus_write = BMP280_I2C_bus_write, .bus_read = BMP280_I2C_bus_read, .delay_msec =
            BMP280_delay_millisec, .dev_addr = config->i2c_slave_addr };
    com_rslt = bmp280_init(&bmp280);
    ESP_LOGD(TAG, "  chip id: 0x%hhx (d %hhu) [EXPECT 0x56 or 0x57 or 0x58 for sensor BMP280]", bmp280.chip_id, bmp280.chip_id);
    if (com_rslt != SUCCESS) {
        ESP_LOGE(TAG, "ABORT. bmp280_init() failed err %i", com_rslt);
        f_retval = ESP_FAIL;

        goto cleanup;
    }


    cleanup:;

    return f_retval;
}

esp_err_t mjd_bmp280_deinit(const mjd_bmp280_config_t* config) {
    ESP_LOGD(TAG, "%s()", __FUNCTION__);

    esp_err_t f_retval = ESP_OK;


    if (config->manage_i2c_driver == true) {
        f_retval = i2c_driver_delete(config->i2c_port_num);
        if (f_retval != ESP_OK) {
            ESP_LOGE(TAG, "ABORT. i2c_driver_delete() error (%i)", f_retval);

            goto cleanup;
        }
    }

    cleanup:;

    return f_retval;
}

esp_err_t mjd_bmp280_read_forced(const mjd_bmp280_config_t* config, mjd_bmp280_data_t* data) {

    ESP_LOGD(TAG, "%s()", __FUNCTION__);

    esp_err_t f_retval = ESP_OK;
    s32 com_rslt;
    u8 filter_coeff = 255;
    u8 power_mode = 255;


    data->pressure_hpascal = 0.0;
    data->temperature_celsius = 0.0;


    ESP_LOGD(TAG, "do bmp280_init()");

    struct bmp280_t bmp280 = { .bus_write = BMP280_I2C_bus_write, .bus_read = BMP280_I2C_bus_read, .delay_msec =
            BMP280_delay_millisec, .dev_addr = config->i2c_slave_addr };
    com_rslt = bmp280_init(&bmp280);
    ESP_LOGD(TAG, "  chip id: 0x%hhx (d %hhu) [EXPECT 0x56 or 0x57 or 0x58 for sensor BMP280]", bmp280.chip_id, bmp280.chip_id);
    if (com_rslt != SUCCESS) {
        ESP_LOGE(TAG, "ABORT. bmp280_init() failed err %i", com_rslt);
        f_retval = ESP_FAIL;

        goto cleanup;
    }

   
    ESP_LOGD(TAG, "do bmp280_get_power_mode()");

    com_rslt = bmp280_get_power_mode(&power_mode);
    ESP_LOGD(TAG, "  power_mode: 0x%hhx (d %hhu) [EXPECT 0x00 BMP280_SLEEP_MODE]", power_mode, power_mode);
    if (com_rslt != SUCCESS) {
        ESP_LOGE(TAG, "ABORT. bmp280_get_power_mode() failed err %i", com_rslt);
        f_retval = ESP_FAIL;
        // LABEL
        goto cleanup;
    }


     
    ESP_LOGD(TAG, "do bmp280_set_work_mode() [config->bmp280_work_mode = 0x%x (d %u)]", config->bmp280_work_mode,
            config->bmp280_work_mode);

    com_rslt = bmp280_set_work_mode(config->bmp280_work_mode);
    if (com_rslt != SUCCESS) {
        ESP_LOGE(TAG, "ABORT. bmp280_set_work_mode() [config->bmp280_work_mode = 0x%x (d %u)] failed err %i",
                config->bmp280_work_mode, config->bmp280_work_mode, com_rslt);
        f_retval = ESP_FAIL;
        // LABEL
        goto cleanup;
    }


    ESP_LOGD(TAG, "do bmp280_set_filter() [config->bmp280_filter_coefficient = 0x%x (d %u)]",
            config->bmp280_filter_coefficient, config->bmp280_filter_coefficient);

    com_rslt = bmp280_set_filter(config->bmp280_filter_coefficient);
    if (com_rslt != SUCCESS) {
        ESP_LOGE(TAG, "ABORT. bmp280_set_filter() [config->bmp280_filter_coefficient = 0x%x (d %u)] failed err %i",
                config->bmp280_filter_coefficient, config->bmp280_filter_coefficient, com_rslt);
        f_retval = ESP_FAIL;
        // LABEL
        goto cleanup;
    }

    ESP_LOGD(TAG, "do bmp280_set_power_mode(BMP280_FORCED_MODE)");

    com_rslt = bmp280_set_power_mode(BMP280_FORCED_MODE);
    if (com_rslt != SUCCESS) {
        ESP_LOGE(TAG, "ABORT. bmp280_set_power_mode(BMP280_FORCED_MODE) failed err %i", com_rslt);
        f_retval = ESP_FAIL;
        // LABEL
        goto cleanup;
    }


    ESP_LOGD(TAG, "do bmp280_get_filter()");

    com_rslt = bmp280_get_filter(&filter_coeff);
    ESP_LOGD(TAG,
            "  filter_coeff: 0x%hhx (d %hhu) [EXPECT 0x00 BMP280_FILTER_COEFF_OFF for work mode BMP280_ULTRA_LOW_POWER_MODE]",
            filter_coeff, filter_coeff);
    if (com_rslt != SUCCESS) {
        ESP_LOGE(TAG, "ABORT. bmp280_get_filter() failed err %i", com_rslt);
        f_retval = ESP_FAIL;

        goto cleanup;
    }


    ESP_LOGD(TAG, "do bmp280_get_power_mode()");

    com_rslt = bmp280_get_power_mode(&power_mode);
    ESP_LOGD(TAG, "  power_mode: 0x%hhx (d %hhu) [EXPECT 0x01 or 0x02 BMP280_FORCED_MODE]", power_mode, power_mode);
    if (com_rslt != SUCCESS) {
        ESP_LOGE(TAG, "ABORT. bmp280_get_power_mode() failed err %i", com_rslt);
        f_retval = ESP_FAIL;

        goto cleanup;
    }

    vTaskDelay(RTOS_DELAY_50MILLISEC);


    ESP_LOGD(TAG, "do bmp280_get_forced_uncomp_pressure_temperature()");

    s32 v_uncomp_pressure_s32;
    s32 v_uncomp_temperature_s32;

    com_rslt = bmp280_get_forced_uncomp_pressure_temperature(&v_uncomp_pressure_s32, &v_uncomp_temperature_s32);
    if (com_rslt != SUCCESS) {
        ESP_LOGE(TAG, "ABORT. bmp280_get_forced_uncomp_pressure_temperature() failed err %i", com_rslt);
        f_retval = ESP_FAIL;

        goto cleanup;
    }

    
    data->pressure_hpascal = bmp280_compensate_pressure_double(v_uncomp_pressure_s32) / 100;
    data->temperature_celsius = bmp280_compensate_temperature_double(v_uncomp_temperature_s32);

 
    ESP_LOGD(TAG, "do bmp280_set_power_mode(BMP280_SLEEP_MODE)");
    com_rslt = bmp280_set_power_mode(BMP280_SLEEP_MODE);
    if (com_rslt != SUCCESS) {
        ESP_LOGE(TAG, "ABORT. bmp280_set_power_mode(BMP280_SLEEP_MODE) failed err %i", com_rslt);
        f_retval = ESP_FAIL;

        goto cleanup;
    }

 
    ESP_LOGD(TAG, "do bmp280_get_power_mode()");

    com_rslt = bmp280_get_power_mode(&power_mode);
    ESP_LOGD(TAG, "  power_mode: 0x%hhx (d %hhu) [EXPECT 0x00 BMP280_SLEEP_MODE]", power_mode, power_mode);
    if (com_rslt != SUCCESS) {
        ESP_LOGE(TAG, "ABORT. bmp280_get_power_mode() failed err %i", com_rslt);
        f_retval = ESP_FAIL;

        goto cleanup;
    }


    cleanup:;

    return f_retval;
}