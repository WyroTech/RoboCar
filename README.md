# 🚘 RoboCar - WiFi Controlled Robot Car with Python Backend and Camera Streaming

This project provides the firmware and backend code for controlling an Elegoo Smart Robot Car (likely V4.0 or similar). It uses a three-tier architecture:

1.  **Robot:** An ESP32 microcontroller handles Wi-Fi, camera input, and communication with the Python backend (via WebSockets) and a local Arduino board (via JSON/Serial). The Arduino controls the motors.
2.  **Python Backend:** A Python script running on a separate computer acts as a web server and WebSocket server. It serves the frontend web interface, receives commands from the browser, relays them to the ESP32, and likely relays the camera stream from the ESP32 back to the browser.
3.  **Web Frontend:** A web-based interface (HTML/JS/CSS served by the Python backend) allows users to control the robot and view the camera feed.

The ESP32 connects to the Python backend over Wi-Fi using WebSockets. The communication between the ESP32 and the Arduino board uses JSON messages over a serial connection.

## Features

* **Wi-Fi Connectivity:** Robot connects via Wi-Fi to the central Python backend.
* **Python Web Service Backend:**
    * Serves the web control interface.
    * Acts as a central hub for communication (WebSockets).
    * Relays control commands from the browser to the robot (ESP32).
    * Relays real-time camera stream from the robot (ESP32) to the browser.
* **Web-Based Control Interface:** Simple interface for driving commands.
* **Real-Time Camera Streaming:** Live video feed displayed in the browser, proxied through the Python backend.
* **WebSocket Communication:** Used for low-latency communication between Browser <-> Python Backend and Python Backend <-> ESP32.
* **ESP32 Controller on Robot:** Manages Wi-Fi, WebSocket client connection, camera data capture/sending, and Arduino communication.
* **Arduino Motor Controller:** Receives JSON commands via Serial from ESP32 and manages the motor driver (L298N).
* **JSON Protocol:** Structured communication format between ESP32 and Arduino.

## Hardware Requirements

* **Elegoo Smart Robot Car Kit V4.0 (or compatible components):**
    * Robot Car Chassis, Motors, Wheels
    * L298N Motor Driver Board
    * Battery Holder & Batteries
    * Arduino UNO R3 (or compatible)
* **ESP32 Development Board with Camera:** (e.g., ESP32-CAM or ESP32 + OV2640)
* **Computer/Server:** A computer (PC, Laptop, Raspberry Pi) on the *same local network* as the robot to run the Python backend script.
* **Wiring:** Jumper wires for ESP32 <-> Arduino Serial, Arduino <-> L298N, ESP32 Camera, Power.
* **Wi-Fi Router:** An accessible 2.4GHz Wi-Fi network for both the robot and the Python backend server.

## Software & Dependencies

* **Python Backend:**
    * Python 3.x
    * Pip (Python package installer)
    * Python Libraries: You likely need libraries for web serving and WebSockets. Common choices include:
        * `websockets`
        * `Flask` or `FastAPI` (for the web server part)
        * `asyncio` (often used with websockets)
        * *(Potentially others like `opencv-python` if doing video processing in Python)*
    * **`requirements.txt`:** Create a `requirements.txt` file listing the necessary Python packages.
* **ESP32 Firmware:**
    * Arduino IDE or PlatformIO
    * ESP32 Board Support
    * Libraries:
        * `WiFi.h`
        * A **WebSocket Client** library (e.g., `WebSocketsClient`, `arduinoWebSockets`)
        * `ArduinoJson.h`
        * Camera driver library (`esp_camera.h`)
        * `HardwareSerial.h`
* **Arduino Firmware:**
    * Arduino IDE or PlatformIO
    * Arduino AVR Board Support
    * Libraries:
        * `ArduinoJson.h`
        * Motor control logic (custom or library)
        * `SoftwareSerial.h` or `HardwareSerial.h`
* **Web Browser:** Modern browser supporting WebSockets (Chrome, Firefox, etc.)

## Setup & Installation

1.  **Clone Repository:**
    ```bash
    git clone [https://github.com/WyroTech/RoboCar.git](https://github.com/WyroTech/RoboCar.git)
    cd RoboCar
    ```
