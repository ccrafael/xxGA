#!/usr/bin/python
# -*- coding: utf-8 -*-
import sys
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.mlab

path_to_csv = sys.argv[1]
cpu = np.genfromtxt(path_to_csv, dtype=float, delimiter=',', names=True)

path_to_csv = sys.argv[2]
gpu = np.genfromtxt(path_to_csv, dtype=float, delimiter=',', names=True)

x = np.unique(cpu['generation'])

ax = plt.subplot(1,2,1)
plt.title("Solo cpu")
ax.set_ylabel('fitnes 1/(1 + f(x))')
ax.set_xlabel('generacion')

#plt.xscale('log')

result = matplotlib.mlab.rec_groupby(cpu, ('generation',), (('mean_fitness',np.mean,'avg_mean'),))
best = matplotlib.mlab.rec_groupby(cpu, ('generation',), (('fitness',np.max,'max_fitness'),))
best_by_exe = matplotlib.mlab.rec_groupby(cpu, ('execution', 'generation',), (('fitness',np.max,'max_fitness'),))
mbest = matplotlib.mlab.rec_groupby(best_by_exe, ('generation',), (('max_fitness',np.mean,'max_fitness_mean'),))

#result2 = matplotlib.mlab.rec_groupby(data, ('generation',), (('stdev_fitness',np.mean,'avg_stddev'),))
# line1, = ax.plot(x, result['avg_mean'], label='fitness avg')
line1, = ax.plot(x, result['avg_mean'], label='fitness mean')
line2, = ax.plot(x, best['max_fitness'], label='best fitness')
line3, = ax.plot(x, mbest['max_fitness_mean'], label='best fitness avg')

# line2, = ax.plot(x, result2['avg_stddev'], label='fitness stdev')

# enable legend
plt.legend()

ax = plt.subplot(1,2,2)

plt.title("Cpu + Gpu")
ax.set_ylabel('fitnes 1/(1 + f(x))')
ax.set_xlabel('generacion')

result = matplotlib.mlab.rec_groupby(gpu, ('generation',), (('mean_fitness',np.mean,'avg_mean'),))
best = matplotlib.mlab.rec_groupby(gpu, ('generation',), (('fitness',np.max,'max_fitness'),))
best_by_exe = matplotlib.mlab.rec_groupby(gpu, ('execution','generation',), (('fitness',np.max,'max_fitness'),))
mbest = matplotlib.mlab.rec_groupby(best_by_exe, ('generation',), (('max_fitness',np.mean,'max_fitness_mean'),))

#result2 = matplotlib.mlab.rec_groupby(data, ('generation',), (('stdev_fitness',np.mean,'avg_stddev'),))
# line1, = ax.plot(x, result['avg_mean'], label='fitness avg')
line1, = ax.plot(x, result['avg_mean'], label='fitness mean')
line2, = ax.plot(x, best['max_fitness'], label='best fitness')
line3, = ax.plot(x, mbest['max_fitness_mean'], label='best fitness avg')
#plt.xscale('log')

# enable legend
plt.legend()

# show me the chart
plt.show()
