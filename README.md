# smartspeak-medinjector
Abstract:
1. This project work falls under wireless biomedical device and is designed with Arduino controller, which is able to monitor the temperature, heart beat rate (number of beats) of a patient continuously.
2. The controller is interfaced with the GSM modem to transmit a message in the form of SMS to the authorized mobile number along with the parameter details. 
3. By replying a SMS to the GSM modem, automatically medicine injector will be activated and injection will be given in case of emergency. A voice chip will announce if any of the parameter goes abnormal or the injection is given through the speaker.
Introduction:
The components used in our project are as follows:
1. L293D Motor Driver
2. Heart Beat Sensor
3. DHT sensor
4. Max30100 spo2 sensor
5. Arduino Uno
6. Syringe
7. 16x2 LCD
8. GSM SIM900A
9. Jumper Wires
Block Diagram:
![image](https://github.com/209x1a04d2/smartspeak-medinjector/assets/112796057/53e7932f-f90d-4995-94ef-376d30862dba)
Circuit Diagram
![image](https://github.com/209x1a04d2/smartspeak-medinjector/assets/112796057/08e0623c-7dcc-4ab2-b414-4dc94a9702ee)
Working Principle:
1. When temperature is above (39.4 Celsius or 103 Fahrenheit) and Heart beat rate is (greater than 120 beats per minute or lesser than 60 beats per minute) then the GSM module should send message to the doctor mobile number 
2. Then, when the doctor sends the message like “INJECT MEDICINE” then the dc motor should be activated to drive the syringe for injection. 
3. Syringe Action: The syringe should suck in the medicine(here take it as water) and should give out (suck out) medicine and this syringe should be connected to a small tube.
Hardware Components:
1. Arduino Uno:
![image](https://github.com/209x1a04d2/smartspeak-medinjector/assets/112796057/2491cb19-90da-4854-a074-845c41b7464d)
2. L293D Motor Driver:
![image](https://github.com/209x1a04d2/smartspeak-medinjector/assets/112796057/b37fe55b-06f1-44bd-8338-f7bf71baa57d)
3. DHT Sensor:
![image](https://github.com/209x1a04d2/smartspeak-medinjector/assets/112796057/1d34809d-d73c-4223-b4dd-7391ce9f2c04)
4. Heart Beat Sensor:
![image](https://github.com/209x1a04d2/smartspeak-medinjector/assets/112796057/293b8000-43ca-4145-b390-e1f6207e5b14)
5. Buzzer:
![image](https://github.com/209x1a04d2/smartspeak-medinjector/assets/112796057/ffe080f9-7b46-4a2d-9e5b-031fcd9a2773)
6. GSM SIM900A:
![image](https://github.com/209x1a04d2/smartspeak-medinjector/assets/112796057/fb81575e-14a2-4d3d-a2cc-4a673f1d25b6)
7. 2x16 LCD display:
![image](https://github.com/209x1a04d2/smartspeak-medinjector/assets/112796057/2dbd4db5-fb68-4abf-a55b-1df664e4b575)
Software Tools:
![image](https://github.com/209x1a04d2/smartspeak-medinjector/assets/112796057/42d0efe1-cde7-4c94-be85-b3572f513d38)
Working Model:
This is the working model of our project as shown.
![image](https://github.com/209x1a04d2/smartspeak-medinjector/assets/112796057/b7465d50-c23b-4202-9c57-b71e037eaed6)
Firstly we need to connect our GSM SIM900A module with the doctor's mobile number, so the 2x16 LCD display will display the following message.
![image](https://github.com/209x1a04d2/smartspeak-medinjector/assets/112796057/3aa6f428-b482-4297-ac38-9ec55280248e)
The DHT sensor and Heart Beat sensor are connected to the patient as follows and the 2x16 LCD will be continuously displaying the temperature and heart beat rate of the patient.
![image](https://github.com/209x1a04d2/smartspeak-medinjector/assets/112796057/fe089691-98cb-4ac6-9681-7bbea9c3a4b1)
Whenever the patient's body temperature (>= 39.4 degree centigrade) and heart beat rate (>=120 BPM and <= 60BPM) then the Arduino Uno will analyse the parameters and sends the message to the concerned doctor's mobile number through the GSM SIM900A module.
![image](https://github.com/209x1a04d2/smartspeak-medinjector/assets/112796057/35002514-49d4-4391-81f4-4042248c4708)
Then, when the doctor analysis the parameters and feels like to inject medicine then he will send the message as "inject" then the L293D Motor driver will drive the motor for syringe action.
![image](https://github.com/209x1a04d2/smartspeak-medinjector/assets/112796057/840a9ce8-67cb-4dfd-890e-d39eaa7ba634)
Advantages:
1. Improved Medication Adherence.
2. Reduced Risk of Medication Errors.
3. Remote Monitoring.
4. Cost-Effective.
Applications:
1. Home Medication Management
2. Elderly Care
3. Remote Patient Monitoring
4. Pediatric Care