2.  **Setup Python Backend:**
    * Navigate to the directory containing your Python backend script (e.g., `python_backend/`).
    * **(Recommended)** Create and activate a Python virtual environment:
        ```bash
        python -m venv venv
        source venv/bin/activate # Linux/macOS
        .\venv\Scripts\activate # Windows
        ```
    * Create a `requirements.txt` file listing needed packages (e.g., `websockets`, `flask`).
    * Install dependencies:
        ```bash
        pip install -r requirements.txt
        ```
3.  **Configure ESP32 Firmware:**
    * Open the ESP32 source code (e.g., `esp32_code/esp32_code.ino`).
    * Modify the Wi-Fi `ssid` and `password` variables.
    * **Crucially:** Modify variables defining the **Python backend server's IP address and port** (e.g., `PYTHON_SERVER_IP`, `PYTHON_SERVER_PORT`). The ESP32 needs to know where to connect.
4.  **Install Arduino/ESP32 Libraries:**
    * Use the Arduino IDE Library Manager or PlatformIO's `platformio.ini` to install the required libraries for both ESP32 and Arduino boards.
5.  **Wiring:**
    * Assemble the robot car.
    * Connect ESP32 Serial TX/RX to Arduino Serial RX/TX (with common GND).
    * Connect Arduino to L298N motor driver.
    * Connect ESP32 Camera.
    * Connect Power.
6.  **Flash Firmware:**
    * Upload the configured code to the ESP32 board.
    * Upload the code to the Arduino board.

## Usage

1.  **Find Python Server IP:** Determine the IP address of the computer that will run the Python backend (e.g., using `ipconfig` on Windows or `ifconfig`/`ip a` on Linux/macOS).
2.  **Start Python Backend:**
    * Open a terminal/command prompt on the server computer.
    * Navigate to the Python script's directory.
    * Activate the virtual environment (if used).
    * Run the backend script:
        ```bash
        python your_backend_script.name.py # Replace with your script's actual filename
        ```
    * Note the IP address and port the server reports it's running on (should match the IP found in step 1 and the port configured in the ESP32 code).
3.  **Power On Robot:**
    * Turn on the robot car.
    * Check the ESP32's Serial Monitor output to confirm it connects to Wi-Fi and successfully establishes a WebSocket connection to the Python backend.
4.  **Open Web Interface:**
    * On any device (laptop, phone) connected to the *same network*, open a web browser.
    * Navigate to the address of the Python backend server: `http://<PYTHON_SERVER_IP>:<PYTHON_SERVER_PORT>`.
5.  **Control:**
    * The web page served by the Python script should load.
    * Use the controls to drive the robot and watch the camera feed.

## Communication Architecture

1.  **Browser <-> Python Backend (WebSockets/HTTP):**
    * The browser loads the HTML/JS/CSS frontend from the Python HTTP server.
    * A WebSocket connection is established between the browser and the Python backend.
    * User actions (button clicks) send command messages (e.g., "forward") via WebSocket to the Python backend.
    * The Python backend streams camera data (received from ESP32) over the WebSocket back to the browser for display.
2.  **Python Backend <-> ESP32 (WebSockets):**
    * The ESP32 firmware initiates a WebSocket client connection *to* the Python backend server upon startup (using the configured IP/Port).
    * The Python backend relays commands received from the browser over the appropriate WebSocket connection to the connected ESP32.
    * The ESP32 captures camera frames and sends the data over its WebSocket connection *to* the Python backend.
3.  **ESP32 <-> Arduino (Serial + JSON):**
    * When the ESP32 receives a valid command from the Python backend, it translates it into a JSON message (e.g., `{"command":"left"}`).
    * This JSON string is sent via the Serial TX pin of the ESP32 to the Serial RX pin of the Arduino.
    * The Arduino listens for, receives, and parses the JSON message.
    * Based on the parsed command, the Arduino controls the motors connected via the L298N driver.

## Contributing

Contributions are welcome! Please open an issue or submit a pull request.

## License

Please add a LICENSE file to define usage rights (e.g., MIT, Apache 2.0).

## Acknowledgements

* Uses hardware from the Elegoo Smart Robot Car Kit V4.0.
