#!/usr/bin/python3
from sys import argv
import numpy as np
from matplotlib import pyplot as plt
import os

filename = argv[1]
snap_data = np.loadtxt(filename, delimiter=",")
vect_sum = [np.sum(snap_data[:, 2]), np.sum(snap_data[:, 2])]

plt.quiver(*snap_data.T)
plt.title(filename + f", vect_sum: {vect_sum}")
plt.xlabel("row")
plt.ylabel("col")
plot_filename = os.path.basename(filename) + '.png'
plt.savefig(plot_filename)

print(f"Ploted into \"{plot_filename}\"")
