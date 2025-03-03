import platform
import serial.tools.list_ports
import os
import time

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


# thank you gemini
def find_first_file(directory):
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith(".bin"):
                return os.path.join(root, file)
    return None

print(bcolors.OKBLUE + "Building files" + bcolors.ENDC)
res1 = os.popen("cmake -B cmake-build-debug/ -DCMAKE_BUILD_TYPE=PRODUCTION")
print(res1.read())

res2 = os.popen("cmake --build cmake-build-debug/ --config PRODUCTION --target vcu-firmware-2025.elf -j 8")
print(res2.read())
build_status = res2.close()

if(build_status):
    build_status = os.waitstatus_to_exitcode(build_status)
    if build_status != 0:
        exit(127)

print(bcolors.OKCYAN + "Searching for .bin build file in directory `cmake-build-debug`" + bcolors.ENDC)
binpath = find_first_file("cmake-build-debug")

if not binpath:
    print("No binary file found, make sure the code correctly builds and it is in the `cmake-build-debug` directory.")
    exit(-1)

print(bcolors.OKGREEN + "Found file at:", binpath, "\n" + bcolors.ENDC)


print(bcolors.OKCYAN  + "Finding Serial port to send update request" + bcolors.ENDC)

ports = serial.tools.list_ports.comports()

serial_port = None
for port, desc, hwid in sorted(ports):
    if("lhre" in desc.lower()):
        print(bcolors.OKGREEN + f"Found device at port {port}: {desc} [{hwid}]" + bcolors.ENDC)
        serial_port = port

if(serial_port):
    print(bcolors.OKBLUE + "Sending device update command" + bcolors.ENDC)
    ser = serial.Serial(serial_port)
    ser.baudrate = 115200
    ser.write("update\n\r\0".encode('utf-8'))
    time.sleep(1)

# result = subprocess.run(["dfu-util", "-a 0 -d 0483:df11 --dfuse-address=0x08000000 -D", binpath], capture_output=True, text=True) # dfu-util -a 0 -d 0483:df11 --dfuse-address=0x08000000 -D cmake-build-debug/upright-2025.bin
res = os.popen("dfu-util -a 0 -d 0483:df11 --dfuse-address=0x08000000 -D " + binpath + "").read()
print(res)

print(bcolors.OKCYAN + "Resetting device" + bcolors.ENDC)
res2 = os.popen("dfu-util -a 0 -d 0483:df11 -s :leave")

print(res2.read())