"""
SENTINEL-BELT AI/ML Training Pipeline
Dataset Generator for Confined-Space Scenarios (T1 - T13)

Generates synthetic and physics-modeled multi-sensor datasets reflecting real-world
manhole and underground sewer conditions, gas dynamics, worker physical activity,
slips, falls, water ingress, and sensor failures.
"""

import numpy as np
import pandas as pd
import random
import os

np.random.seed(42)
random.seed(42)

def generate_sensor_reading(base_o2=20.9, base_h2s=0.0, base_co=0.0, base_ch4=0.0,
                            base_temp=24.0, base_hum=65.0, base_water=5.0,
                            ax=0.05, ay=0.02, az=0.98, noise_scale=1.0):
    """Adds realistic sensor Gaussian noise and discretization."""
    o2 = np.clip(base_o2 + np.random.normal(0, 0.05 * noise_scale), 0.0, 25.0)
    h2s = max(0.0, base_h2s + np.random.normal(0, 0.2 * noise_scale))
    co = max(0.0, base_co + np.random.normal(0, 0.5 * noise_scale))
    ch4 = max(0.0, base_ch4 + np.random.normal(0, 0.2 * noise_scale))
    temp = base_temp + np.random.normal(0, 0.1 * noise_scale)
    hum = np.clip(base_hum + np.random.normal(0, 0.5 * noise_scale), 10.0, 100.0)
    water = np.clip(base_water + np.random.normal(0, 0.3 * noise_scale), 0.0, 100.0)
    
    acc_x = ax + np.random.normal(0, 0.02 * noise_scale)
    acc_y = ay + np.random.normal(0, 0.02 * noise_scale)
    acc_z = az + np.random.normal(0, 0.02 * noise_scale)
    
    return [o2, h2s, co, ch4, temp, hum, water, acc_x, acc_y, acc_z]

