import platform
import subprocess
import serial

import os

import serial.tools.list_ports as port_list
ports = list(port_list.comports())
for p in ports:
    print (p)

# thank you gemini
def find_first_python_file(directory):
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith(".bin"):
                return os.path.join(root, file)
    return None

print("Searching for .bin build file in directory `cmake-build-debug`")
binpath = find_first_python_file("cmake-build-debug")

if not binpath:
    print("No binary file found, make sure you built the code correctly and it is in the `cmake-build-debug` directory.")
    exit(-1)

print("Found file at:", binpath, "\n")

# if(platform.system() == "Darwin"):
#     # ser = serial.Serial('/dev/tty.')
#     # ser.baudrate = 128000 # 12 Mb/s
# result = subprocess.run(["dfu-util", "-a 0 -d 0483:df11 --dfuse-address=0x08000000 -D", binpath], capture_output=True, text=True) # dfu-util -a 0 -d 0483:df11 --dfuse-address=0x08000000 -D cmake-build-debug/upright-2025.bin
res = os.popen("dfu-util -a 0 -d 0483:df11 --dfuse-address=0x08000000 -D " + binpath).read()
print(res)