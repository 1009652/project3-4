import serial
import time
from datetime import datetime
from datetime import date

arduino = serial.Serial(port='COM8', baudrate=9600, timeout=1)

def printReceipt(currentTransaction, currentAccount, currentCard, currentAmount):
    sendChoice(2)
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

        if data == "" or int(data) <= int(temp):
            data = temp
        else:
            temp = data

        data = int(data)
        print("data-bon: ", data)

        if(data == 0):
            output = "d," + currentDay
            writeOut(output)
        elif (data == 1):
            output = "t," + currentTime
            writeOut(output)
        elif (data == 2):
            output = "o," + str(currentTransaction)
            writeOut(output)
        elif (data == 3):
            output = "a," + str(currentAccount)
            writeOut(output)
        elif (data == 4):
            output = "c," + str(currentCard)
            writeOut(output)
        elif (data == 5):
            output = "h," + str(currentAmount)
            writeOut(output)
        elif (data == 6):
            break;


def sentNotes(notes50, notes20, notes10):
    sendChoice(1)
    temp = 0
    while True:
        readVal = arduino.readline()[:-2]
        data = str(readVal, 'UTF-8')
        #print(data)

        if data == "" or int(data) <= int(temp):
            data = temp
        else:
            temp = data
        data = int(data)
        print("data: ", data)

        if data == 0:
            output = "f," + str(notes50)
            writeOut(output)
        elif data == 1:
            output = "s," + str(notes20)
            writeOut(output)
        elif data == 2:
            output = "t," + str(notes10)
            writeOut(output)
            break


def writeOut(x):
    print("x: ", x)
    output = x
    outputBytes = str.encode(output)
    arduino.write(outputBytes)
    time.sleep(1)

def sendChoice(choice):
    temp = 9
    if choice == 1:
        output = 'm'
    elif choice == 2:
        output = 'r'
    while(True):
        readVal = arduino.readline()[:-2]
        data = str(readVal, 'UTF-8')
        print("data: ", data)

        if data == "":
            data = temp
        else:
            temp = data
        data = int(data)
        #print("data: ", data)
        print("output: ", output)
        writeOut(output)

        if data != -1:
            break



sentNotes(1, 0, 0)
printReceipt(2, 1306, 10, 220)
