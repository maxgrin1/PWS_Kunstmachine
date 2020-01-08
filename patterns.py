#Modules

import serial
import random
import math #needed for using square root for example
import turtle #needed for drawing purposes

#Variables
t = 0
T = 1000
x = 0 #x-coordinate
y = 0 #y-coordinate
z = 1 #scale
zz = 1
c = 0 #dont touch, for looping
d = random.random()*2+1
a = 300
angle = 0
margin = 5
turtle.pensize(0.1)

distList = []
angleList = []

print(d)

#Part 3

turtle.penup()
turtle.setpos(0*z,0*z)
turtle.pendown()
for t in range(0, T): #loops untill the max amount of coordinates are made
    turtle.speed(0) #speeds up the process

    x = -math.cos(t*d)*z*zz
    y = math.sin(t*d)*z*zz

    t += 0.001
    z += 1.5
    dist = round(math.sqrt(x**2+y**2)*100,0)
    if (dist/100 <= a):
      turtle.goto(x,y)
      x += 0.0001
      angle = round(math.degrees(math.atan(y/x)),0)
      distList.append(dist)
      angleList.append(angle)
      f= open("file.txt","w+") #opens the text file with write permission. If it doesnt exist it will make a new one.
      f.write("Angles:"+str(angleList)+"\n"+str(len(distList))+"Distances:"+str(distList)) #writes list of coordinates to the text file
      f.close() #closes the text file and saves it
