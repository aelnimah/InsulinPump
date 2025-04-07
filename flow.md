# 💡 Project Flow Summary (flow.md)

---

## ✅ Overview
You're transitioning from Use Case 1 (**Manage Personal Profiles**) to Use Case 2 (**Deliver Manual Bolus**) in your insulin pump simulator. You’ve structured your code into:

- `mycode.txt`: Your working simulator, including profile CRUD logic and a Qt GUI.
- `groupcode.txt`: Team backend logic, including bolus calculations, insulin delivery, CGM, and ControlIQ logic.

---

## 📦 Your Local Code (`mycode.txt`)

### 💾 Backend Classes
- **Profile / ProfileManager**: Models insulin delivery profiles and manages active profiles.
- **ProfileCRUDController**: CRUD interface for the GUI to manipulate profiles.
- **PumpSimulator**: Main simulation controller that coordinates components. Currently uses `ProfileManager` and connects to GUI.

### 🖥️ UI Components (Qt)
- `MainWindow`: Central window using QStackedWidget to manage navigation.
- `OptionsWindow`, `PersonalProfilesWindow`, `AddProfileWindow`, `ViewProfileWindow`: Profile management interface.
- Well-connected navigation and signal-slot architecture for Use Case 1 is already in place.

---

## 🧠 Group Code (`groupcode.txt`)

### 🔬 Bolus Logic
- **BolusCalculator**:
  - `calculateBolus(...)`: Computes recommended insulin based on BG, carbs, IOB, and profile settings.
  - `calculateCorrectionBolus(...)`, `calculateExtendedBolusSplit(...)`

- **InsulinDeliveryManager**:
  - `deliverBolus(...)`, `getInsulinOnBoard()`, `startBasalDelivery(...)`, etc.
  - Handles insulin delivery modes (immediate, extended).

- **CGMSensorInterface**:
  - Simulates blood glucose readings (get/set/simulate).

- **ControlIQController** (optional, for future use):
  - Uses CGM to automate insulin adjustments.

- **PumpSimulator** (enhanced version):
  - Command-line simulation includes `bolus`, `setbg`, `basal`, etc.
  - Already connects to `BolusCalculator`, `ProfileManager`, and `InsulinDeliveryManager`

---

## 🧩 What You Need to Build (Use Case 2: Deliver Manual Bolus)

### ✅ Step 1: Refactor Backend
Create a new class `BolusManager` that:
- Accepts: `ProfileManager*`, `BolusCalculator*`, `InsulinDeliveryManager*`, `CGMSensorInterface*`
- Methods:
  - `double computeRecommendedDose(double bg, double carbs)`
  - `void deliverBolus(double dose, bool extended, double duration)`
  - Optionally: `double getBGFromCGM()`

### ✅ Step 2: Test Backend
Before integrating UI, build a test harness (CLI or unit test) that:
- Creates dummy profiles
- Simulates BG and carb inputs
- Outputs recommended dose and mocks delivery

### ✅ Step 3: UI Flow for Use Case 2
1. **Home Page** → Add new "Bolus" button next to "Options"
2. **Bolus Input Page**:
   - Manual BG + Carb input or auto-CGM read
3. **Recommended Dose Page**:
   - Show dose + allow override
4. **Confirmation Page**:
   - Choose Immediate or Extended delivery
5. **Logging & Return**:
   - Log bolus event, update IOB, return to home screen

---

## 🛠️ Integration Plan

| Phase | Goal | Notes |
|-------|------|-------|
| 1 | Build and test `BolusManager` backend | CLI/Unit test only |
| 2 | Design bolus flow UI pages | Use existing QStackedWidget |
| 3 | Connect UI to `BolusManager` | Trigger from "Bolus" button |
| 4 | Log bolus & update IOB | Reuse deliveryManager logic |
| 5 | Future: ControlIQ | Optional auto mode logic |

---

## ✅ Ready to Start
Once the three files (`mycode.txt`, `groupcode.txt`, `flow.md`) are ready in your new project folder, use the bootstrapping prompt and let's build this step-by-step.

Let's go! 🚀

