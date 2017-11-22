import os
import datetime
import numpy as np
import pandas as pd
import pymysql
import copy
import pickle
import jieba
from gensim import corpora, models, similarities
from collections import defaultdict
import random
import math

class tlda:
    def __init__(self, posts):
        self.posts = posts
        self.init()

    def init(self):
        self.data_init()
        self.param_init()

    def data_init(self):
        # init wordmap and data
        self.wordmap = {}
        self.rwordmap = {}
        t = 0
        for post in self.posts:
            for reply in post:
                seg_list = reply.split()
                for word in seg_list:
                    if word not in self.wordmap:
                        self.wordmap[word] = t
                        t = t + 1

        for k in self.wordmap:
            self.rwordmap[self.wordmap[k]] = k

        self.data = []
        dpost = []
        dreply = []
        for post in self.posts:
            dpost.clear()
            for reply in post:
                dreply.clear()
                seg_list = reply.split()
                for each in seg_list:
                    dreply.append(self.wordmap[each])
                dpost.append(copy.copy(dreply))
            self.data.append(copy.copy(dpost))

    def param_init(self):
        self.A = 5
        self.U = len(self.data)
        self.V = len(self.wordmap)
        # print(str(self.A) + "  " + str(self.U) + "  " + str(self.V))
        self.nIter = 100

        self.alpha_general = np.array([0.5 for x in range(self.A)])
        self.alpha_general_sum = 0.5 * self.A

        self.gamma = np.array([20 for x in range(2)])

        self.beta_background = np.array([0.01 for x in range(self.V)])
        self.beta_word = np.array([0.01 for x in range(self.V)])
        self.beta_background_sum = self.V * 0.01
        self.beta_word_sum = self.V * 0.01

        self.C_ua = np.zeros((self.U, self.A), dtype="int")
        self.theta_general = np.zeros((self.U, self.A), dtype="float")

        self.C_lv = np.zeros(2, dtype="int")
        self.rho = np.zeros(2, dtype="float")

        self.C_word = np.zeros((self.A, self.V), dtype="int")
        self.phi_word = np.zeros((self.A, self.V), dtype="float")

        self.C_b = np.zeros(self.V, dtype="int")
        self.phi_background = np.zeros(self.V, dtype="float")

        self.countAllWord = np.zeros(self.A, dtype="int")

    def intialize(self):
        print("intialize start: ")
        self.z = np.array([[0 for d in range(len(self.data[u]))] for u in range(len(self.data))])
        self.x = np.array([[[0 for w in range(len(self.data[u][d]))] for d in range(len(self.data[u]))] for u in range(len(self.data))])
        u = 0
        d = 0
        w = 0

        for u in range(len(self.data)):
            buffer_user = self.data[u]
            for d in range(len(buffer_user)):
                tw = buffer_user[d]
                randgeneral = random.random()
                thred = 0.0
                a_general = 0
                for t in range(self.A):
                    thred = thred + 1 / self.A
                    if thred >= randgeneral:
                        a_general = t
                        break
                self.z[u][d] = a_general
                self.C_ua[u][a_general] += 1
                for w in range(len(tw)):
                    word = tw[w]
                    randback = random.random()
                    buffer_x = 1
                    if randback > 0.5:
                        buffer_x = 1
                    else:
                        buffer_x = 0

                    if buffer_x == 1:
                        self.C_lv[1] += 1
                        self.C_word[a_general][word] += 1
                        self.countAllWord[a_general] += 1
                        self.x[u][d][w] = buffer_x
                    else:
                        self.C_lv[0] += 1
                        self.C_b[word] += 1
                        self.x[u][d][w] = buffer_x

        print("Intialize Done")

    def estimate(self):
        niter = 0

        while True:
            niter += 1
            # print("iteration  " + str(niter))
            print("=", end="")

            self.sweep()

            if niter > self.nIter:
                print(" ")
                print("update_distribution")
                self.update_distribution()
                break

    def sweep(self):
        for u in range(len(self.data)):
            buffer_user = self.data[u]
            for d in range(len(buffer_user)):
                tw = buffer_user[d]
                self.sample_z(u, d, buffer_user, tw)

                for cntword in range(len(tw)):
                    word = tw[cntword]
                    self.sample_x(u, d, cntword, word)


    def sample_x(self, u, d, n, word):
        binarylabel = self.x[u][d][n]
        binary = 0
        if binarylabel == 1:
            binary = 1
        else:
            binary = 0

        self.C_lv[binary] -= 1
        if binary == 0:
            self.C_b[word] -= 1
        else:
            self.C_word[self.z[u][d]][word] -= 1
            self.countAllWord[self.z[u][d]] -= 1

        binarylabel = self.draw_x(u, d, n, word)
        self.x[u][d][n] = binarylabel

        if binarylabel == 1:
            binary = 1
        else:
            binary = 0

        self.C_lv[binary] += 1
        if binary == 0:
            self.C_b[word] += 1
        else:
            self.C_word[self.z[u][d]][word] += 1
            self.countAllWord[self.z[u][d]] += 1


    def draw_x(self, u, d, n, word):
        returnvalue = 0
        P_lv = np.zeros(2, dtype="float")
        Pb = 1.0
        Ptopic = 1.0

        P_lv[0] = (self.C_lv[0] + self.gamma[0]) \
            / (self.C_lv[0] + self.C_lv[1] + self.gamma[0] + self.gamma[1])

        P_lv[1] = (self.C_lv[1] + self.gamma[1]) \
            / (self.C_lv[0] + self.C_lv[1] + self.gamma[0] + self.gamma[1])

        Pb = (self.C_b[word] + self.beta_background[word]) \
            / (self.C_lv[0] + self.beta_background_sum)

        Ptopic = (self.C_word[self.z[u][d]][word] + self.beta_word[word]) \
            / (self.countAllWord[self.z[u][d]] + self.beta_word_sum)

        p0 = Pb * P_lv[0]
        p1 = Ptopic * P_lv[1]

        sum = p0 + p1
        randPick = random.random()
        if randPick <= p0 / sum:
            returnvalue = 0
        else:
            returnvalue = 1

        return returnvalue




    def sample_z(self, u, d, buffer_user, tw):
        tweet_topic = self.z[u][d]
        # print("tweet_topic == " + str(tweet_topic))
        w = 0
        self.C_ua[u][tweet_topic] -= 1
        for w in range(len(tw)):
            word = tw[w]
            if self.x[u][d][w] == 1:
                self.C_word[tweet_topic][word] -= 1
                self.countAllWord[tweet_topic] -= 1

        buffer_z = self.draw_z(u, d, buffer_user, tw)
        tweet_topic = buffer_z

        self.z[u][d] = tweet_topic

        self.C_ua[u][tweet_topic] += 1
        for w in range(len(tw)):
            word = tw[w]
            if self.x[u][d][w] == 1:
                # print(str(tweet_topic) + "  " + str(word))
                self.C_word[tweet_topic][word] += 1
                self.countAllWord[tweet_topic] += 1


    def draw_z(self, u, d, buffer_user, tw):
        P_topic = np.zeros(self.A, dtype="double")
        pCount = np.zeros(self.A, dtype="int")
        wordcnt = {}
        totalWords = 0

        for w in range(len(tw)):
            if self.x[u][d][w] == 1:
                totalWords += 1
                word = tw[w]
                if word not in wordcnt:
                    wordcnt[word] = 1
                else:
                    wordcnt[word] = wordcnt[word] + 1

        for a in range(self.A):
            P_topic[a] = (self.C_ua[u][a] + self.alpha_general[a]) \
                / (len(buffer_user) - 1 + self.alpha_general_sum)
            buffer_P = 1.0
            i = 0
            for k in wordcnt:
                for j in range(wordcnt[k]):
                    value = float(self.C_word[a][k] + self.beta_word[k] + j) \
                        / (self.countAllWord[a] + self.beta_word_sum + i)
                    i += 1
                    buffer_P *= value
                    buffer_P = self.isOverFlow(buffer_P, pCount, a)
                    # print(value)
                    # print(buffer_P)
                    # print(".................................")

            P_topic[a] *= buffer_P

        self.reComputeProbs(P_topic, pCount)

        randz = random.random()
        sum = 0.0
        for t in range(self.A):
            sum += P_topic[t]

        thred = 0
        chosena = -1
        for a in range(self.A):
            thred += P_topic[a] / sum
            if thred >= randz:
                chosena = a
                break

        if chosena == -1:
            print("chosena == -1, error !")
            print("????????")
            for aa in range(self.A):
                print(P_topic[aa])
            print("????????")


        wordcnt.clear()
        return chosena

    def reComputeProbs(self, P_topic, pCount):
        mm = pCount[0]
        for i in range(self.A):
            if pCount[i] > mm:
                mm = pCount[i]

        # print("max == " + str(mm))
        
        # print("oooooooooooooooooooooooooo")
        # for i in range(self.A):
        #     P_topic[i] = P_topic[i] * math.pow(1e150, pCount[i] - mm)
        #     print(P_topic[i])

        # print("oooooooooooooooooooooooooo")



    def isOverFlow(self, buffer_P, pCount, a2):
        if buffer_P > 1e150:
            pcount[a2] += 1
            return buffer_P / 1e150
        if buffer_P < 1e-150:
            pCount[a2] -= 1
            return buffer_P * 1e150

        return buffer_P



    def update_distribution(self):
        for u in range(self.U):
            c_u_a = 0
            for a in range(self.A):
                c_u_a += self.C_ua[u][a]

            for a in range(self.A):
                self.theta_general[u][a] = (self.C_ua[u][a] + self.alpha_general[a]) \
                    / (c_u_a + self.alpha_general_sum)

        # print("wwwwwwwwwwwwwwwww")
        for a in range(self.A):
            c_v = 0
            for v in range(self.V):
                c_v += self.C_word[a][v]
                # print(str(v) + "     " + str(self.C_word[a][v]))

            # print("cv =====  " + str(c_v))
            for v in range(self.V):
                self.phi_word[a][v] = (self.C_word[a][v] + self.beta_word[v]) \
                    / (c_v + self.beta_word_sum)
                # print(str(v) + "     " + str(self.phi_word[a][v]))
        # print("wwwwwwwwwwwwwwwww")

        c_b_v = 0
        for v in range(self.V):
            c_b_v += self.C_b[v]
        for v in range(self.V):
            self.phi_background[v] = (self.C_b[v] + self.beta_background[v]) \
                / (c_b_v + self.beta_background_sum)

        self.rho[0] = (self.C_lv[0] + self.gamma[0]) \
            / (self.C_lv[0] + self.C_lv[1] + self.gamma[0] + self.gamma[1]) 

        self.rho[1] = (self.C_lv[1] + self.gamma[1]) \
            / (self.C_lv[0] + self.C_lv[1] + self.gamma[0] + self.gamma[1]) 


    def get_result(self):
        # print("zzzzzzzzzzzzzzzzzzzzz")
        # for i in range(len(self.data)):
        #     for j in range(len(self.data[i])):
        #         print(self.z[i][j])
        # print("zzzzzzzzzzzzzzzzzzzzz")

        # print("xxxxxxxxxxxxxxxxxxxxx")
        # for i in range(len(self.data)):
        #     for j in range(len(self.data[i])):
        #         print(self.x[i][j])
        #         # for k in range(len(self.data[i][j][k])):
        #             # print(self.x[i][j][k])
        # print("xxxxxxxxxxxxxxxxxxxxx")


        
        print("start result")
        for a in range(self.A):
            print("topic " + str(a))
            
            rst = self.gettop(self.phi_word[a], 20)
            for k in range(len(copy.copy(rst))):
                print(self.rwordmap[rst[k]], end=' ')
            print("")

        print("end result")



    def gettop(self, array, Cnt):
        # print("xxxxxxxxxxxxxxxxxxxxx")
        # print(array)
        # print("xxxxxxxxxxxxxxxxxxxxx")
        index = -1
        rankList = []
        for i in range(Cnt):
            mm = -float("inf")
            index = -1
            for j in range(len(array)):
                if array[j] > mm and j not in rankList:
                    mm = array[j]
                    index = j
            rankList.append(index)

        # print("rrrrrrrr")
        # print(rankList)
        # print("rrrrrrrr")
        return rankList

















