#!/bin/bash

tmpfile=$(mktemp /tmp/time.XXXXXX)
grep Time $1  | awk '{print $2}' | sed  's/\[\|\]//g' > "$tmpfile"

python - << END
import numpy as np

data = np.loadtxt("$tmpfile")

print np.mean(data)

END
rm "$tmpfile"
