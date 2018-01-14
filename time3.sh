#!/bin/bash

tmpfile1=$(mktemp /tmp/time.XXXXXX)
tmpfile2=$(mktemp /tmp/time.XXXXXX)

echo "op, time" > "$tmpfile1"

grep "time a" $1 |awk -F[ '{print $1","$2}' | sed 's/] s//g' | sed 's/Repl time avg:/Re/g'|sed 's/Eval time avg:/Ev/g'|sed 's/Muta time avg:/Mu/g'|sed 's/Cros time avg:/Cr/g'|sed 's/Sele time avg:/Se/g' >> "$tmpfile1"


python - << END
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.mlab

data1 = np.genfromtxt("$tmpfile1", delimiter=',',dtype=None, autostrip=True, names=True)

mean1 = matplotlib.mlab.rec_groupby(data1, ('op',), (('time',np.mean,'time'),))


ind = np.arange(5)

f, ax = plt.subplots()
plt.title("Tiempo medio por operador (s)")
ax.bar(ind, mean1['time'], 0.35, label='cpu' )


ax.set_xticks(ind)
ax.set_xticklabels((mean1['op'][0], mean1['op'][1],mean1['op'][2],mean1['op'][3] , mean1['op'][4]))

plt.legend()
plt.show()

print mean
END
rm "$tmpfile1"
