/**
 * @file alert_manager.c
 * @brief Fail-Safe Local Alert System Manager implementation.
 */

#include "alert_manager.h"
#include "hal_gpio.h"
#include "hal_timer.h"
#include "oled_display.h"

static bool s_sos_latched = false;
static led_color_t s_current_led = LED_COLOR_OFF;
static buzzer_mode_t s_current_buzzer = BUZZER_SILENT;
static vibration_mode_t s_current_vib = VIB_OFF;

static void set_rgb_led(led_color_t color) {
    s_current_led = color;
    switch (color) {
        case LED_COLOR_GREEN:
            hal_gpio_set_level(PIN_LED_RED, HAL_GPIO_LOW);
            hal_gpio_set_level(PIN_LED_GREEN, HAL_GPIO_HIGH);
            hal_gpio_set_level(PIN_LED_BLUE, HAL_GPIO_LOW);
            break;
        case LED_COLOR_YELLOW:
            hal_gpio_set_level(PIN_LED_RED, HAL_GPIO_HIGH);
            hal_gpio_set_level(PIN_LED_GREEN, HAL_GPIO_HIGH);
            hal_gpio_set_level(PIN_LED_BLUE, HAL_GPIO_LOW);
            break;
        case LED_COLOR_RED:
            hal_gpio_set_level(PIN_LED_RED, HAL_GPIO_HIGH);
            hal_gpio_set_level(PIN_LED_GREEN, HAL_GPIO_LOW);
            hal_gpio_set_level(PIN_LED_BLUE, HAL_GPIO_LOW);
            break;
        case LED_COLOR_BLUE_SOS:
            hal_gpio_set_level(PIN_LED_RED, HAL_GPIO_HIGH);
            hal_gpio_set_level(PIN_LED_GREEN, HAL_GPIO_LOW);
            hal_gpio_set_level(PIN_LED_BLUE, HAL_GPIO_HIGH);
            break;
        case LED_COLOR_OFF:
        default:
            hal_gpio_set_level(PIN_LED_RED, HAL_GPIO_LOW);
            hal_gpio_set_level(PIN_LED_GREEN, HAL_GPIO_LOW);
            hal_gpio_set_level(PIN_LED_BLUE, HAL_GPIO_LOW);
            break;
    }
}

static void set_buzzer_and_vibration(buzzer_mode_t bz, vibration_mode_t vib) {
    s_current_buzzer = bz;
    s_current_vib = vib;

    switch (bz) {
        case BUZZER_SILENT:
            hal_gpio_set_pwm_duty(PIN_BUZZER_PWM, 0);
            break;
        case BUZZER_PATTERN_WARNING_BEEP:
            /* 50% duty intermittent beep */
            hal_gpio_set_pwm_duty(PIN_BUZZER_PWM, 50);
            break;
        case BUZZER_CONTINUOUS_CRITICAL:
        case BUZZER_SOS_SIREN:
            /* 90% duty continuous loud alarm */
            hal_gpio_set_pwm_duty(PIN_BUZZER_PWM, 90);
            break;
    }

    switch (vib) {
        case VIB_OFF:
            hal_gpio_set_level(PIN_VIBRATION, HAL_GPIO_LOW);
            break;
        case VIB_INTERMITTENT_PULSE:
            hal_gpio_set_level(PIN_VIBRATION, HAL_GPIO_HIGH);
            break;
        case VIB_CONTINUOUS_MAX:
            hal_gpio_set_level(PIN_VIBRATION, HAL_GPIO_HIGH);
            break;
    }
}

esp_err_t alert_manager_init(void) {
    hal_gpio_init_output(PIN_LED_RED);
    hal_gpio_init_output(PIN_LED_GREEN);
    hal_gpio_init_output(PIN_LED_BLUE);
    hal_gpio_init_output(PIN_BUZZER_PWM);
    hal_gpio_init_output(PIN_VIBRATION);
    hal_gpio_init_input(PIN_SOS_BUTTON, true);

    set_rgb_led(LED_COLOR_GREEN);
    set_buzzer_and_vibration(BUZZER_SILENT, VIB_OFF);
    s_sos_latched = false;
    return ESP_OK;
}

void alert_manager_trigger_sos(void) {
    s_sos_latched = true;
    set_rgb_led(LED_COLOR_BLUE_SOS);
    set_buzzer_and_vibration(BUZZER_SOS_SIREN, VIB_CONTINUOUS_MAX);
}

bool alert_manager_is_sos_active(void) {
    /* Also check hardware button */
    if (hal_gpio_get_level(PIN_SOS_BUTTON) == HAL_GPIO_LOW) {
        s_sos_latched = true;
    }
    return s_sos_latched;
}

void alert_manager_clear_sos(void) {
    s_sos_latched = false;
    set_rgb_led(LED_COLOR_GREEN);
    set_buzzer_and_vibration(BUZZER_SILENT, VIB_OFF);
}

void alert_manager_update(risk_status_t status,
                          const risk_assessment_t *assessment,
                          const multi_sensor_snapshot_t *sensors) {
    if (alert_manager_is_sos_active()) {
        alert_manager_trigger_sos();
        oled_show_critical(assessment ? assessment->composite_score : 100.0f, sensors);
        return;
    }

    switch (status) {
        case RISK_SAFE:
            set_rgb_led(LED_COLOR_GREEN);
            set_buzzer_and_vibration(BUZZER_SILENT, VIB_OFF);
            oled_show_safe(assessment ? assessment->composite_score : 0.0f, sensors);
            break;

        case RISK_WARNING:
            set_rgb_led(LED_COLOR_YELLOW);
            set_buzzer_and_vibration(BUZZER_PATTERN_WARNING_BEEP, VIB_INTERMITTENT_PULSE);
            oled_show_warning(assessment ? assessment->composite_score : 45.0f, sensors);
            break;

        case RISK_CRITICAL:
        case RISK_EMERGENCY_SOS:
        default:
            set_rgb_led(LED_COLOR_RED);
            set_buzzer_and_vibration(BUZZER_CONTINUOUS_CRITICAL, VIB_CONTINUOUS_MAX);
            oled_show_critical(assessment ? assessment->composite_score : 90.0f, sensors);
            break;
    }
}