def generate_synthetic_dataset(num_samples_per_scenario=400):
    """
    Generates time-series samples across all 13 standard test scenarios:
    T1: Normal atmospheric condition
    T2: Oxygen reduction (displacement / microbial consumption)
    T3: Increasing H2S (sewer gas release)
    T4: Increasing CO (engine exhaust / smoldering)
    T5: Increasing CH4 (pipeline leak / anaerobic digestion)
    T6: Rising water level (flash flood / pipe burst)
    T7: High temperature & humidity (heat stress)
    T8: Worker fall (slip, sudden drop, impact)
    T9: Worker inactivity / unconsciousness
    T10: Multiple simultaneous hazards (Compound: low O2 + high H2S + rising water)
    T11: Sensor failure (erratic out-of-range or frozen reading)
    T12: Communication failure (local processing validation)
    T13: Low battery condition
    """
    data = []
    
    scenarios = [
        "T1_NORMAL", "T2_O2_DEPLETION", "T3_H2S_SURGE", "T4_CO_SURGE",
        "T5_CH4_SURGE", "T6_WATER_RISE", "T7_HEAT_STRESS", "T8_FALL_EVENT",
        "T9_INACTIVITY", "T10_COMPOUND_HAZARDS", "T11_SENSOR_FAULT",
        "T12_COMM_FAULT", "T13_LOW_BATTERY"
    ]
    
    sample_id = 0
    
    for scenario in scenarios:
        # State trajectories over time
        prev_o2 = 20.9
        prev_h2s = 0.0
        prev_co = 0.0
        prev_ch4 = 0.0
        
        for step in range(num_samples_per_scenario):
            t_norm = step / float(num_samples_per_scenario)
            
            # Default normal baselines
            o2 = 20.9
            h2s = 0.1
            co = 1.0
            ch4 = 0.0
            temp = 24.0 + 2.0 * np.sin(t_norm * np.pi)
            hum = 65.0
            water = 5.0
            ax, ay, az = 0.05, 0.02, 0.98
            fall_flag = 0
            inactivity_flag = 0
            risk_label = "SAFE"
            anomaly_label = 0  # 0: normal, 1: anomaly
            
            if scenario == "T1_NORMAL":
                # Healthy worker walking/working
                ax = 0.1 * np.sin(step * 0.2)
                ay = 0.1 * np.cos(step * 0.2)
                az = 0.98 + 0.15 * np.sin(step * 0.4)
                risk_label = "SAFE"
                anomaly_label = 0
                
            elif scenario == "T2_O2_DEPLETION":
                # Linear and exponential depletion of oxygen down to 14.5%
                o2 = max(14.5, 20.9 - 6.5 * (t_norm ** 1.3))
                if o2 < 16.0:
                    risk_label = "CRITICAL"
                elif o2 < 19.5:
                    risk_label = "WARNING"
                else:
                    risk_label = "SAFE"
                anomaly_label = 1 if o2 < 19.5 else 0
                
            elif scenario == "T3_H2S_SURGE":
                # Sudden sewer gas release (0 to 35 ppm)
                h2s = 0.1 + 35.0 * (t_norm ** 2.0)
                if h2s >= 20.0:
                    risk_label = "CRITICAL"
                elif h2s >= 10.0:
                    risk_label = "WARNING"
                else:
                    risk_label = "SAFE"
                anomaly_label = 1 if h2s >= 10.0 else 0
                
            elif scenario == "T4_CO_SURGE":
                # CO buildup from 0 to 120 ppm
                co = 1.0 + 120.0 * (t_norm ** 1.5)
                if co >= 50.0:
                    risk_label = "CRITICAL"
                elif co >= 25.0:
                    risk_label = "WARNING"
                else:
                    risk_label = "SAFE"
                anomaly_label = 1 if co >= 25.0 else 0
                
            elif scenario == "T5_CH4_SURGE":
                # Methane build-up (% LEL: 0 to 60%)
                ch4 = 0.0 + 60.0 * (t_norm ** 1.8)
                if ch4 >= 25.0:
                    risk_label = "CRITICAL"
                elif ch4 >= 10.0:
                    risk_label = "WARNING"
                else:
                    risk_label = "SAFE"
                anomaly_label = 1 if ch4 >= 10.0 else 0
                
            elif scenario == "T6_WATER_RISE":
                # Water level rises from 5% to 85%
                water = 5.0 + 80.0 * t_norm
                if water >= 60.0:
                    risk_label = "CRITICAL"
                elif water >= 30.0:
                    risk_label = "WARNING"
                else:
                    risk_label = "SAFE"
                anomaly_label = 1 if water >= 30.0 else 0
                
            elif scenario == "T7_HEAT_STRESS":
                # Extreme temperature and humidity
                temp = 24.0 + 20.0 * t_norm
                hum = 65.0 + 30.0 * t_norm
                if temp >= 38.0 and hum >= 80.0:
                    risk_label = "WARNING"
                    anomaly_label = 1
                else:
                    risk_label = "SAFE"
                    anomaly_label = 0
                    
            elif scenario == "T8_FALL_EVENT":
                # Free-fall followed by impact, then motionless
                if step < 150:
                    # Normal walking
                    ax = 0.1 * np.sin(step * 0.3)
                    ay = 0.1 * np.cos(step * 0.3)
                    az = 0.98
                    risk_label = "SAFE"
                elif 150 <= step < 156:
                    # Free fall: magnitude < 0.5g
                    ax, ay, az = 0.1, 0.1, 0.2
                    risk_label = "WARNING"
                    anomaly_label = 1
                elif 156 <= step < 165:
                    # Impact: magnitude > 2.8g
                    ax, ay, az = 1.8, 1.5, 2.2
                    fall_flag = 1
                    risk_label = "CRITICAL"
                    anomaly_label = 1
                else:
                    # Worker lying stationary at altered angle (inactivity)
                    ax, ay, az = 0.75, 0.65, 0.12
                    fall_flag = 1
                    risk_label = "CRITICAL"
                    anomaly_label = 1
                    
            elif scenario == "T9_INACTIVITY":
                # Complete lack of movement for extended duration
                ax, ay, az = 0.01, 0.01, 0.99
                if step > 200:
                    inactivity_flag = 1
                    risk_label = "WARNING"
                    anomaly_label = 1
                else:
                    risk_label = "SAFE"
                    
            elif scenario == "T10_COMPOUND_HAZARDS":
                # Multiple hazards: O2 drops, H2S climbs, water rises simultaneously
                o2 = max(15.0, 20.9 - 5.0 * t_norm)
                h2s = 0.1 + 18.0 * t_norm
                water = 5.0 + 45.0 * t_norm
                risk_label = "CRITICAL"
                anomaly_label = 1
                
            elif scenario == "T11_SENSOR_FAULT":
                # Sensor error: negative O2 reading or saturated spike
                if step > 100 and step % 40 < 10:
                    o2 = -5.0 # Out of range fault
                elif step > 250:
                    h2s = 999.0 # Saturation fault
                risk_label = "WARNING"
                anomaly_label = 1
                
            elif scenario == "T12_COMM_FAULT":
                # Comm failure scenario: environmental values vary but local processing continues
                o2 = 18.5 # warning level
                risk_label = "WARNING"
                anomaly_label = 0
                
            elif scenario == "T13_LOW_BATTERY":
                # Low battery simulation with normal readings
                risk_label = "SAFE"
                anomaly_label = 0

            # Compute rates of change (sampling delta dt = 1.0 sec for dataset)
            do2_dt = (o2 - prev_o2) / 1.0
            dh2s_dt = (h2s - prev_h2s) / 1.0
            dco_dt = (co - prev_co) / 1.0
            dch4_dt = (ch4 - prev_ch4) / 1.0
            
            prev_o2 = o2
            prev_h2s = h2s
            prev_co = co
            prev_ch4 = ch4
            
            # Apply sensor noise
            s = generate_sensor_reading(o2, h2s, co, ch4, temp, hum, water, ax, ay, az)
            
            # Vector: [O2, H2S, CO, CH4, T, RH, W, Ax, Ay, Az, dO2/dt, dH2S/dt, dCO/dt, dCH4/dt]
            features = [
                s[0], s[1], s[2], s[3], s[4], s[5], s[6],
                s[7], s[8], s[9],
                do2_dt, dh2s_dt, dco_dt, dch4_dt
            ]
            
            data.append({
                "sample_id": sample_id,
                "scenario": scenario,
                "step": step,
                "O2": features[0],
                "H2S": features[1],
                "CO": features[2],
                "CH4": features[3],
                "Temp": features[4],
                "Humidity": features[5],
                "WaterLevel": features[6],
                "Ax": features[7],
                "Ay": features[8],
                "Az": features[9],
                "dO2_dt": features[10],
                "dH2S_dt": features[11],
                "dCO_dt": features[12],
                "dCH4_dt": features[13],
                "fall_flag": fall_flag,
                "inactivity_flag": inactivity_flag,
                "risk_label": risk_label,
                "anomaly_label": anomaly_label
            })
            sample_id += 1
            
    df = pd.DataFrame(data)
    return df

if __name__ == "__main__":
    out_dir = os.path.dirname(os.path.abspath(__file__))
    os.makedirs(out_dir, exist_ok=True)
    csv_path = os.path.join(out_dir, "sentinel_dataset.csv")
    print(f"Generating synthetic multi-sensor dataset across scenarios T1 - T13...")
    df = generate_synthetic_dataset(num_samples_per_scenario=350)
    df.to_csv(csv_path, index=False)
    print(f"Saved {len(df)} samples to {csv_path}")
    print("Class distribution:")
    print(df["risk_label"].value_counts())
    print("Anomaly distribution:")
    print(df["anomaly_label"].value_counts())
