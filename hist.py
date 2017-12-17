#!/usr/bin/python
import sys
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.mlab

path_to_csv = sys.argv[1]
data = np.genfromtxt(path_to_csv, dtype=float, delimiter=',', names=True)


x = np.unique(data['generation'])

ax = plt.subplot(1,2,1)

result = matplotlib.mlab.rec_groupby(data, ('generation',), (('mean_fitness',np.mean,'avg_mean'),))
result2 = matplotlib.mlab.rec_groupby(data, ('generation',), (('stdev_fitness',np.mean,'avg_stddev'),))
line1, = ax.plot(x, result['avg_mean'])
line2, = ax.plot(x, result2['avg_stddev'])

ax = plt.subplot(1,2,2)

threads = np.unique(data['thread_id'] )
for i in threads:
    y_new = data['mean_fitness'][data['thread_id'] == i]
    line1, = ax.plot(x, y_new)
    y_new = data['stdev_fitness'][data['thread_id'] == i]
    line2, = ax.plot(x, y_new)

plt.show()
