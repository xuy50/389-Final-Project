#Project Description
-------
**Materials:**<br>
Arduino UNO R3 board * 1<br>
Buttons * 9<br>
74HC595 Chip * 1<br>
LED * 8<br>
16*2 I2C LCD * 1<br>
Buzzer * 1<br>
Photoresistor * 1<br>
330Ω Resistors * 19<br>
<br>
**Description: <br>**
<br>
&emsp;&emsp;The project is toy that is a Simon game which is a memory game and a piano mode with a scale.<br>

&emsp;&emsp;First when the toy open or reset, the LCD will display the welcome interface, and then will show the options interface. The options interface can choose the Simon Game mode or the piano mode, and only when the toy reset or reopen the options interface will be shown. The first mode is Simon Game mode, this mode is a memory game which has 10 levels for player. The system will give player a order for light, and player will use the right order to push buttons, and the number of the lights order will add one for each level, only one light will light and one button need to be pushed for level one, and the buttons need to be pushed in right order ten times for level ten. Only when player pass the level 10, the player will get the game the clear and congratulation information interface. If the player pushes wrong button or did not push the buttons in right order, the game will fail, in the other case, there are 10 seconds for each level, if the player did not pass the current level in 10 seconds, the displayer will show the left time and current level for player, the game will fail. If the game fails, the displayer will show the final level player reach, and the failure reason, “Wrong order!” or “Time up!” The Simon Game mode will push the Start button to start game, and the start button is also the end button for Simon Game mode, when you are in the game, you can push the button to stop the current game, and it will back to the initial interface for the Simon Game. And there is a mode change button. The button can change the mode between the Simon Game and Piano Mode, whatever player is in anytime. When the player push the mode change button in the Simon game mode, the mode will change to the Piano Mode.<br>

&emsp;&emsp;The Piano Mode is like a normal piano with one scale, but because of the buzzer, it cannot implement the chord, and it is without the black keys. When player bush the keys, the LED that corresponds to the keys will light up. And when player pushes the mode change button, the mode will change to the Simon Game.<br>
<br><br>

#Project Detail and Build Description
-------
&emsp;&emsp;The beginning of the project, I tried how to use the buzzer to make different sound effects, and search how to do that, then I use the Diatonic table to write the head file "gamut.h" for the diatonic frequency for the buzzer, which will make the data easier to use multiple times and make the main program cleaner. Then, I tried to make some different tones and do the piano mode function to test different tones with different sounds.<br>

&emsp;&emsp;The piano function is a dead loop in the “loop()” function, and call the sub-function, “piano()”, for the Piano mode repeatedly. The button signal was read in “piano()” function. When different button is pushed, the function will check which one it is, then make the specific sound, and then return the specific number for the 74HC595 chip back to the “loop()” function to let the corresponding LED light when the button did not be released. At the beginning, I want to use the buzzer to implement the chord, but the result is the same as PWM, which make the sound was low and inaccuracy, I found that it is possibly because the buzzer does not support chord which make this case, then I change the code to one time one button and one sound, which fix the problem. The piano mode which is the basic part is complete. Because want to implement the piano, so I increase more buttons, and these buttons I also use for the Simon Game, which is not 4 buttons Simon Game, but become a 7 buttons Simon Game, I think it is more interesting and challenging than the original version. Then I create a variable “int mode” which is control the mode change, when the mode = 0, it will be Simon Game mode, if the mode = 1, the mode will be piano mode.<br>

&emsp;&emsp;Next, for the Simon Game, I need to get 10 random numbers which is between 1 and 7 for the random number array to implement each time game, for each time the numbers in the array are different which can give player more gameplay. For the random numbers, I want to use the Photoresistor which is use for the random number seed to get the real random number, but I found the photoresistor cannot drastic change for each time, which make the random number are same sometimes. Then I search the Arduino official website, and found the “int analogRead(0)” whose 0 is an analog input pin is not connected to anything, which is a mysterious random numbers with some element, but I found some information said that it is a pseudo-random numbers, so I use the photoresistor with the analogRead(0) to make the random seed, I think which is very close to the real random number, and I test many times to get the result that the random numbers in the array look like real random. After the random numbers array was implemented, the system can show the order with the LED and sound, and input and check the order with button by player, both operations are with the random number array.<br>

