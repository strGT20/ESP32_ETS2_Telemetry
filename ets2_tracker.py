# pip install requests pyserial
import requests, time, serial, sys

PC_IP = "192.168.251.15"        # ganti
PORT = 25555
URL = f"http://192.168.251.15:25555/api/ets2/telemetry"
SERIAL_PORT = "COM13"          # ganti: Windows "COMx", Linux "/dev/ttyUSB0"
BAUD = 115200
POLL_INTERVAL = 0.12

try:
    ser = serial.Serial(SERIAL_PORT, BAUD, timeout=1)
    time.sleep(1)
except Exception as e:
    print("Gagal buka serial:", e); sys.exit(1)

while True:
    try:
        r = requests.get(URL, timeout=0.8)
        if r.status_code == 200:
            j = r.json()
            # Ambil fields umum (sesuaikan struktur JSON jika berbeda)
            speed = j.get("truck", {}).get("speed", 0.0)      # unit: km/h atau mph sesuai game
            rpm   = j.get("truck", {}).get("engineRpm", 0)
            ser.write(f"{speed:.1f},{int(rpm)}\n".encode())
        else:
            ser.write(b"0.0,0\n")
    except Exception as e:
        print("Err:", e)
        try: ser.write(b"0.0,0\n")
        except: pass
    time.sleep(POLL_INTERVAL)
