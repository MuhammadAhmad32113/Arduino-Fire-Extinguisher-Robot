# 🤖 Autonomous Fire-Extinguisher Robot

An autonomous ground vehicle designed to detect and suppress small fires without human intervention. Built as part of a professional undergraduate portfolio in Robotics and Artificial Intelligence.

---

## 📋 1. Project Overview
The Fire Extinguisher Robot is an autonomous ground vehicle designed to locate the direction of a fire using three infrared (IR) flame sensors, navigate toward it, and activate a water pump to extinguish the flame. A servo-controlled nozzle sweeps horizontally to improve water coverage. This repository documents the hardware components, circuit connections, power architecture, and operational logic of the system.

---

## 🎯 2. Project Objectives
* Autonomously detect flame direction using three IR flame sensors.
* Navigate a 4-wheel DC motor chassis toward the detected fire.
* Activate a submersible water pump via a relay to extinguish the fire.
* Sweep the nozzle using a servo motor for wider suppression coverage.
* Operate entirely on battery power without external connections.

---

## 🛠️ 3. Hardware Components
| Component | Qty | Role / Function |
| :--- | :---: | :--- |
| **Arduino Uno (ATmega328P)** | ×1 | Main microcontroller — runs all logic, reads sensors, controls motors and pump |
| **IR Flame Sensor Module** | ×3 | Detects infrared radiation from flames (Right A0, Front A1, Left A2) |
| **L298N H-Bridge Motor Driver** | ×1 | Drives 4 DC gear motors — accepts PWM for variable speed |
| **DC Gear Motor (5V–9V)** | ×4 | Chassis locomotion motors (2 left, 2 right) |
| **Rubber Wheels** | ×4 | Mounted on DC motors for traction |
| **Micro Servo (SG90)** | ×1 | Sweeps water nozzle horizontally to aim at flame |
| **5V Submersible Water Pump** | ×1 | Pumps water through tube to nozzle for fire suppression |
| **5V Single-Channel Relay** | ×1 | Electronically switches pump on/off via Arduino (Active-Low) |
| **3-Cell Li-Ion Battery Pack** | ×1 | 11.1V — powers L298N and DC gear motors |
| **1-Cell Li-Ion Battery** | ×1 | 3.7–4.2V — powers Arduino, sensors, servo, and pump |
| **Flexible Tubing** | ×1 | Carries water from pump reservoir to nozzle |
| **Mini Water Reservoir** | ×1 | Bucket holding water supply for the pump |

---

## 🔌 4. Pin Configuration & Wiring
All analog sensor lines run at 5V logic; the relay is active-low (logic LOW = pump ON).

| Component | Component Pin | Arduino Pin | Pin Type | Description |
| :--- | :--- | :---: | :---: | :--- |
| **Right Flame Sensor** | AO (Analog Out) | A0 | Analog Input | Reads right flame intensity |
| **Front Flame Sensor** | AO (Analog Out) | A1 | Analog Input | Reads front flame intensity |
| **Left Flame Sensor** | AO (Analog Out) | A2 | Analog Input | Reads left flame intensity |
| **Micro Servo SG90** | PWM Signal (Yellow) | A4 | PWM Output | Controls nozzle sweep angle |
| **5V Relay Module** | IN (Trigger) | A5 | Digital Output | LOW = Pump ON / HIGH = Pump OFF |
| **L298N Motor Driver** | ENA | 10 | PWM Output | Right motor speed (PWM) |
| **L298N Motor Driver** | IN1 | 9 | Digital Output | Right motor direction A |
| **L298N Motor Driver** | IN2 | 8 | Digital Output | Right motor direction B |
| **L298N Motor Driver** | IN3 | 7 | Digital Output | Left motor direction A |
| **L298N Motor Driver** | IN4 | 6 | Digital Output | Left motor direction B |
| **L298N Motor Driver** | ENB | 5 | PWM Output | Left motor speed (PWM) |

---

## ⚡ 5. Power Architecture
* **5.1 11.1V Rail — 3-Cell Lithium-Ion Pack:** Connected directly to the L298N 12V input terminal. Drives all four DC gear motors through the H-bridge. This rail is kept separate from logic to avoid voltage spikes.
* **5.2 3.7–4.2V Rail — 1-Cell Lithium-Ion:** Powers the Arduino Uno via the VIN / 5V pin. Supplies the three IR flame sensor modules. Supplies the SG90 servo motor. Powers the isolated side of the submersible water pump through the relay NO contact.
* **5.3 Common Ground:** The negative terminals of both battery packs and the Arduino GND pin are all connected together to form a single common ground reference. This is essential to ensure that all voltage levels are measured relative to the same reference, preventing erratic sensor readings and logic errors.

---

## ⚙️ 6. System Operation Logic
The Arduino firmware continuously polls the three IR flame sensors (A0, A1, A2). Lower analog readings indicate a stronger flame signal. The control logic proceeds as follows:
* If only the right sensor detects flame $\rightarrow$ turn right.
* If only the left sensor detects flame $\rightarrow$ turn left.
* If the front sensor (or both side sensors simultaneously) detects flame $\rightarrow$ move forward.
* When the robot is close enough (sensor value below threshold) $\rightarrow$ stop motors, activate relay (pump ON), sweep servo.
* When no flame is detected $\rightarrow$ stop all actuators and wait.

---

## 🛡️ 7. Safety Considerations
* Keep the water reservoir sealed to prevent spills onto electronics.
* Do not operate near voltages higher than rated values — L298N is rated for max 46 V, 2 A per channel.
* Lithium-ion cells must never be discharged below 3.0 V to prevent cell damage.
* Ensure all wires are securely insulated to avoid short circuits.
* Test in a controlled environment with small, contained flames only.

---

## 🏁 8. Conclusion
The Fire Extinguisher Robot successfully integrates sensing, motion control, and actuation into a compact autonomous system. The three-sensor array provides directional awareness, while the relay-controlled pump and servo-swept nozzle deliver effective fire suppression. The dual-battery architecture cleanly separates high-current motor loads from sensitive logic circuitry, ensuring reliable operation. Future improvements could include ultrasonic obstacle avoidance, a larger water reservoir, PID-based motor speed control, and wireless monitoring via Bluetooth or Wi-Fi.
