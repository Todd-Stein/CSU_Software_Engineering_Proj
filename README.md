# LEDPanelDesktopClient
**Required**
Install OpenCV, pyserial, and PIL



Download project with
```
git clone https://github.com/Todd-Stein/CSU_Software_Engineering_Proj/ 
```

1. After download, run the program. A user interface will appear. 
<img width="1590" alt="Screenshot 2024-12-16 at 10 52 51 PM" src="https://github.com/user-attachments/assets/600aae50-bbd5-4145-8e4c-7d8b041d64f4" />

2. Enter the image file path in the space under "Select file below"
<img width="1603" alt="Screenshot 2024-12-16 at 10 57 19 PM" src="https://github.com/user-attachments/assets/7e57649b-5e58-422f-a203-6ac05af1a8c8" />

3. Press "Open File". 
  - If the file loads successfully, a message indicating "Image loaded successfully" will appear along with the pixelated version of the image.
<img width="1603" alt="Screenshot 2024-12-16 at 10 58 54 PM" src="https://github.com/user-attachments/assets/044afea8-08dc-4cc0-ac9c-ab8d85c9fd25" />

  - If the file doesn't load or there is an error with the path, an "Invalid path" message will appear.
<img width="259" alt="Screenshot 2024-12-16 at 11 00 27 PM" src="https://github.com/user-attachments/assets/db4b2d6d-2fed-4f57-8122-b705b5a85ae7" />

4. To send image to the LED matrix, choose the correct serial port in the drop down.
<img width="346" alt="Screenshot 2024-12-16 at 11 00 58 PM" src="https://github.com/user-attachments/assets/f3b23ee9-fde4-475b-9797-a7c9a952b516" />

  - If no options appear, press the "Refresh serial port list" button. 
<img width="256" alt="Screenshot 2024-12-16 at 11 03 01 PM" src="https://github.com/user-attachments/assets/dddaaded-b3c9-4093-a938-e1ef658951bf" />

5. Once a port is chosen, press "Open port" to confirm port.
6. Press send image to display the image on the LED Panel.
