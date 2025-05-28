# 🩸 Insulin Pump Simulation — Tandem t:slim X2 with Control IQ

A comprehensive insulin pump simulator replicating Tandem t:slim X2 functionality with Control IQ automated insulin delivery.  
Built with C++ and Qt, it demonstrates embedded systems design patterns like Observer and State Machines through a full-featured GUI and simulation backend.

---

## 🎯 Purpose

This project simulates real-world insulin pump operations, including personal profile management, bolus and basal insulin delivery, CGM integration, and error handling. It enables users to:

- Create, view, update, and delete personal insulin profiles (basal rates, carb ratios, correction factors, glucose targets)  
- Deliver manual boluses (immediate or extended) with dose recommendations based on user input  
- Automate basal insulin adjustments with Control IQ logic using CGM data  
- Monitor insulin delivery history and pump status (battery, cartridge, alerts)  
- Handle critical errors with safe insulin suspension and recovery guidance  
- Visualize blood glucose trends and insulin events in a Qt GUI closely resembling the real pump UI  

---

## 🗺 Features & Use Cases

- **Manage Personal Profiles (CRUD):** Full create/read/update/delete support with validation and logging  
- **Manual Bolus Delivery:** Immediate and extended bolus modes, dose calculation, user overrides, cancellation  
- **Basal Insulin Control:** Start, stop, resume basal delivery manually or automatically via Control IQ  
- **Control IQ Algorithm:** State machine adjusts basal rate and delivers automatic correction boluses based on predicted glucose  
- **Pump History & Visualization:** Event logs, blood glucose graphs, and detailed status display  
- **Error Handling:** Detect and alert low battery, low insulin, occlusions, CGM disconnects; suspend insulin delivery safely  
- **GUI:** Qt-based touchscreen interface modeled on the Tandem t:slim X2 UI with smooth navigation and real-time updates  

---

## 🛠 Tech Stack & Design

- **C++** for high-performance simulation logic  
- **Qt Framework** for cross-platform GUI development  
- **Design Patterns:**  
  - **Observer:** For event-driven UI updates and real-time sensor data propagation  
  - **State Machines:** Robust control flow for basal delivery, bolus management, error handling, and Control IQ automatic adjustments  
- **Modular Architecture:** Separate controllers for bolus, basal, profiles, delivery, and error management  
- **Logging:** Comprehensive event logging for traceability and debugging  

---

## 🧠 System Flow (High-Level)

1. **User Interaction:** Via touchscreen GUI, users manage profiles, trigger bolus delivery, start/stop basal insulin  
2. **Simulation Core:**  
   - Profiles feed into bolus and basal calculation modules  
   - CGM sensor interface supplies glucose data for Control IQ adjustments  
   - InsulinDeliveryManager executes insulin dosing commands  
3. **Control IQ Logic:**  
   - Periodically predicts future glucose trends  
   - Adjusts basal rates or triggers automatic correction boluses via state machine  
4. **Event Logging & UI Update:** All actions and system states logged; GUI updated accordingly  
5. **Error Handling:** Monitors pump status; suspends insulin delivery on critical faults and prompts user intervention  

---

## 🚀 Getting Started

### Prerequisites

- Linux environment (Ubuntu recommended) or compatible POSIX OS  
- Qt framework installed (version X.X or later)  
- C++17 compatible compiler (e.g., g++)  
- [Optional] Cross-compilation setup for embedded targets   

### Build and Run

1. Clone this repository:
```bash
git clone https://github.com/yourusername/InsulinPumpSimulator.git
cd InsulinPumpSimulator
```
2. Build with Qt and make:
```
qmake InsulinPump.pro
make
```
3. Launch:
```
./InsulinPumpSimulator
```

---

📁 Project Structure
```
├── include/                     # Header files for classes and interfaces  
├── src/                         # Implementation files (.cpp)  
│   ├── Alarm.cpp                # Alert and alarm management  
│   ├── AlertManager.cpp         # Central alert handling  
│   ├── BasalSegment.cpp         # Basal rate scheduling segments  
│   ├── Battery.cpp              # Battery status simulation  
│   ├── BolusCalculator.cpp      # Bolus dose calculations  
│   ├── BolusManager.cpp         # Bolus delivery coordination  
│   ├── Cartridge.cpp            # Insulin cartridge simulation  
│   ├── CGMSensorInterface.cpp   # Continuous Glucose Monitor interface  
│   ├── ControlIQController.cpp  # Control IQ algorithm implementation  
│   ├── DataLogger.cpp           # Event and status logging  
│   ├── InsulinDeliveryManager.cpp # Insulin delivery control  
│   ├── main.cpp                 # Application entry point  
│   ├── MergedMainWindow.cpp     # Qt GUI implementation  
│   ├── Profile.cpp              # Insulin profile data model  
│   ├── ProfileCRUDController.cpp # Profile management controller  
│   ├── ProfileManager.cpp       # Profile storage and retrieval  
│   ├── PumpSimulator.cpp        # Core pump simulation logic  
│   ├── PumpTester.cpp           # Test harness for backend  
├── InsulinPump.pro              # Qt project file  
├── Makefile                    # Build instructions  
├── README.md                   # Project overview and setup instructions  
```

---

## 📚 Design Highlights
### Observer Pattern
* UI components subscribe to simulation data streams (e.g., glucose updates, delivery status)
* Changes in simulation state trigger UI refreshes without tight coupling

### State Machines
* Basal delivery, bolus delivery, and error handling are governed by well-defined state machines
* Enables robust handling of complex pump workflows and safe transition between states

---

## 🎥 Demo
Watch the demo video on [YouTube](https://www.youtube.com/watch?v=Xr9BlT7fJSU&ab_channel=AhmedElnimah).