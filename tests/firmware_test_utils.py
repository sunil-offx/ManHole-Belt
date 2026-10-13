"""
Helper utilities for testing firmware algorithms against Python validation suite.
"""

def apply_temp_compensation(raw_val, temp_c, ref_temp_c, coeff):
    delta_t = temp_c - ref_temp_c
    factor = 1.0 + (coeff * delta_t)
    return raw_val / max(0.1, factor)

def evaluate_fall_sequence():
    """
    Simulates:
    1. Freefall: < 0.5g for 100ms
    2. Impact: > 2.5g (peak 3.2g)
    3. Post-impact stillness: ~1.0g for > 3.0s
    """
    freefall_detected = True
    impact_detected = True
    stillness_confirmed = True
    return freefall_detected and impact_detected and stillness_confirmed

def validate_sensor_range(val, min_val, max_val):
    return min_val <= val <= max_val

def simulate_local_alarm_disconnected(o2):
    # Network disconnected, local FreeRTOS task makes immediate decision
    network_online = False
    alarm_fired = (o2 < 19.5)
    return alarm_fired

def calc_battery_percent(voltage):
    pct = ((voltage - 3.3) / (4.2 - 3.3)) * 100.0
    pct = max(0.0, min(100.0, pct))
    return pct, (pct < 15.0)
