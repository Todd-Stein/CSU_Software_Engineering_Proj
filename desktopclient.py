import serial
import serial.tools.list_ports
from serial import SerialException
import cv2 as cv
import tkinter as tk
from tkinter import *
from PIL import Image, ImageTk
import os
import numpy as np
import base64
from time import sleep


class DektopPanel():

    def __init__(self):
        self.root = tk.Tk()
        self.root.geometry('1600x900')
        self.root.title = "LEDPanelDesktopCLient"

        self.fileLabel = tk.Label(self.root, text="Select file below")
        self.fileLabel.pack()

        self.filePath = tk.StringVar()
        self.fileSelect = tk.Entry(self.root, textvariable=self.filePath)
        self.fileSelect.pack()
        self.pathValid = tk.Label(self.root, text="")
        self.pathValid.pack()

        self.fileCheckState = tk.IntVar()
        self.fileCheckButton = tk.Button(self.root, text="Open File", command = self.CheckPath)
        self.fileCheckButton.pack()

        self.sendWidth = 25
        self.sendHeight = 11

        self.sendImageArr = bytearray()
        
        self.prevImageArr = bytearray()
        for i in range(0, 200*200):
            self.prevImageArr+=(0).to_bytes()

        self.emptyImageArr = np.zeros((3, 3, 3), dtype=np.uint8)
        self.originalImage = self.emptyImageArr
        self.sendImage = self.emptyImageArr
        self.previewImage = self.emptyImageArr

        self.prevImageRGB = cv.cvtColor(self.previewImage, cv.COLOR_BGR2RGB)
        self.pilImage = Image.fromarray(self.prevImageRGB)
        self.pilImageTk = ImageTk.PhotoImage(self.pilImage)
        self.prevImageLabel = tk.Label(self.root, image=self.pilImageTk)
        self.prevImageLabel.pack()


        self.portsList = [p.device for p in serial.tools.list_ports.comports()]
        if (len(self.portsList)==0):
            self.portsList.append("NULL")
        self.ser = serial.Serial()
        self.ser.baudrate = 115200
        self.ser.timeout = 60

        self.portID = tk.StringVar(self.root)

        self.serialLabel = tk.Label(self.root, text="Current serial port:\t"+self.portID.get())
        self.serialLabel.pack()
        print(len(self.portsList))
        self.serialDropdown = tk.OptionMenu(self.root, self.portID, *self.portsList)
        self.serialDropdown.pack()
        self.refreshSerialListButton = tk.Button(self.root, text="Refresh Serial Port List", command=self.RefreshSerial)
        self.refreshSerialListButton.pack()
        self.openPortButton = tk.Button(self.root, text="Open Port", command=self.OpenSerialPort)
        self.openPortButton.pack()
        self.sendButton = tk.Button(self.root, text="Send Image", command=self.SendImage)
        self.sendButton.pack()
        #self.previewImage
        #self.senDataButton
        

        self.root.mainloop()
    def CheckPath(self):
        if(os.path.exists(self.filePath.get())):
            print("path exists")
            self.originalImage = cv.imread(self.filePath.get())
            if(self.originalImage is None):
                self.pathValid.config(text="Not a valid image.")
                print("Cannot open file")
            else:
                self.pathValid.config(text="Image loaded successfully")
                self.ScaleImage()
                print(len(self.sendImageArr))
        else:
            self.pathValid.config(text="Invalid path")
    def RefreshSerial(self):
        self.portsList = [p.device for p in serial.tools.list_ports.comports()]
    def OpenSerialPort(self):
        self.ser.port = self.portID.get()
        try:
            self.ser.open()
            self.serialLabel.config(text="Current serial port:\t"+self.portID.get())
            print("port open")
            getDims = "GET".encode('utf-8')
            inDims = bytearray()
            self.ser.write(getDims)
            sleep(2)
            while self.ser.in_waiting>0:
                inDims+=self.ser.read()
            self.sendWidth = inDims[0]
            self.sendHeight = inDims[2]
            self.ScaleImage()
            print(inDims)
        except SerialException:
            print("unable to open port")
    def ScaleImage(self):
        self.sendImage = cv.resize(self.originalImage, (self.sendWidth, self.sendHeight), interpolation=cv.INTER_NEAREST)
        self.previewImage = cv.resize(self.sendImage, (250, 110), interpolation=cv.INTER_NEAREST)
        self.sendImageArr = bytearray()
        self.sendImageArr = self.sendImage.tobytes()

        self.previewImageArr = bytearray()
        self.previewImageArr = self.previewImage.tobytes()

        self.prevImageRGB = cv.cvtColor(self.previewImage, cv.COLOR_BGR2RGB)
        self.pilImage = Image.fromarray(self.prevImageRGB)
        self.pilImageTk = ImageTk.PhotoImage(self.pilImage)
        self.prevImageLabel.config(image=self.pilImageTk)

    def SendImage(self):
        start = ("ST0").encode('utf-8')
        self.ser.write(start)
        sleep(2)
        newArr=bytearray()
        for i in self.sendImageArr: 
            newArr+=i.to_bytes()
            if len(newArr):
                self.ser.write(newArr)
                sleep(.01)
                #print(self.ser.readline())
                newArr=bytearray()
        sleep(5)
        
        #while(self.ser.in_waiting==0):
         #   print("waiting")
        print(self.ser.readline())
DektopPanel()