&emsp;&emsp;The sub-function “showSimon(int level)” is show the order by the LED and sound with the random number array. The function uses a loop whose counter controller is the current level, in the level one it will only loop one time, and in the level 10 it will loop 10 times. I use two variables, one is for LED light “int ledlight” and the other one is for tone frequency “int note”. Then in the loop, check the random number in the array to get its specific “ledlight” and “note”. After checking the number, in the loop it will make sound with the buzzer and number “note” and use the “ledlight” let the specific light, and delay 1.5 seconds in this state, and then close the sound and LEDs. After the loop finish, the player will get the order for this game and remember them to push buttons. Then I need to implement the push buttons part.<br>

&emsp;&emsp;I write a sub-function “int simon(int randomNums[10], int level)” to implement. I use a dead loop, because I need use the count down and keep trying to read the key data. There is a counter in the loop, each time the player push the button in right order, the counter will add 1, when the counter equals to the level, which means the player pass the level, then the sub-function will return int number 1 to “loop()” function, which will make level go up till passing the level 10, clear the game one time. If the player push the button wrong, it will return int number -2, which will told the main part that the player push the wrong button, then the game fails. Because I need a count down to control time, so in the loop it will check the time left, if time is up, it will return -1, the game also fails. When the game fails, all of the light will light, and the buzzer will make a sound for failure, and the displayer will show the reason why the game fail, “Time up” or “Wrong order!” and the final level player achieved. Then if player push the start/end button(button 8), the sub-function will return 8 to loop() function and the game will finish and go back to the start interface for the Simon game, when the player push the start/end button, the game will start. If player push the mode change button(button 9), it will return 9, and the game will also finish and change mode to piano mode.<br>

&emsp;&emsp;Then I think how to implement the time count down. I think it can use interrupt to implement, and I found the information that I can use the Timer1 clock in the Arduino board to control the interrupt, which need the library <TimerOne.h> and <Wire.h>. Then I initialize the Timer1 for 1 second. I create a variable “int simonTimer” which is timer counter. Then I use the Time1 to control the interrupt function “attachInterrupt(simonTime)”  to do the operation for each 1 second interrupt. I write the sub-function “simonTime()” when the interrupt happens, the timer counter “simonTimer” will -1. When the Simon Game finish phase of showing order, the “simonTimer” will become 10. By the Timer1 Interrupt, when the “simonTimer” = 0, the Simon Game will fail. And using the “simonTimer” write the sub-function for display the left time, when the player is in the push button phase, it will call the display function and show the left time in the LCD displayer.<br>
<br>
<br>
**#Project files:** <br>
gamut.h<br>
piano_simon_game.ino<br>
<br>
**#Extra libraries(need install for arduino IDE)(in the library folder):**<br>
LiquidCrystal_I2C.h<br>
TimerOne.h<br>
<br>
<br>
**#Sketch:**<br>
![image](https://github.com/xuy50/389-Final-Project/blob/main/sketch.png)
<br>
<br>
**#Demo link:**<br>
**https://youtu.be/Z1jW39UWck0<br>**
<br>
<br>
References:<br>
https://www.arduino.cc/en/Tutorial/Foundations<br>
http://jiader.blogspot.com/2018/08/arduino.html<br>

<br>
<br>

#Final-Project<br>
Final project proposal<br>
I want to build a Simon Game. It basically has 4 button with 4 LED like general Simon Game, and I will use the LCD to show what is the level the player in, and show how much time left the player can do this level, after the time finishes, the game will be over, and the LCD will show the result. I want to add a buzzer to play 4 kinds of sound effects for the Simon Game, and I want to add a failure sound effect for the game failure with buzzer. If the player press the buttons in the wrong order, the game will be over.<br>
<br><br>
