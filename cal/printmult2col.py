import struct
import numpy as np
import matplotlib.pyplot as plt

file1 = open("mult.txt")
txt= file1.read()
x = txt.split(" ")
#print(x)
lenx = int(len(x)/2)

xx = [];
yy = [];
for index in range(lenx):
	print(x[index*2]+"\t"+x[index*2+1])
	xx.append(x[index*2])
	yy.append(x[index*2+1])
#plt.plot(xx,yy,"s")
#plt.show()

