# -*- coding: utf-8 -*-
import numpy as np
import matplotlib.pyplot as plt

def drawPic():
    plt.figure()
    plt.rcParams['axes.unicode_minus']=False

    x = [x for x in range(50)]
    y = []
    y.append(np.random.randn(50).cumsum())
    y.append(np.random.randn(50).cumsum())
    y.append(np.random.randn(50).cumsum())

    colors = ['r', 'y', 'g', 'b', 'c', 'm',]
    #legends = {'circle':'o', 'star':'*', 'triangle_down':'v', 'dash-dot':'-.', 'dashed_line':'--', 'x_mark':'x'}
    legends = {'A':'-o', 'B':'-x', 'C':'-v'}

    for index,t in enumerate(legends.values()):
        sample_y = y[index]
        plt.plot(x, sample_y, t, color='black')

    plt.xlabel("X", fontproperties="simhei")
    plt.ylabel("Y", fontproperties="simhei")
    plt.title("Title", fontproperties="simhei")

    ax = plt.gca()
    ax.spines['right'].set_visible(False)
    ax.spines['top'].set_visible(False)
    plt.legend(legends.keys(), loc=1)
    plt.show()
    # plt.savefig("line.png", dpi=200)

drawPic()
