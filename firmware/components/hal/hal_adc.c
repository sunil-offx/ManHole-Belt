/**
 * @file hal_adc.c
 * @brief ADC Hardware Abstraction Layer implementation.
 */

#include "hal_adc.h"

#ifdef ESP_PLATFORM
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

static adc_oneshot_unit_handle_t adc1_handle = NULL;

esp_err_t hal_adc_init(void) {
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    esp_err_t err = adc_oneshot_new_unit(&init_config1, &adc1_handle);
    if (err != ESP_OK) return err;

    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_12,
    };
    for (int i = 0; i <= 4; i++) {
        adc_oneshot_config_channel(adc1_handle, (adc_channel_t)i, &config);
    }
    return ESP_OK;
}

esp_err_t hal_adc_read_raw(hal_adc_channel_t channel, int *raw_value) {
    if (!adc1_handle || !raw_value) return ESP_ERR_INVALID_STATE;
    return adc_oneshot_read(adc1_handle, (adc_channel_t)channel, raw_value);
}

esp_err_t hal_adc_read_voltage(hal_adc_channel_t channel, float *voltage_volts) {
    int raw = 0;
    esp_err_t err = hal_adc_read_raw(channel, &raw);
    if (err != ESP_OK) return err;
    /* Approximate voltage for 12-bit ADC with 3.3V full scale */
    *voltage_volts = ((float)raw / 4095.0f) * 3.3f;
    return ESP_OK;
}

#else
/* Native host mock values */
static float mock_voltages[5] = {
    1.65f, /* O2 in normal air (~20.9%) */
    0.05f, /* H2S clean air */
    0.10f, /* CO clean air */
    0.02f, /* CH4 clean air */
    3.85f  /* Battery ~70% */
};

esp_err_t hal_adc_init(void) {
    return ESP_OK;
}

esp_err_t hal_adc_read_raw(hal_adc_channel_t channel, int *raw_value) {
    if (!raw_value || channel < 0 || channel > 4) return ESP_ERR_INVALID_ARG;
    *raw_value = (int)((mock_voltages[channel] / 3.3f) * 4095.0f);
    return ESP_OK;
}

esp_err_t hal_adc_read_voltage(hal_adc_channel_t channel, float *voltage_volts) {
    if (!voltage_volts || channel < 0 || channel > 4) return ESP_ERR_INVALID_ARG;
    *voltage_volts = mock_voltages[channel];
    return ESP_OK;
}

/* Helper for host simulation tests to inject voltages */
void hal_adc_mock_set_voltage(hal_adc_channel_t channel, float voltage) {
    if (channel >= 0 && channel <= 4) {
        mock_voltages[channel] = voltage;
    }
}
#endif
