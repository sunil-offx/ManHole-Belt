/**
 * SENTINEL-BELT SUPERVISOR DASHBOARD
 * Real-time WebSocket Telemetry Client, Dynamic Visualizations & Scenario Test Rig
 */

(function () {
    'use strict';

    // Application State
    const state = {
        deviceId: 'SB-001',
        connected: false,
        audioEnabled: true,
        lastStatus: 'SAFE',
        chart: null,
        historyLimit: 30,
        audioCtx: null,
        sirenOscillator: null,
        activeAlerts: []
    };

    // DOM Elements
    const elements = {
        banner: document.getElementById('master-hazard-banner'),
        bannerText: document.getElementById('banner-status-text'),
        bannerRiskPill: document.getElementById('banner-risk-pill'),
        connIndicator: document.getElementById('conn-indicator'),
        connStatusText: document.getElementById('conn-status-text'),
        btnAudioToggle: document.getElementById('btn-audio-toggle'),
        audioIcon: document.getElementById('audio-icon'),
        activeScenarioBadge: document.getElementById('active-scenario-badge'),

        // Gauges & Readouts
        valO2: document.getElementById('val-o2'),
        barO2: document.getElementById('bar-o2'),
        deltaO2: document.getElementById('delta-o2'),
        pillO2: document.getElementById('status-pill-o2'),

        valH2S: document.getElementById('val-h2s'),
        barH2S: document.getElementById('bar-h2s'),
        deltaH2S: document.getElementById('delta-h2s'),
        pillH2S: document.getElementById('status-pill-h2s'),

        valCO: document.getElementById('val-co'),
        barCO: document.getElementById('bar-co'),
        deltaCO: document.getElementById('delta-co'),
        pillCO: document.getElementById('status-pill-co'),

        valCH4: document.getElementById('val-ch4'),
        barCH4: document.getElementById('bar-ch4'),
        deltaCH4: document.getElementById('delta-ch4'),
        pillCH4: document.getElementById('status-pill-ch4'),

        // Water & Env
        valWater: document.getElementById('val-water'),
        valWaterCm: document.getElementById('val-water-cm'),
        visualWaterFill: document.getElementById('visual-water-fill'),
        valTemp: document.getElementById('val-temp'),
        valHum: document.getElementById('val-hum'),
        valSoundSpeed: document.getElementById('val-sound-speed'),

        // Risk Meter
        riskCircleBar: document.getElementById('risk-circle-bar'),
        valRiskScore: document.getElementById('val-risk-score'),
        valRiskGrade: document.getElementById('val-risk-grade'),
        pbO2: document.getElementById('pb-o2'),
        pbH2S: document.getElementById('pb-h2s'),
        pbCO: document.getElementById('pb-co'),
        pbCH4: document.getElementById('pb-ch4'),
        pbWater: document.getElementById('pb-water'),
        pbMotion: document.getElementById('pb-motion'),

        // Diagnostics
        diagAnomaly: document.getElementById('diag-anomaly-pill'),
        diagFall: document.getElementById('diag-fall-pill'),
        diagOverride: document.getElementById('diag-override-pill'),

        // Worker Meta
        valBattery: document.getElementById('val-battery'),
        valMotionState: document.getElementById('val-motion-state'),
        workerStatusDot: document.getElementById('worker-status-dot'),

        // Temporal
        timeO2: document.getElementById('time-to-o2'),
        timeH2S: document.getElementById('time-to-h2s'),
        timeCO: document.getElementById('time-to-co'),
        timeCH4: document.getElementById('time-to-ch4'),

        // Table
        alertsTbody: document.getElementById('alerts-tbody'),
        activeAlertCount: document.getElementById('active-alert-count'),

        // Config Drawer
        toggleConfigBtn: document.getElementById('toggle-config-btn'),
        configPanelBody: document.getElementById('config-panel-body'),
        configArrow: document.getElementById('config-arrow'),
        formConfig: document.getElementById('form-device-config'),
        cfgFeedbackMsg: document.getElementById('cfg-feedback-msg')
    };

    // Initialize Chart.js
    function initChart() {
        const ctx = document.getElementById('liveMultiSensorChart').getContext('2d');
        const initialLabels = Array.from({ length: 20 }, (_, i) => `-${20 - i}s`);
        
        state.chart = new Chart(ctx, {
            type: 'line',
            data: {
                labels: initialLabels,
                datasets: [
                    {
                        label: 'O₂ (%)',
                        borderColor: '#06b6d4',
                        backgroundColor: 'rgba(6, 182, 212, 0.05)',
                        borderWidth: 2,
                        tension: 0.3,
                        pointRadius: 0,
                        data: Array(20).fill(20.9)
                    },
                    {
                        label: 'H₂S (ppm)',
                        borderColor: '#f59e0b',
                        backgroundColor: 'transparent',
                        borderWidth: 2,
                        tension: 0.3,
                        pointRadius: 0,
                        data: Array(20).fill(0.1)
                    },
                    {
                        label: 'CO (ppm)',
                        borderColor: '#3b82f6',
                        backgroundColor: 'transparent',
                        borderWidth: 2,
                        tension: 0.3,
                        pointRadius: 0,
                        data: Array(20).fill(1.0)
                    },
                    {
                        label: 'CH₄ (% LEL)',
                        borderColor: '#a78bfa',
                        backgroundColor: 'transparent',
                        borderWidth: 2,
                        tension: 0.3,
                        pointRadius: 0,
                        data: Array(20).fill(0.0)
                    },
                    {
                        label: 'Risk Score',
                        borderColor: '#ef4444',
                        borderDash: [4, 4],
                        borderWidth: 2,
                        tension: 0.3,
                        pointRadius: 0,
                        data: Array(20).fill(4.2)
                    }
                ]
            },
            options: {
                responsive: true,
                maintainAspectRatio: false,
                animation: { duration: 250 },
                plugins: {
                    legend: { display: false }
                },
                scales: {
                    x: {
                        grid: { color: 'rgba(255, 255, 255, 0.04)' },
                        ticks: { color: '#64748b', font: { family: 'JetBrains Mono', size: 10 } }
                    },
                    y: {
                        grid: { color: 'rgba(255, 255, 255, 0.04)' },
                        ticks: { color: '#64748b', font: { family: 'JetBrains Mono', size: 10 } },
                        min: 0,
                        max: 100
                    }
                }
            }
        });
    }

    // Web Audio Synthesizer for Local Audible Siren
    function playAlarmTone(status) {
        if (!state.audioEnabled) return;
        try {
            if (!state.audioCtx) {
                const AudioContext = window.AudioContext || window.webkitAudioContext;
                state.audioCtx = new AudioContext();
            }
            if (state.audioCtx.state === 'suspended') {
                state.audioCtx.resume();
            }

            if (status === 'CRITICAL' || status === 'EMERGENCY_SOS') {
                const osc = state.audioCtx.createOscillator();
                const gain = state.audioCtx.createGain();
                osc.type = 'sawtooth';
                osc.frequency.setValueAtTime(880, state.audioCtx.currentTime);
                osc.frequency.exponentialRampToValueAtTime(440, state.audioCtx.currentTime + 0.3);
                gain.gain.setValueAtTime(0.12, state.audioCtx.currentTime);
                gain.gain.exponentialRampToValueAtTime(0.01, state.audioCtx.currentTime + 0.3);
                osc.connect(gain);
                gain.connect(state.audioCtx.destination);
                osc.start();
                osc.stop(state.audioCtx.currentTime + 0.3);
            } else if (status === 'WARNING') {
                const osc = state.audioCtx.createOscillator();
                const gain = state.audioCtx.createGain();
                osc.type = 'sine';
                osc.frequency.setValueAtTime(600, state.audioCtx.currentTime);
                gain.gain.setValueAtTime(0.08, state.audioCtx.currentTime);
                gain.gain.exponentialRampToValueAtTime(0.01, state.audioCtx.currentTime + 0.15);
                osc.connect(gain);
                gain.connect(state.audioCtx.destination);
                osc.start();
                osc.stop(state.audioCtx.currentTime + 0.15);
            }
        } catch (e) {
            console.warn("Audio Context error:", e);
        }
    }

    // Update UI with incoming telemetry data
    function applyTelemetry(data) {
        const sensors = data.sensors || {};
        const o2 = Number(sensors.O2 !== undefined ? sensors.O2 : 20.9);
        const h2s = Number(sensors.H2S !== undefined ? sensors.H2S : 0.0);
        const co = Number(sensors.CO !== undefined ? sensors.CO : 0.0);
        const ch4 = Number(sensors.CH4 !== undefined ? sensors.CH4 : 0.0);
        const temp = Number(sensors.temp !== undefined ? sensors.temp : 24.5);
        const hum = Number(sensors.hum !== undefined ? sensors.hum : 65.0);
        const water = Number(sensors.water_level !== undefined ? sensors.water_level : 5.0);
        const riskScore = Number(data.risk_score !== undefined ? data.risk_score : 4.2);
        const status = data.status || 'SAFE';
        const anomaly = Boolean(data.anomaly);
        const fall = Boolean(data.fall_detected);

        // 1. Update Master Banner
        elements.banner.className = 'hazard-banner';
        if (fall) {
            elements.banner.classList.add('status-sos');
            elements.bannerText.textContent = '🚨 EMERGENCY SOS: WORKER FALL / INCAPACITATED DETECTED!';
            elements.workerStatusDot.className = 'avatar-status-dot status-dot-critical';
        } else if (status === 'CRITICAL') {
            elements.banner.classList.add('status-critical');
            elements.bannerText.textContent = 'CRITICAL ATMOSPHERIC HAZARD — IMMEDIATE EVACUATION ORDERED';
            elements.workerStatusDot.className = 'avatar-status-dot status-dot-critical';
        } else if (status === 'WARNING') {
            elements.banner.classList.add('status-warning');
            elements.bannerText.textContent = 'CAUTION: HAZARD THRESHOLD EXCEEDED — PREPARE EXTRACTION';
            elements.workerStatusDot.className = 'avatar-status-dot status-dot-warning';
        } else {
            elements.banner.classList.add('status-safe');
            elements.bannerText.textContent = 'SYSTEM NORMAL - ALL WORKERS SAFE';
            elements.workerStatusDot.className = 'avatar-status-dot status-dot-safe';
        }
        elements.bannerRiskPill.textContent = `RISK: ${riskScore.toFixed(1)} / 100`;

        // 2. Play audible siren if risk escalated
        if (status !== 'SAFE' && (status !== state.lastStatus || Math.random() < 0.25)) {
            playAlarmTone(status);
        }
        state.lastStatus = status;

        // 3. Update Gas Displays
        elements.valO2.textContent = o2.toFixed(1);
        elements.barO2.style.width = `${Math.min(100, (o2 / 25.0) * 100)}%`;
        if (o2 < 16.0) {
            elements.pillO2.className = 'gas-status-pill pill-crit';
            elements.pillO2.textContent = 'CRITICAL (<16%)';
        } else if (o2 < 19.5) {
            elements.pillO2.className = 'gas-status-pill pill-warn';
            elements.pillO2.textContent = 'WARNING (<19.5%)';
        } else {
            elements.pillO2.className = 'gas-status-pill pill-safe';
            elements.pillO2.textContent = 'SAFE';
        }

        elements.valH2S.textContent = h2s.toFixed(1);
        elements.barH2S.style.width = `${Math.min(100, (h2s / 30.0) * 100)}%`;
        if (h2s >= 20.0) {
            elements.pillH2S.className = 'gas-status-pill pill-crit';
            elements.pillH2S.textContent = 'CRITICAL (IDLH)';
        } else if (h2s >= 10.0) {
            elements.pillH2S.className = 'gas-status-pill pill-warn';
            elements.pillH2S.textContent = 'WARNING (>10ppm)';
        } else {
            elements.pillH2S.className = 'gas-status-pill pill-safe';
            elements.pillH2S.textContent = 'SAFE';
        }

        elements.valCO.textContent = co.toFixed(1);
        elements.barCO.style.width = `${Math.min(100, (co / 100.0) * 100)}%`;
        if (co >= 50.0) {
            elements.pillCO.className = 'gas-status-pill pill-crit';
            elements.pillCO.textContent = 'CRITICAL (>50ppm)';
        } else if (co >= 25.0) {
            elements.pillCO.className = 'gas-status-pill pill-warn';
            elements.pillCO.textContent = 'WARNING (>25ppm)';
        } else {
            elements.pillCO.className = 'gas-status-pill pill-safe';
            elements.pillCO.textContent = 'SAFE';
        }

        elements.valCH4.textContent = ch4.toFixed(1);
        elements.barCH4.style.width = `${Math.min(100, (ch4 / 25.0) * 100)}%`;
        if (ch4 >= 25.0) {
            elements.pillCH4.className = 'gas-status-pill pill-crit';
            elements.pillCH4.textContent = 'EXPLOSIVE (>25%)';
        } else if (ch4 >= 10.0) {
            elements.pillCH4.className = 'gas-status-pill pill-warn';
            elements.pillCH4.textContent = 'WARNING (>10%)';
        } else {
            elements.pillCH4.className = 'gas-status-pill pill-safe';
            elements.pillCH4.textContent = 'SAFE';
        }

        // 4. Update Water Level Visualizer & Env
        elements.valWater.textContent = water.toFixed(1);
        const waterHeightCm = (water / 100.0) * 200.0;
        elements.valWaterCm.textContent = `${waterHeightCm.toFixed(1)} cm`;
        elements.visualWaterFill.style.height = `${Math.min(100, water)}%`;
        elements.valTemp.textContent = `${temp.toFixed(1)} °C`;
        elements.valHum.textContent = `${hum.toFixed(1)} %`;
        const soundSpeed = 331.3 + (0.606 * temp);
        elements.valSoundSpeed.textContent = `${soundSpeed.toFixed(1)} m/s`;

        // 5. Update Risk Gauge
        elements.valRiskScore.textContent = riskScore.toFixed(1);
        elements.valRiskGrade.textContent = status;
        if (status === 'SAFE') elements.valRiskGrade.className = 'risk-grade text-green';
        else if (status === 'WARNING') elements.valRiskGrade.className = 'risk-grade text-amber';
        else elements.valRiskGrade.className = 'risk-grade text-red';

        // Update circular SVG stroke offset (total circumference 440)
        const offset = 440 - (440 * (riskScore / 100));
        elements.riskCircleBar.style.strokeDashoffset = offset;
        if (status === 'SAFE') elements.riskCircleBar.style.stroke = '#10b981';
        else if (status === 'WARNING') elements.riskCircleBar.style.stroke = '#f59e0b';
        else elements.riskCircleBar.style.stroke = '#ef4444';

        // Breakdown bars
        elements.pbO2.style.width = `${Math.min(100, (o2 < 19.5 ? (19.5 - o2) / 3.5 : 0) * 100)}%`;
        elements.pbH2S.style.width = `${Math.min(100, (h2s / 20.0) * 100)}%`;
        elements.pbCO.style.width = `${Math.min(100, (co / 50.0) * 100)}%`;
        elements.pbCH4.style.width = `${Math.min(100, (ch4 / 25.0) * 100)}%`;
        elements.pbWater.style.width = `${Math.min(100, water)}%`;
        elements.pbMotion.style.width = fall ? '100%' : '0%';

        // 6. Diagnostics
        elements.diagAnomaly.className = anomaly ? 'diag-pill pill-alert' : 'diag-pill pill-normal';
        elements.diagAnomaly.textContent = anomaly ? 'ANOMALY DETECTED' : 'NORMAL PATTERN';

        elements.diagFall.className = fall ? 'diag-pill pill-alert' : 'diag-pill pill-normal';
        elements.diagFall.textContent = fall ? 'WORKER FALL' : 'NO FALL DETECTED';

        if (o2 < 19.5 || fall) {
            elements.diagOverride.className = 'diag-pill pill-alert';
            elements.diagOverride.textContent = 'ACTIVE (OVERRIDE)';
        } else {
            elements.diagOverride.className = 'diag-pill pill-inactive';
            elements.diagOverride.textContent = 'STANDBY';
        }

        // Motion & Battery
        if (fall) {
            elements.valMotionState.textContent = 'FALL EVENT';
            elements.valMotionState.className = 'stat-value text-red';
        } else {
            elements.valMotionState.textContent = 'Active (1.0g)';
            elements.valMotionState.className = 'stat-value text-green';
        }

        // 7. Update Chart
        if (state.chart) {
            const chartData = state.chart.data;
            const nowTime = new Date().toLocaleTimeString();
            chartData.labels.push(nowTime);
            if (chartData.labels.length > state.historyLimit) chartData.labels.shift();

            chartData.datasets[0].data.push(o2);
            chartData.datasets[1].data.push(h2s);
            chartData.datasets[2].data.push(co);
            chartData.datasets[3].data.push(ch4);
            chartData.datasets[4].data.push(riskScore);

            chartData.datasets.forEach(ds => {
                if (ds.data.length > state.historyLimit) ds.data.shift();
            });

            state.chart.update('none');
        }
    }

    // Connect to Backend WebSocket
    function connectWebSocket() {
        const protocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
        const host = window.location.host || 'localhost:8000';
        const wsUrl = `${protocol}//${host}/ws/telemetry`;

        try {
            const ws = new WebSocket(wsUrl);

            ws.onopen = () => {
                state.connected = true;
                elements.connIndicator.style.background = '#10b981';
                elements.connIndicator.style.boxShadow = '0 0 8px #10b981';
                elements.connStatusText.textContent = 'LIVE WEBSOCKET STREAM';
                console.log('WebSocket connected to', wsUrl);
            };

            ws.onmessage = (event) => {
                try {
                    const msg = JSON.parse(event.data);
                    if (msg.type === 'telemetry') {
                        applyTelemetry(msg.data);
                    } else if (msg.type === 'alert') {
                        prependAlertRow(msg.data);
                    }
                } catch (err) {
                    console.error('Error parsing WS message:', err);
                }
            };

            ws.onclose = () => {
                state.connected = false;
                elements.connIndicator.style.background = '#f59e0b';
                elements.connIndicator.style.boxShadow = '0 0 8px #f59e0b';
                elements.connStatusText.textContent = 'RECONNECTING WS (POLLING REST)...';
                setTimeout(connectWebSocket, 3000);
            };

            ws.onerror = () => {
                ws.close();
            };
        } catch (e) {
            console.warn('WS Init failure, relying on HTTP polling:', e);
            setTimeout(connectWebSocket, 5000);
        }
    }

    // Poll REST for telemetry fallback and alerts
    async function pollFallbackData() {
        try {
            const res = await fetch('/api/v1/devices/SB-001/telemetry?limit=1');
            if (res.ok) {
                const logs = await res.json();
                if (logs.length > 0) {
                    applyTelemetry(logs[0]);
                }
            }
        } catch (e) {
            // Backend offline or standalone preview
        }
    }

    // Prepend Alert to Table
    function prependAlertRow(alert) {
        state.activeAlerts.unshift(alert);
        elements.activeAlertCount.textContent = `${state.activeAlerts.length} INCIDENTS`;

        const row = document.createElement('tr');
        row.innerHTML = `
            <td style="font-family: var(--font-mono);">${new Date(alert.timestamp).toLocaleTimeString()}</td>
            <td><strong style="color: var(--color-cyan);">${alert.device_id}</strong></td>
            <td><span class="gas-status-pill pill-crit">${alert.severity}</span></td>
            <td><strong>${alert.alert_type}</strong>: ${alert.message}</td>
            <td><small style="color: var(--color-safe);">✓ SMS & Dispatch Logged</small></td>
            <td>
                <button class="btn-table-ack" onclick="acknowledgeAlert(${alert.alert_id || 0}, this)">
                    Acknowledge
                </button>
            </td>
        `;

        const emptyRow = elements.alertsTbody.querySelector('.empty-row');
        if (emptyRow) emptyRow.remove();

        elements.alertsTbody.insertBefore(row, elements.alertsTbody.firstChild);
    }

    window.acknowledgeAlert = async function (alertId, btn) {
        try {
            if (alertId > 0) {
                await fetch(`/api/v1/alerts/${alertId}/acknowledge`, {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({ acknowledged_by: 'Supervisor on Watch' })
                });
            }
            btn.textContent = 'Acknowledged';
            btn.disabled = true;
            btn.style.opacity = '0.5';
        } catch (e) {
            btn.textContent = 'Done';
        }
    };

    // Scenario Simulator Handler (T1 - T13)
    async function triggerScenario(scenarioId) {
        elements.activeScenarioBadge.textContent = `Scenario: Testing ${scenarioId}...`;
        try {
            const res = await fetch(`/api/v1/simulate/scenario?scenario_id=${scenarioId}`, {
                method: 'POST'
            });
            if (res.ok) {
                const json = await res.json();
                elements.activeScenarioBadge.textContent = `Active Scenario: ${json.name} (${scenarioId})`;
                applyTelemetry(json.telemetry);
            }
        } catch (e) {
            console.error('Failed to trigger scenario:', e);
        }
    }

    // Bind UI Events
    function bindEvents() {
        // Audio Toggle
        elements.btnAudioToggle.addEventListener('click', () => {
            state.audioEnabled = !state.audioEnabled;
            elements.audioIcon.textContent = state.audioEnabled ? '🔊' : '🔇';
            elements.btnAudioToggle.innerHTML = `<span id="audio-icon">${state.audioEnabled ? '🔊' : '🔇'}</span> Siren ${state.audioEnabled ? 'Enabled' : 'Muted'}`;
        });

        // Scenario Buttons
        document.querySelectorAll('.btn-scenario').forEach(btn => {
            btn.addEventListener('click', () => {
                document.querySelectorAll('.btn-scenario').forEach(b => b.classList.remove('active'));
                btn.classList.add('active');
                const scenario = btn.getAttribute('data-scenario');
                if (scenario) triggerScenario(scenario);
            });
        });

        // Manual SOS
        const btnSos = document.getElementById('btn-manual-sos');
        if (btnSos) {
            btnSos.addEventListener('click', () => {
                triggerScenario('T8');
            });
        }

        // Toggle Config Accordion
        elements.toggleConfigBtn.addEventListener('click', () => {
            const isHidden = elements.configPanelBody.style.display === 'none';
            elements.configPanelBody.style.display = isHidden ? 'block' : 'none';
            elements.configArrow.textContent = isHidden ? '▲ Collapse' : '▼ Expand';
        });

        // Config Form Submit
        elements.formConfig.addEventListener('submit', async (e) => {
            e.preventDefault();
            const configPayload = {
                weight_o2: parseFloat(document.getElementById('cfg-w-o2').value),
                weight_h2s: parseFloat(document.getElementById('cfg-w-h2s').value),
                weight_co: parseFloat(document.getElementById('cfg-w-co').value),
                weight_ch4: parseFloat(document.getElementById('cfg-w-ch4').value),
                weight_water: parseFloat(document.getElementById('cfg-w-water').value),
                weight_motion: parseFloat(document.getElementById('cfg-w-motion').value),
                threshold_safe_max: parseFloat(document.getElementById('cfg-th-safe').value),
                threshold_warning_max: parseFloat(document.getElementById('cfg-th-warn').value),
                o2_override_threshold: parseFloat(document.getElementById('cfg-th-o2-over').value)
            };

            elements.cfgFeedbackMsg.textContent = 'Pushing to belt over MQTT...';
            try {
                const res = await fetch(`/api/v1/devices/${state.deviceId}/config`, {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify(configPayload)
                });
                if (res.ok) {
                    elements.cfgFeedbackMsg.textContent = '✓ Synchronized to device NVS!';
                    setTimeout(() => { elements.cfgFeedbackMsg.textContent = ''; }, 3000);
                }
            } catch (err) {
                elements.cfgFeedbackMsg.textContent = 'Error updating configuration';
            }
        });
    }

    // Initialization
    function init() {
        initChart();
        bindEvents();
        connectWebSocket();
        setInterval(pollFallbackData, 3000);
        // Initial load
        triggerScenario('T1');
    }

    window.addEventListener('DOMContentLoaded', init);
})();
