"""
SENTINEL-BELT SYSTEM: PUBLICATION GRAPH & METRIC GENERATOR
Generates 8 high-resolution publication charts based on the 13 confined space scenarios.
Saves all charts into ml_pipeline/evaluation_plots/
"""

import os
import time
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
try:
    import seaborn as sns
    HAS_SEABORN = True
except ImportError:
    HAS_SEABORN = False
import joblib
from sklearn.metrics import confusion_matrix, roc_curve, auc, precision_recall_curve
from sklearn.preprocessing import label_binarize

# Configure aesthetic styles
plt.style.use('seaborn-v0_8-whitegrid' if 'seaborn-v0_8-whitegrid' in plt.style.available else 'default')
plt.rcParams['font.sans-serif'] = 'DejaVu Sans'
plt.rcParams['axes.edgecolor'] = '#cbd5e1'
plt.rcParams['axes.linewidth'] = 1.2

def generate_all_evaluation_plots():
    curr_dir = os.path.dirname(os.path.abspath(__file__))
    output_dir = os.path.join(curr_dir, "evaluation_plots")
    os.makedirs(output_dir, exist_ok=True)
    
    csv_path = os.path.join(curr_dir, "sentinel_dataset.csv")
    rf_path = os.path.join(curr_dir, "rf_risk_model.joblib")
    iso_path = os.path.join(curr_dir, "iso_forest_model.joblib")
    
    if not os.path.exists(csv_path) or not os.path.exists(rf_path):
        import train_models
        train_models.train_and_evaluate()
        
    df = pd.read_csv(csv_path)
    model = joblib.load(rf_path)
    iso_model = joblib.load(iso_path)
    
    features = [
        "O2", "H2S", "CO", "CH4", "Temp", "Humidity", "WaterLevel",
        "Ax", "Ay", "Az", "dO2_dt", "dH2S_dt", "dCO_dt", "dCH4_dt"
    ]
    
    X = df[features].values
    label_map = {"SAFE": 0, "WARNING": 1, "CRITICAL": 2}
    class_names = ["SAFE", "WARNING", "CRITICAL"]
    y_true = df["risk_label"].map(label_map).values
    y_pred = model.predict(X)
    y_proba = model.predict_proba(X)
    
    print("[1/8] Generating Confusion Matrices Plot...")
    # Plot 1: Multi-class and Binary Confusion Matrix
    fig, axes = plt.subplots(1, 2, figsize=(14, 5.5), dpi=300)
    
    cm_multi = confusion_matrix(y_true, y_pred)
    if HAS_SEABORN:
        sns.heatmap(cm_multi, annot=True, fmt='d', cmap='Blues', cbar=False,
                    xticklabels=class_names, yticklabels=class_names, ax=axes[0],
                    annot_kws={"size": 13, "weight": "bold"})
    else:
        im0 = axes[0].imshow(cm_multi, cmap='Blues')
        for i in range(len(class_names)):
            for j in range(len(class_names)):
                axes[0].text(j, i, str(cm_multi[i, j]), ha="center", va="center", color="black" if cm_multi[i, j] < cm_multi.max()/2 else "white", weight="bold", fontsize=13)
        axes[0].set_xticks(range(len(class_names)))
        axes[0].set_yticks(range(len(class_names)))
        axes[0].set_xticklabels(class_names)
        axes[0].set_yticklabels(class_names)
        
    axes[0].set_title("Multi-Class Confusion Matrix (Risk State)", fontsize=13, weight="bold", pad=12)
    axes[0].set_xlabel("Predicted Class", fontsize=11, weight="bold")
    axes[0].set_ylabel("True Ground Truth", fontsize=11, weight="bold")
    
    # Binary Hazard CM
    y_true_bin = (y_true > 0).astype(int)
    y_pred_bin = (y_pred > 0).astype(int)
    cm_bin = confusion_matrix(y_true_bin, y_pred_bin)
    bin_names = ["Normal (Safe)", "Hazard Present"]
    if HAS_SEABORN:
        sns.heatmap(cm_bin, annot=True, fmt='d', cmap='Oranges', cbar=False,
                    xticklabels=bin_names, yticklabels=bin_names, ax=axes[1],
                    annot_kws={"size": 13, "weight": "bold"})
    else:
        axes[1].imshow(cm_bin, cmap='Oranges')
        for i in range(2):
            for j in range(2):
                axes[1].text(j, i, str(cm_bin[i, j]), ha="center", va="center", color="black" if cm_bin[i, j] < cm_bin.max()/2 else "white", weight="bold", fontsize=13)
        axes[1].set_xticks(range(2))
        axes[1].set_yticks(range(2))
        axes[1].set_xticklabels(bin_names)
        axes[1].set_yticklabels(bin_names)
        
    axes[1].set_title("Binary Hazard Detection Matrix (Safe vs Danger)", fontsize=13, weight="bold", pad=12)
    axes[1].set_xlabel("Predicted State", fontsize=11, weight="bold")
    axes[1].set_ylabel("True Ground Truth", fontsize=11, weight="bold")
    
    plt.tight_layout()
    p1 = os.path.join(output_dir, "01_confusion_matrices.png")
    plt.savefig(p1)
    plt.close()

    print("[2/8] Generating Multi-Gas Time-Series Trajectories Plot...")
    # Plot 2: Simulated Gas Dynamics (T2, T3, T4, T6)
    t2_df = df[df["scenario"] == "T2_O2_DEPLETION"].reset_index()
    t3_df = df[df["scenario"] == "T3_H2S_SURGE"].reset_index()
    t4_df = df[df["scenario"] == "T4_CO_SURGE"].reset_index()
    t6_df = df[df["scenario"] == "T6_WATER_RISE"].reset_index()
    
    fig, axes = plt.subplots(2, 2, figsize=(15, 10), dpi=300)
    
    # Oxygen Depletion
    axes[0, 0].plot(t2_df["step"], t2_df["O2"], color="#0284c7", lw=2.5, label="O2 Concentration (%)")
    axes[0, 0].axhline(19.5, color="#f59e0b", ls="--", lw=1.8, label="OSHA Warning Limit (19.5%)")
    axes[0, 0].axhline(16.0, color="#ef4444", ls="--", lw=1.8, label="IDLH Critical Limit (16.0%)")
    axes[0, 0].set_title("Scenario T2: Oxygen Depletion Dynamics", fontsize=12, weight="bold")
    axes[0, 0].set_xlabel("Time (seconds)", fontsize=10)
    axes[0, 0].set_ylabel("Oxygen Concentration (%)", fontsize=10)
    axes[0, 0].set_ylim(14.0, 22.0)
    axes[0, 0].legend(loc="upper right", frameon=True)
    
    # H2S Sewer Gas Surge
    axes[0, 1].plot(t3_df["step"], t3_df["H2S"], color="#e11d48", lw=2.5, label="H2S Concentration (ppm)")
    axes[0, 1].axhline(10.0, color="#f59e0b", ls="--", lw=1.8, label="OSHA PEL Warning (10 ppm)")
    axes[0, 1].axhline(20.0, color="#b91c1c", ls="--", lw=1.8, label="Ceiling / Critical (20 ppm)")
    axes[0, 1].set_title("Scenario T3: Hydrogen Sulphide (H2S) Surge Dynamics", fontsize=12, weight="bold")
    axes[0, 1].set_xlabel("Time (seconds)", fontsize=10)
    axes[0, 1].set_ylabel("H2S (ppm)", fontsize=10)
    axes[0, 1].legend(loc="upper left", frameon=True)
    
    # CO Surge
    axes[1, 0].plot(t4_df["step"], t4_df["CO"], color="#ea580c", lw=2.5, label="CO Concentration (ppm)")
    axes[1, 0].axhline(25.0, color="#f59e0b", ls="--", lw=1.8, label="NIOSH Warning (25 ppm)")
    axes[1, 0].axhline(50.0, color="#ef4444", ls="--", lw=1.8, label="Evacuation Critical (50 ppm)")
    axes[1, 0].set_title("Scenario T4: Carbon Monoxide (CO) Accumulation Dynamics", fontsize=12, weight="bold")
    axes[1, 0].set_xlabel("Time (seconds)", fontsize=10)
    axes[1, 0].set_ylabel("CO (ppm)", fontsize=10)
    axes[1, 0].legend(loc="upper left", frameon=True)
    
    # Water Level Inundation
    axes[1, 1].plot(t6_df["step"], t6_df["WaterLevel"], color="#0891b2", lw=2.5, label="Water Inundation Level (%)")
    axes[1, 1].axhline(30.0, color="#f59e0b", ls="--", lw=1.8, label="Ankle Warning (30%)")
    axes[1, 1].axhline(60.0, color="#ef4444", ls="--", lw=1.8, label="Waist Inundation Critical (60%)")
    axes[1, 1].set_title("Scenario T6: Shaft Inundation & Rising Water Dynamics", fontsize=12, weight="bold")
    axes[1, 1].set_xlabel("Time (seconds)", fontsize=10)
    axes[1, 1].set_ylabel("Water Depth Capacity (%)", fontsize=10)
    axes[1, 1].legend(loc="upper left", frameon=True)
    
    plt.tight_layout()
    p2 = os.path.join(output_dir, "02_gas_time_series_trajectories.png")
    plt.savefig(p2)
    plt.close()

    print("[3/8] Generating Risk Score Timeline Plot...")
    # Plot 3: Risk Score Trajectory across Compound Scenario T10
    t10_df = df[df["scenario"] == "T10_COMPOUND_HAZARDS"].reset_index()
    # Compute simulated risk score:
    risk_scores = []
    for _, row in t10_df.iterrows():
        s_o2 = 0.0 if row["O2"] >= 19.5 else ((19.5 - row["O2"]) / 3.5 if row["O2"] > 16.0 else 1.0)
        s_h2s = min(1.0, row["H2S"] / 20.0)
        s_w = min(1.0, row["WaterLevel"] / 60.0)
        score = 100.0 * (0.30 * s_o2 + 0.25 * s_h2s + 0.10 * s_w)
        risk_scores.append(score)
        
    plt.figure(figsize=(12, 5), dpi=300)
    plt.plot(t10_df["step"], risk_scores, color="#7c3aed", lw=2.8, label="Composite Risk Index (0-100)")
    plt.axhline(30.0, color="#10b981", ls="--", lw=1.5, label="Safe Threshold (<30)")
    plt.axhline(60.0, color="#ef4444", ls="--", lw=1.5, label="Critical Evacuation Threshold (>60)")
    plt.fill_between(t10_df["step"], 0, 30, color="#10b981", alpha=0.12, label="Green Zone: Safe Working")
    plt.fill_between(t10_df["step"], 30, 60, color="#f59e0b", alpha=0.12, label="Yellow Zone: Warning / Ventilate")
    plt.fill_between(t10_df["step"], 60, 100, color="#ef4444", alpha=0.15, label="Red Zone: Critical / Evacuate Now")
    plt.title("Scenario T10: Multi-Hazard Compound Risk Index Progression & Alarm Triggers", fontsize=13, weight="bold")
    plt.xlabel("Elapsed Inspection Time (seconds)", fontsize=11)
    plt.ylabel("Composite Risk Score (0 - 100)", fontsize=11)
    plt.ylim(0, 105)
    plt.legend(loc="upper left", frameon=True)
    plt.tight_layout()
    p3 = os.path.join(output_dir, "03_risk_score_timeline.png")
    plt.savefig(p3)
    plt.close()

    print("[4/8] Generating Feature Importance Plot...")
    # Plot 4: Random Forest Feature Importances
    importances = model.feature_importances_
    indices = np.argsort(importances)[::-1]
    
    plt.figure(figsize=(11, 5.5), dpi=300)
    colors = plt.cm.viridis(np.linspace(0.2, 0.85, len(features)))
    plt.bar([features[i] for i in indices], [importances[i] for i in indices], color=colors, edgecolor="#1e293b", lw=0.8)
    plt.title("Random Forest Multi-Sensor Feature Importance for Manhole Hazard Prediction", fontsize=13, weight="bold", pad=12)
    plt.xlabel("Sensor Features & Rate of Change (Temporal Derivatives)", fontsize=11, weight="bold")
    plt.ylabel("Relative Gini Importance Score", fontsize=11, weight="bold")
    plt.xticks(rotation=45, ha="right", fontsize=10)
    plt.grid(axis='y', linestyle='--', alpha=0.7)
    plt.tight_layout()
    p4 = os.path.join(output_dir, "04_feature_importance.png")
    plt.savefig(p4)
    plt.close()

    print("[5/8] Generating ROC and PR Curves...")
    # Plot 5: Multi-class ROC and Precision-Recall Curves
    y_bin = label_binarize(y_true, classes=[0, 1, 2])
    fig, axes = plt.subplots(1, 2, figsize=(14, 5.5), dpi=300)
    
    curve_colors = ["#10b981", "#f59e0b", "#ef4444"]
    for i in range(3):
        fpr, tpr, _ = roc_curve(y_bin[:, i], y_proba[:, i])
        roc_auc = auc(fpr, tpr)
        axes[0].plot(fpr, tpr, color=curve_colors[i], lw=2.2, label=f"{class_names[i]} (AUC = {roc_auc:.4f})")
        
    axes[0].plot([0, 1], [0, 1], 'k--', lw=1.2)
    axes[0].set_title("Multi-Class Receiver Operating Characteristic (ROC)", fontsize=12, weight="bold")
    axes[0].set_xlabel("False Positive Rate (FPR)", fontsize=10)
    axes[0].set_ylabel("True Positive Rate (Sensitivity / Recall)", fontsize=10)
    axes[0].legend(loc="lower right")
    
    for i in range(3):
        precision, recall, _ = precision_recall_curve(y_bin[:, i], y_proba[:, i])
        pr_auc = auc(recall, precision)
        axes[1].plot(recall, precision, color=curve_colors[i], lw=2.2, label=f"{class_names[i]} (AUC-PR = {pr_auc:.4f})")
        
    axes[1].set_title("Multi-Class Precision-Recall Curve (PR)", fontsize=12, weight="bold")
    axes[1].set_xlabel("Recall (Sensitivity)", fontsize=10)
    axes[1].set_ylabel("Precision (Positive Predictive Value)", fontsize=10)
    axes[1].legend(loc="lower left")
    
    plt.tight_layout()
    p5 = os.path.join(output_dir, "05_roc_and_pr_curves.png")
    plt.savefig(p5)
    plt.close()

    print("[6/8] Generating Fall and Motion Signature Plot...")
    # Plot 6: Fall Signature (T8 Scenario IMU acceleration magnitude)
    t8_df = df[df["scenario"] == "T8_FALL_EVENT"].reset_index()
    accel_mag = np.sqrt(t8_df["Ax"]**2 + t8_df["Ay"]**2 + t8_df["Az"]**2)
    
    plt.figure(figsize=(12, 5), dpi=300)
    plt.plot(t8_df["step"], accel_mag, color="#dc2626", lw=2.2, label="Acceleration Vector Magnitude |a| (g)")
    plt.axhline(0.5, color="#f59e0b", ls=":", lw=1.6, label="Freefall Threshold (< 0.5g)")
    plt.axhline(2.5, color="#991b1b", ls="--", lw=1.6, label="Impact Shock Threshold (> 2.5g)")
    plt.axvspan(150, 156, color="#fef08a", alpha=0.35, label="Stage 1: Freefall Phase (Slip/Drop)")
    plt.vlines(160, 0, 3.5, color="#b91c1c", lw=2, label="Stage 2: Ground Impact Collision")
    plt.axvspan(165, len(t8_df), color="#fee2e2", alpha=0.45, label="Stage 3: Post-Impact Motionless Immobility")
    plt.title("Scenario T8: Fall Detection Physics & 3-Axis IMU Acceleration Signature", fontsize=13, weight="bold")
    plt.xlabel("Sampling Time Frame", fontsize=11)
    plt.ylabel("Resultant Acceleration Magnitude (g = 9.81 m/s²)", fontsize=11)
    plt.ylim(0, 3.8)
    plt.legend(loc="upper right", frameon=True)
    plt.tight_layout()
    p6 = os.path.join(output_dir, "06_fall_motion_signature.png")
    plt.savefig(p6)
    plt.close()

    print("[7/8] Generating Anomaly Distribution Plot...")
    # Plot 7: Isolation Forest Decision Score Distribution
    scores = -iso_model.score_samples(X) # Higher score = more anomalous
    anom_labels = df["anomaly_label"].values
    
    plt.figure(figsize=(11, 5), dpi=300)
    if HAS_SEABORN:
        sns.kdeplot(scores[anom_labels == 0], fill=True, color="#10b981", alpha=0.4, label="Normal Operation Profile", lw=2)
        sns.kdeplot(scores[anom_labels == 1], fill=True, color="#ef4444", alpha=0.4, label="Sensor Fault & Anomaly Profile", lw=2)
    else:
        plt.hist(scores[anom_labels == 0], bins=30, density=True, color="#10b981", alpha=0.5, label="Normal Operation Profile")
        plt.hist(scores[anom_labels == 1], bins=30, density=True, color="#ef4444", alpha=0.5, label="Sensor Fault & Anomaly Profile")
    plt.axvline(0.5, color="#1e293b", ls="--", lw=1.8, label="Anomaly Decision Boundary Threshold")
    plt.title("Isolation Forest Multi-Variate Anomaly Score Density Distribution", fontsize=13, weight="bold")
    plt.xlabel("Anomaly Deviance Score", fontsize=11)
    plt.ylabel("Probability Density", fontsize=11)
    plt.legend(loc="upper right", frameon=True)
    plt.tight_layout()
    p7 = os.path.join(output_dir, "07_anomaly_distribution.png")
    plt.savefig(p7)
    plt.close()

    print("[8/8] Generating Latency Distribution Plot...")
    # Plot 8: Latency distribution
    latencies = []
    for _ in range(1000):
        t0 = time.perf_counter()
        _ = model.predict(X[:1])
        t1 = time.perf_counter()
        latencies.append((t1 - t0) * 1000.0)
        
    plt.figure(figsize=(11, 5), dpi=300)
    plt.hist(latencies, bins=35, color="#2563eb", edgecolor="#1e3a8a", alpha=0.75, density=True)
    plt.axvline(np.mean(latencies), color="#dc2626", lw=2.2, ls="--", label=f"Mean Inference: {np.mean(latencies):.3f} ms")
    plt.axvline(50.0, color="#ea580c", lw=2.0, ls=":", label="Hard Real-Time Constraint (50.0 ms)")
    plt.title("Edge AI Model Inference Latency Distribution on Sensor Vectors", fontsize=13, weight="bold")
    plt.xlabel("Inference Latency (Milliseconds)", fontsize=11)
    plt.ylabel("Sample Density", fontsize=11)
    plt.legend(loc="upper right", frameon=True)
    plt.tight_layout()
    p8 = os.path.join(output_dir, "08_latency_distribution.png")
    plt.savefig(p8)
    plt.close()

    print(f"\nSuccessfully generated and saved all 8 publication graphs to:\n{output_dir}")

if __name__ == "__main__":
    generate_all_evaluation_plots()
