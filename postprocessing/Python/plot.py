import sys
import pandas as pd
import matplotlib
matplotlib.use('SVG')
import matplotlib.pyplot as plt

# Usage: ../bin/xdyn -y falling_ball_test.yml -s rkck --dt 0.1 --tstart 0 --tend 10 | python plot.py 'falling ball' 1 3

output = sys.argv[1]

R = pd.read_csv(sys.stdin)

fig = plt.figure("")
plt.grid()
plt.title(output)
k = 0
legend = []
for idx in sys.argv[2:]:
    if (k % 2 == 0):
        x = R.iloc[:, int(idx)]
        plt.xlabel(x.name)
    else:
        y = R.iloc[:, int(idx)]
        plt.plot(x, y, label = y.name)
        legend.append(y.name)
    k += 1

plt.legend(legend, loc = 'upper right')
# plt.show()
plt.savefig(output)
