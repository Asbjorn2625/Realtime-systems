import serial
import time
# opens connections to arduino
arduino = serial.Serial(port='COM4', baudrate=9600, timeout=.01)
time.sleep(0.1)
arduino.flushInput()
# number written to the arduino to synchronise python and the arduino, if it does'nt try putting a 1 inside the "".
sync = "1"


# reformating the serial read from the arduino
def conversion():
    data = str(arduino.readlines())
    data=data.split("'")
    if len(data) > 1:
        data = data[1].split("\\")
        return data[0]
    else: 
        return "00000000"
def binary2int(binary):
    """convert binary string to integer"""
    binary = binary[::-1]
    result = 0 
    for i in range(0,len(binary)):
        result += int(binary[i]) * 2 ** i
    return result
def convertto8bit(binary):
    """converts binary to 8 bit"""
    while len(binary) < 8:
        binary = "0" + binary
    return binary

    
    
        
state = 0
receiverID = []
RSSI = []
TXID_low = []
TXID_midlle = []
TXID_high = []
packagecertification = False
quiet_time1 = 1
recieverIDCnt = 0
RSSICnt= 0
TXIDCnt = 0
checkcnt = 0
start_byte = "2"
end_byte = "3"

while(True):
    time.sleep(int(quiet_time1))
    packet = conversion()
    arduino.write(sync.encode())
    
    if state == 0:
        print("State: Start_byte")
        if packet == start_byte:
            state = 1
            continue
    if state == 1:
        print("State: Length")
        length = packet
        state = 2
        continue
    if state == 2:
        print("State: type")
        type = packet
        state = 3
        continue
    if state == 3:
        print("State: Receiver id")
        if recieverIDCnt < 3:
            receiverID.append(packet)
        recieverIDCnt += 1
        if len(receiverID) >= 3:
            state = 4
            continue
    if state == 4:
        print("State: Transducer")
        Transducer = packet
        state = 5
        continue
    if state == 5:
        print("State: Quiet Time")
        quiet_time = packet
        state = 6
        continue
    if state == 6:
        print("State: Masters ")
        masters_in_range = packet
        state = 7
        continue
    if state == 7:
        print("State: RSSI")
        if RSSICnt < int(masters_in_range):
            RSSI.append(packet)
        RSSICnt += 1
        if len(RSSI) >= int(masters_in_range):
            state = 8
            continue
    if state == 8:
        print("State: TXID_low")
        print(TXIDCnt)
        if TXIDCnt < int(masters_in_range):
            TXID_low.append(packet)
            TXIDCnt += 1
        if len(TXID_low) == int(masters_in_range):
            state = 9
            TXIDCnt = 0
            continue
    if state == 9:
        print("State: TXID_mid")
        if TXIDCnt < int(masters_in_range):
                TXID_midlle.append(packet)
                TXIDCnt += 1
        if len(TXID_midlle) == int(masters_in_range):
            state = 10
            TXIDCnt = 0
            continue
    if state == 10:
        print("State: TXID: High")
        if TXIDCnt < int(masters_in_range):
                TXID_high.append(packet)
                TXIDCnt += 1
        if len(TXID_high) == int(masters_in_range):
            state = 11
            TXIDCnt = 0
            continue
    if state == 11:
        print("State: Checksum")
        if checkcnt == 0:
            checksumlower = bin(int(packet))
            checksumlower = checksumlower.replace("0b","")
            checksumlower = convertto8bit(checksumlower)
            checkcnt += 1
            continue
        elif checkcnt >= 1:
            checksumupper = bin(int(packet))
            checksumupper = checksumupper.replace("0b","")
            checksumupper = convertto8bit(checksumupper)
            recievedChecksum = str(checksumupper) + str(checksumlower)
            print(binary2int(recievedChecksum))
            #calculate the checksum
            checksum = int(start_byte) + int(length) + int(type) + int(receiverID[0]) + int(receiverID[1]) + int(receiverID[2]) + int(Transducer) + int(quiet_time) + int(masters_in_range) + int(end_byte)
            for i in range(0,int(masters_in_range)):
                checksum += int(RSSI[i])
                checksum += int(TXID_low[i])+int(TXID_midlle[i])+int(TXID_high[i])
                if checksum == binary2int(recievedChecksum):
                    packagecertification = True
            state = 12
        continue

    if state == 12:
        print("State: End byte")
        print(packet)
        if packet == end_byte:
            if packagecertification == True:
                print("Package certified")
                #do something with the data
                print("message type:")
                print(type)
                print("Receiver ID:")
                print(receiverID)
                print("Transducer:")
                print(Transducer)
                print("Quiet time:")
                print(quiet_time)
                print("Masters in range:")
                print(masters_in_range)
                print("RSSI:")
                print(RSSI)
                print("message length")
                print(length)
                packagecertification = False
            state = 0
            checksum=0
            #reset the variables
            receiverID = []
            RSSI = []
            TXID_low = []
            TXID_midlle = []
            TXID_high = []
            recieverIDCnt = 0
            RSSICnt= 0
            TXIDCnt = 0
    
            

   
    