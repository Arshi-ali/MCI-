
import serial
import matplotlib.pyplot as plt
from drawnow import drawnow

# === Setup Serial (Windows) ===
sinWaveData = serial.Serial('COM7', 115200, timeout=1)
plt.ion()

# === Data Storage ===
NUM_SAMPLES = 100
time_s = []
all_signals = [[], [], []]
signals = ['X', 'Y', 'Z']
plot_styles = ['r.-', 'b.-', 'g.-']
cnt = 0

# === Plotting Function ===
def makeFig():
    plt.clf()
    plt.title('Accelerometer Plot - Lab 09 Task 2')
    plt.grid(True)
    plt.xlabel('Time (s)')
    plt.ylabel('Acceleration (g)')
    plt.ylim(-2, 2)

    for i in range(3):
        plt.plot(time_s, all_signals[i],
                 plot_styles[i], label=signals[i])

    plt.legend(loc='upper left')

# === Main Loop ===
try:
    while True:
        line = sinWaveData.readline().decode(
            'utf-8', errors='ignore').strip()

        if not line:
            continue

        try:
            # Example: X: 0.005 g, Y: -0.008 g, Z: 1.001 g
            parts = line.split(',')

            if len(parts) != 3:
                continue

            values = [
                float(part.split(':')[1].replace('g', '').strip())
                for part in parts
            ]

        except (ValueError, IndexError):
            print('Skipped:', line)
            continue

        time_s.append(cnt * 0.1)
        cnt += 1

        for i in range(3):
            all_signals[i].append(values[i])

        if len(time_s) > NUM_SAMPLES:
            time_s.pop(0)
            for signal in all_signals:
                signal.pop(0)

        if cnt % 5 == 0:
            drawnow(makeFig)
            plt.pause(0.001)

except KeyboardInterrupt:
    print('Plot stopped.')

finally:
    sinWaveData.close()
