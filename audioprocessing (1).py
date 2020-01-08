# Modules

import serial
import math  # needed for using square root for example
import turtle  # needed for drawing purposes

# Variables

x = 0  # x-coordinate
y = 0  # y-coordinate
angle = 0
sample = 0
t = 0
dt = 0.6
step = 0
next = 1
z = 100  # scale
c = 0  # for looping
e = 10000  # amount of bytes read
a = 0  # used for grouping the splitted pieces
d = 5  # amount of strings that get combined
b = a + d  # used for grouping the splitted pieces
pi = 3.1415
pi2 = int(pi * 2)
angleList = []
distList = []
direction = 1
count = 0
kill = 20
i = 0

# Part 1

file = open("sound.wav", "rb")  # opens the file
data = str(file.read(e))  # data is the file in string form, with 'e' being the amount of bytes it reads
print("raw data:")
print(data)  # shows the raw data it reads

print(" ")

# Part 2

allowed = "0123456789abcdefx"  # only hexadecimal characters will remain with 'x'
data2 = ""  # this will be the new string with only the characters we want
for character in data:
    for alChar in allowed:
        if alChar == character:  # if character    is allowed
            data2 += character  # adds the allowed character to the new string
print("modified data:")
print(data2)  # shows the new string, with only hexadecimal characters and 'x'

print(" ")

# Part 3

data2 = data2.split("x")  # splits the data, with 'x' being the place where it splits
print("splitted data:")
print(data2)  # shows splitted data

print(" ")

# Part 4

n = int(
    len(data2))  # counts the amount of pieces in the splitted data, and gives the number of coordinates should be made
for c in range(0, n):  # loops untill the max amount of coordinates are made
    if count <= kill:
        turtle.speed(0)  # speeds up the process
        turtle.pensize(5)
        if next == 1:
            sample = int(''.join(data2[a:b]),
                         16)  # groups 2 strings, converts it to decimal numbers, then makes a string of it
            next = 0
            if sample > 0:
                sample = math.sqrt(math.sqrt(sample / d ** d))

        a += d  # makes sure the strings that will be grouped are the the following ones, not the same one
        b += d  # makes sure the strings that will be grouped are the the following ones, not the same one

        wave = pi2 / sample
        steps = wave / dt

        dist = round((3 + math.cos(sample * t))*100,2)
        angle = t
        t += dt * direction
        if t >= pi2 and direction == 1:
            direction = -1
            count += 1
        if t <= 0 and direction == -1:
            direction = 1
            count += 1
        step += 1
        if step >= steps:
            step = 0
            next = 1

        x = math.degrees(math.sin(angle)) * dist
        y = math.degrees(math.cos(angle)) * dist

        turtle.goto(x, y)

        angle = round(math.degrees(angle)*100,2)
        angleList.append(angle)
        distList.append(dist)

        f = open("file.txt", "w+")  # opens the text file with write permission. If it doesnt exist it will make a new one.
        f.write(str(len(angleList))+"Angles:"+str(angleList)+"\n"+str(len(distList))+"Distances:"+str(distList))
        f.close()  # closes the text file and saves it

        c += 1  # counts the amount of coordinates are made

print(angleList)
print(distList)
