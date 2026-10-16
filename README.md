# Arduino Color Memory Game

An interactive, "Simon Says" memory game built with an Arduino. This project challenges players to remember and repeat increasingly complex sequences of LEDs and sounds. It utilizes a custom array-based sequence generation and synchronized audio-visual hardware feedback to create a simple mini-game.

This project also demonstrates custom driver implementation for I2C devices by utilizing a Waveshare 1602 LCD module running on an AiP31068 controller.

---

## Features

- **Classic Arcade Logic:** dynamic levels of increasing difficulty. Random sequences are generated dynamically via an analog noise seed for a unique game every time.
- **I2C Display Integration:** Uses a custom Waveshare 1602 LCD to provide real-time game status, level tracking, and win/loss feedback, without blocking the main loop.
- **Instant Audio-Visual Feedback:** Each button press triggers an immediate, synchronized LED flash and a unique buzzer frequency corresponding to the chosen color.
- **Input Debouncing:** Custom button-polling logic ensures that holding down a button isn't accidentally registered as multiple inputs.

---

## How to Play

The LCD screen will guide you through the game states:

1. **Start:** The screen will display "Press Any Key". Press any of the 4 colored buttons to begin Level 1.
2. **Watch:** The screen will display "Watch Pattern". Pay close attention as the Arduino flashes a sequence of colored LEDs and plays corresponding tones.
3. **Repeat:** When the screen says "Your Turn !", press the buttons in the exact order you just saw.
4. **Advance:** If you are correct, the screen will say "Correct!" and advance you to the next, longer sequence. 
5. **Win/Lose:** Complete all levels to Win. Pressing a wrong button at any point resets the game to level 1.

---

## Hardware Required
- **Microcontroller:** Arduino UNO (or Nano/Mega).
- **Display:** Waveshare LCD1602 I2C Module (AiP31068 controller - `0x3E`).
- **Inputs:** 4x Tactile Push Buttons (Red, Yellow, Green, Blue preferably).
- **Outputs:** 4x LEDs (eg. Red, Yellow, Green, Blue), 1x Buzzer.
- **Miscellaneous:** Breadboard, Jumper Wires, 4x 220Ω-330Ω Resistors (for the LEDs).

---

## Wiring Instructions

### 1. Power & Ground Bus
- Connect Arduino **5V** to the **`+` (power) rail** of the breadboard.
- Connect Arduino **GND** to the **`-` (ground) rail** of the breadboard.

### 2. The I2C Display
- Connect **VCC** on the LCD to the **5V rail**.
- Connect **GND** on the LCD to the **GND rail**.
- Connect **SDA** on the LCD to Arduino pin **A4**.
- Connect **SCL** on the LCD to Arduino pin **A5**.

### 3. Inputs (Buttons)
- **Red Button:** Connect to Arduino Pin **6**.
- **Yellow Button:** Connect to Arduino Pin **8**.
- **Green Button:** Connect to Arduino Pin **10**.
- **Blue Button:** Connect to Arduino Pin **12**.

### 4. Audio / Visual Feedback (Outputs)
- **Red LED:** Positive leg to Pin **5**. Negative leg through a resistor to **GND**.
- **Yellow LED:** Positive leg to Pin **7**. Negative leg through a resistor to **GND**.
- **Green LED:** Positive leg to Pin **9**. Negative leg through a resistor to **GND**.
- **Blue LED:** Positive leg to Pin **11**. Negative leg through a resistor to **GND**.
- **Passive Buzzer:** Positive leg to Pin **13**. Negative leg to **GND**.

---

## File Structure

```text
arduino-color-memory/
├── WaveshareLCD.h      # Custom driver header for the Waveshare 1602 display (AiP31068)
├── ColorMemory.ino     # Main sketch / State Machine & hardware logic
└── README.md           # Project documentation
```
---

## How to Run

1. Open `memoryGame.ino` in the **Arduino IDE**.
2. Ensure `WaveshareLCD.h` is located in the same directory as `memoryGame.ino` 
3. Verify the I2C address in `WaveshareLCD.h` matches your module:
   ```cpp
   #define LCD_ADDR 0x3E  // Default Waveshare address (change to 0x27 if needed)
   ```
4. Select your Board (e.g., **Arduino Uno**) under **Tools > Board**.
5. Select your COM port under **Tools > Port**.
6. Click **Upload**.

---

## License

This project is open-source and available under the [MIT License](LICENSE).
