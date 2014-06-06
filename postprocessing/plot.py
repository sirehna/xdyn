import csv
import sys
import matplotlib
matplotlib.use('SVG')
import matplotlib.pyplot as plt
import pandas as pd
import matplotlib.pyplot as plt

# Usage: executables/sim -y falling_ball_test.yml -s rkck --dt 0.1 --tstart 0 --tend 10 | python test.py 'falling ball' 1 3


output = sys.argv[1]
def plot(x,y):
    plt.xlabel(x.name)
    plt.ylabel(y.name)
    plt.plot(x,y,label=y.name)
    plt.show()
    plt.savefig(output)

R = pd.read_csv(sys.stdin)

plt.figure("")
plt.grid()
plt.title(output)
k = 0
for idx in sys.argv[2:]:
   if (k % 2 == 0):
      x =  R.ix[:,int(idx)]
   else:
      y =  R.ix[:,int(idx)]
      plot(x,y)
   k = k +1
plt.legend(loc='upper right')
