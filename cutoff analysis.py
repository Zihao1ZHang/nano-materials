import numpy
import numpy as np
import scipy.misc
from matplotlib import lines
from scipy.optimize import fmin, minimize
import matplotlib.pyplot as plt
from gekko import GEKKO
import math
from scipy.misc import derivative
# measurements
from scipy.signal import argrelextrema

xm = np.loadtxt("source/test2.txt", delimiter=" ", usecols=(0))
ym = np.loadtxt("source/test2.txt", delimiter=" ", usecols=(1))

# GEKKO modelb
m = GEKKO()

# parameters
X = m.Param(value=xm)

A = m.FV(ub = 9, lb = 6.5)
r1 = m.FV(ub = 2.6, lb = 2.2)
a1 = m.FV(ub = 30, lb = 0.25)
B = m.FV(ub = 3, lb = 1)
r2 = m.FV(ub = 4, lb = 3.6)
b1 = m.FV(ub = 20, lb = 5)

r1.STATUS = 1
a1.STATUS = 1
A.STATUS = 1
B.STATUS = 1
r2.STATUS = 1
b1.STATUS = 1


# variables
y = m.CV(value=ym)
y.FSTATUS = 1

# regression equation
m.Equation(y == A*m.exp(-a1*(r1-X)**2)+B*m.exp(-b1*(r2-X)**2))

# regression mode
m.options.IMODE = 2 # 2 for estimate

# optimize
m.solve(disp=False)

# print parameters
print('Optimized, A = ' + str(A.value[0]))
print('Optimized, r1 = ' + str(r1.value[0]))
print('Optimized, a1 = ' + str(a1.value[0]))
print('Optimized, B = ' + str(B.value[0]))
print('Optimized, r2 = ' + str(r2.value[0]))
print('Optimized, b1 = ' + str(b1.value[0]))

A = A.value[0]
r1 = r1.value[0]
a1 = a1.value[0]
B = B.value[0]
r2 = r2.value[0]
b1 = b1.value[0]

def f(x): return A*math.exp(-a1*(r1-x)**2)+B*math.exp(-b1*(r2-x)**2)


#minimum = scipy.optimize.fmin(f , np.array[0,0])
mini = minimize(f, 2.88)
print(mini.x);

min = derivative(f, 2.77431257, dx=1e-10)
print(min)

'''
index = 0;
for i in range(0, int(4e5)):
    if (derivative(f, i/4e5, n = 2, dx=1e-10) >= 0):
        if(derivative(f, i/4e5, dx=1e-10) < min):
            min = derivative(f, i/4e5, dx=1e-10);
            index = i;
print(min)
'''

plt.bar(xm,ym,align='center', width = 0.05)
plt.xlabel('radius')
plt.ylabel('Frequency')
t = np.arange(0., 6., 0.01)
plt.plot(t,A*np.exp(-a1*(r1-t)**2)+B*np.exp(-b1*(r2-t)**2) )
plt.show()
