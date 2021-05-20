# Importing Libraries
import serial
import time
from datetime import datetime
from datetime import date

arduino = serial.Serial(port='COM4', baudrate=9600, timeout=.1)

def writeOut(x):
    print(x)
    output = x
    outputBytes = str.encode(output)
    arduino.write(outputBytes)
    time.sleep(1)


def printReceipt(currentTransaction, currentAccount, currentCard, currentAmount):
    #Get time
    now = datetime.now()
    currentTime = now.strftime("%H:%M")
    #Get date
    today = date.today()
    currentDay = today.strftime("%d-%m-%Y")
    temp = 0
    while True:
        readVal = arduino.readline()[:-2]
        data = str(readVal, 'UTF-8')

        if(data == "" or int(data) <= int(temp)):
            data = temp
        else:
            temp = data

        print("data:")
        data = int(data)
        print(data)

        if(data == 0):
            output = "d," + currentDay
            writeOut(output)
        elif (data == 1):
            output = "t," + currentTime
            writeOut(output)
        elif (data == 2):
            output = "o," + currentTransaction
            writeOut(output)
        elif (data == 3):
            output = "a," + currentAccount
            writeOut(output)
        elif (data == 4):
            output = "c," + currentCard
            writeOut(output)
        elif (data == 5):
            output = "h," + currentAmount
            writeOut(output)
        elif (data == 6):
            break;
    #print(value)  # printing the value