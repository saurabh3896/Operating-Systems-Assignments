import re
import numpy as np
import matplotlib.pyplot as plt

with open("b-time.txt") as f:
    data = f.read()

data = data.split('\n')[:-1]

x = map(float, [row.split(' ')[0] for row in data])
y = map(float, [(row.split(' ')[1]) for row in data])

fig = plt.figure()
plt.grid()

ax1 = fig.add_subplot(111)

ax1.set_title("MutualExclusionUsingBounded&UnboundedLockingAlgorithms")
ax1.set_xlabel('Number of threads (n)')
ax1.set_ylabel('Average time taken')

for xy in zip(x, y):                                       # <--
    ax1.annotate('(%s, %s)' % xy, xy = xy, textcoords = 'data') # <--

ax1.plot(x,y,'bo-',c='r',label='BoundedLockAlgo',linewidth = 2)

with open("un-time.txt") as f:
    data = f.read()

data = data.split('\n')[:-1]

x = map(float, [row.split(' ')[0] for row in data])
y = map(float, [(row.split(' ')[1]) for row in data])

for xy in zip(x, y):                                       # <--
    ax1.annotate('(%s, %s)' % xy, xy = xy, textcoords = 'data') # <--

ax1.plot(x,y,'bo-',c='m',label='UnboundedLockAlgo',linewidth = 2)

leg = ax1.legend(loc='upperright')

plt.show()
