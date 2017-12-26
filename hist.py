#!/usr/bin/python
import sys
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.mlab

path_to_csv = sys.argv[1]
data = np.genfromtxt(path_to_csv, dtype=float, delimiter=',', names=True)

x = np.unique(data['generation'])

ax = plt.subplot(1,2,1)
plt.title("Population fitness evolution")
ax.set_ylabel('fitnes 1/(1 + f(x))')
ax.set_xlabel('generation')

#plt.xscale('log')

result = matplotlib.mlab.rec_groupby(data, ('generation',), (('mean_fitness',np.mean,'avg_mean'),))
best = matplotlib.mlab.rec_groupby(data, ('generation',), (('fitness',np.max,'max_fitness'),))

#result2 = matplotlib.mlab.rec_groupby(data, ('generation',), (('stdev_fitness',np.mean,'avg_stddev'),))
# line1, = ax.plot(x, result['avg_mean'], label='fitness avg')
line1, = ax.plot(x, result['avg_mean'], label='fitness mean')
line2, = ax.plot(x, best['max_fitness'], label='best fitness')

# line2, = ax.plot(x, result2['avg_stddev'], label='fitness stdev')

# enable legend
plt.legend()

ax = plt.subplot(1,2,2)

plt.title("Population fitness evolution by island")
ax.set_ylabel('fitnes 1/(1 + f(x))')
ax.set_xlabel('generation')

#plt.xscale('log')

threads = np.unique(data['thread_id'] )

count = 0
for i in threads:
    y_new = data['mean_fitness'][data['thread_id'] == i]
    line1, = ax.plot(x, y_new, label= 'mean fitness '+str(count))
    
    y_new = data['fitness'][data['thread_id'] == i]
    line2, = ax.plot(x, y_new, label= 'best fitness '+str(count))
    
#    y_new = data['stdev_fitness'][data['thread_id'] == i]
#    line2, = ax.plot(x, y_new, label= 'fitness '+str(count) + ' stdev')
    count = count + 1

# enable legend
plt.legend()

# show me the chart
plt.show()
