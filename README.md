# LEDPanelDesktopClient
**Required**
Install OpenCV, pyserial, and PIL



Download project with
```
git clone https://github.com/Todd-Stein/CSU_Software_Engineering_Proj/ 
```
After download, run program the program. A user interface will appear. 
First enter the image file path in the space under "Select file below"
Then press "Open File". 
  If the file loads successfully, a message indicating "Image loaded successfully" will appear. The pixelated image will appear.
  If the file doesn't load, an "Invalid path" message will appear.
To send image to the LED matrix, choose the correct serial port in the drop down.
  If no options appear, press the "Refresh serial port list" button. 
Once a port is chosen, press "Open port" to confirm port.
Press send image to display the image on the LED Panel.
