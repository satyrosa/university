import matplotlib
from math import cos
from math import pi
from matplotlib import patches
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches

maxd = 6
maxl = 16
maxm = cos(pi/6)*maxl
plt.xlim(0, maxl)
plt.ylim(0, maxl)
plt.grid()
axes = plt.gca()
axes.set_aspect("equal")

def serp_tria(l, m, d):
    global maxd
    global maxm
    global maxl
    if d == 1:
        return
    h = maxm/pow(2, maxd - d + 1)
    len = maxl/pow(2, maxd-d+1)
    r = 2 * h/3
    Ox = l
    Oy = m - h/3
    tria = patches.RegularPolygon((Ox, Oy), 3, r, pi)
    tria.set_fill(False)
    axes.add_patch(tria)
    serp_tria(l-len/2, m-h/2, d-1)
    serp_tria(l+len/2, m-h/2, d-1)
    serp_tria(l, m+h/2, d-1)


tria0 = patches.Polygon([(0,0), (maxl, 0), (maxl/2, maxm)])
tria0.set_fill(False)
axes.add_patch(tria0)
serp_tria(maxl/2, maxm/2, maxd)
plt.show()