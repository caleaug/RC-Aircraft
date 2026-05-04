import serial
import time

#00:4B:12:EC:BC:F0
#CC:DB:A7:9F:7E:60

# --- CONFIGURATION ---
# Replace 'COM3' with your actual port. 
# On Linux/Mac, it usually looks like '/dev/ttyUSB0' or '/dev/cu.usbserial-xxx'
SERIAL_PORT = 'COM6' 
BAUD_RATE = 115200

def run_bridge():
    try:
        # Initialize Serial connection
        # timeout=1 ensures the script doesn't hang if no data comes in
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        
        # ESP32s often reset when the serial port opens; wait for it to boot
        print(f"Connecting to ESP32 on {SERIAL_PORT}...")
        time.sleep(2) 
        print("Connected! Waiting for ESP-NOW data...\n")

        while True:
            if ser.in_waiting > 0:
                # Read the line and decode from bytes to string
                raw_data = ser.readline().decode('utf-8').strip()
                
                if raw_data:
                    # Split by the comma we used in the Arduino code
                    parts = raw_data.split(',')
                    
                    if len(parts) == 3:
                        esp_id = parts[0]
                        temp = parts[1]
                        status = parts[2]
                        
                        print(f"[ESP-{esp_id}] Temp: {temp}°C | Status: {status}")
                    else:
                        # Handles cases where a line might be partial or corrupted
                        print(f"Raw Serial Output: {raw_data}")

    except serial.SerialException as e:
        print(f"Error: Could not open serial port {SERIAL_PORT}. {e}")
    except KeyboardInterrupt:
        print("\nStopping script...")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print("Serial connection closed.")

if __name__ == "__main__":
    run_bridge()
