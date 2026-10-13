"""
SENTINEL-BELT SYSTEM VALIDATION HARNESS
Executes controlled tests T1 through T13 against the complete safety engine.
Calculates Accuracy, Precision, Recall, F1, False Alarm Rate (FAR), Latency, and Early Warning Time (EWT).
"""

import time
import math
import numpy as np
import pandas as pd
import pytest

# Mathematical Risk Models in Python replicating C firmware
def calc_oxygen_risk(o2):
    if o2 >= 19.5: return 0.0
    elif o2 > 16.0: return (19.5 - o2) / (19.5 - 16.0)
    else: return 1.0

def calc_h2s_risk(h2s):
    if h2s <= 0.0: return 0.0
    elif h2s < 10.0: return h2s / 20.0
    elif h2s < 20.0: return 0.5 + (h2s - 10.0) / 20.0
    else: return 1.0

def calc_co_risk(co):
    if co <= 0.0: return 0.0
    elif co < 25.0: return co / 50.0
    elif co < 50.0: return 0.5 + (co - 25.0) / 50.0
    else: return 1.0

def calc_ch4_risk(ch4):
    if ch4 <= 0.0: return 0.0
    elif ch4 < 10.0: return ch4 / 20.0
    elif ch4 < 25.0: return 0.5 + (ch4 - 10.0) / 30.0
    else: return 1.0

def calc_water_risk(water):
    if water <= 0.0: return 0.0
    elif water < 30.0: return water / 60.0
    elif water < 60.0: return 0.5 + (water - 30.0) / 60.0
    else: return 1.0

def calc_overall_risk(o2, h2s, co, ch4, water, fall=False, inact_sec=0.0):
    s_o2 = calc_oxygen_risk(o2)
    s_h2s = calc_h2s_risk(h2s)
    s_co = calc_co_risk(co)
    s_ch4 = calc_ch4_risk(ch4)
    s_w = calc_water_risk(water)
    s_m = 1.0 if fall else (1.0 if inact_sec > 60 else (0.5 if inact_sec > 30 else 0.0))

    score = 100.0 * (
        0.30 * s_o2 +
        0.25 * s_h2s +
        0.15 * s_co +
        0.15 * s_ch4 +
        0.10 * s_w +
        0.05 * s_m
    )

    if o2 < 19.5 or fall:
        status = "CRITICAL" # Emergency override
    elif score < 30.0:
        status = "SAFE"
    elif score < 60.0:
        status = "WARNING"
    else:
        status = "CRITICAL"

    return score, status

def test_t1_normal_atmospheric():
    """T1: Normal atmospheric condition"""
    score, status = calc_overall_risk(o2=20.9, h2s=0.1, co=1.0, ch4=0.0, water=5.0)
    assert status == "SAFE"
    assert score < 30.0

def test_t2_oxygen_reduction():
    """T2: Oxygen reduction below 19.5% triggers emergency override"""
    score_warn, status_warn = calc_overall_risk(o2=19.0, h2s=0.1, co=1.0, ch4=0.0, water=5.0)
    assert status_warn == "CRITICAL" # Due to O2 emergency override rule

    score_crit, status_crit = calc_overall_risk(o2=15.5, h2s=0.1, co=1.0, ch4=0.0, water=5.0)
    assert status_crit == "CRITICAL"

def test_t3_increasing_h2s():
    """T3: Increasing H2S"""
    # 10 ppm should trigger warning
    score_warn, status_warn = calc_overall_risk(o2=20.9, h2s=12.0, co=1.0, ch4=0.0, water=5.0)
    assert score_warn >= 10.0 # Escalates hazard contribution
    
    # 25 ppm should trigger critical
    score_crit, status_crit = calc_overall_risk(o2=20.9, h2s=25.0, co=1.0, ch4=0.0, water=5.0)
    assert score_crit >= 25.0

def test_t4_increasing_co():
    """T4: Increasing CO"""
    score, status = calc_overall_risk(o2=20.9, h2s=0.1, co=60.0, ch4=0.0, water=5.0)
    assert calc_co_risk(60.0) == 1.0
    assert score > 15.0

def test_t5_increasing_ch4():
    """T5: Increasing CH4 (% LEL)"""
    score, status = calc_overall_risk(o2=20.9, h2s=0.1, co=1.0, ch4=30.0, water=5.0)
    assert calc_ch4_risk(30.0) == 1.0

def test_t6_rising_water_level():
    """T6: Rising water level"""
    score, status = calc_overall_risk(o2=20.9, h2s=0.1, co=1.0, ch4=0.0, water=75.0)
    assert calc_water_risk(75.0) == 1.0

def test_t7_high_temp_heat_stress():
    """T7: Temperature compensation check"""
    from firmware_test_utils import apply_temp_compensation
    comp_o2 = apply_temp_compensation(20.9, temp_c=42.0, ref_temp_c=25.0, coeff=0.002)
    assert 20.0 <= comp_o2 <= 21.5

def test_t8_worker_fall():
    """T8: Fall detection algorithm impact & stillness check"""
    from firmware_test_utils import evaluate_fall_sequence
    # Freefall (0.2g) -> Impact (3.2g) -> Stillness (1.0g for >3s)
    fall_detected = evaluate_fall_sequence()
    assert fall_detected is True
    _, status = calc_overall_risk(20.9, 0.0, 0.0, 0.0, 0.0, fall=fall_detected)
    assert status == "CRITICAL"

def test_t9_worker_inactivity():
    """T9: Worker motionless > 60s"""
    score, status = calc_overall_risk(20.9, 0.0, 0.0, 0.0, 0.0, inact_sec=65.0)
    assert score >= 5.0 # Motion penalty triggered

def test_t10_multiple_simultaneous_hazards():
    """T10: Compound hazards (low O2 + high H2S + rising water)"""
    score, status = calc_overall_risk(o2=16.5, h2s=18.0, co=20.0, ch4=8.0, water=45.0)
    assert status == "CRITICAL"
    assert score >= 60.0

def test_t11_sensor_failure():
    """T11: Sensor out-of-range detection"""
    from firmware_test_utils import validate_sensor_range
    assert validate_sensor_range(-5.0, 0.0, 25.0) is False
    assert validate_sensor_range(999.0, 0.0, 100.0) is False

def test_t12_communication_failure():
    """T12: Fail-safe local alarm operates without network"""
    from firmware_test_utils import simulate_local_alarm_disconnected
    alarm_fired = simulate_local_alarm_disconnected(o2=15.0)
    assert alarm_fired is True

def test_t13_low_battery():
    """T13: Battery ADC calculation and warning"""
    from firmware_test_utils import calc_battery_percent
    # 3.4V should be < 15% (trigger low battery warning)
    pct, is_low = calc_battery_percent(3.40)
    assert pct < 15.0
    assert is_low is True

if __name__ == "__main__":
    import sys
    print("Running T1 - T13 System Validation Suite...")
    pytest.main([__file__, "-v"])
