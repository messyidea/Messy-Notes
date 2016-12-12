#!/usr/bin/env python
# coding=utf-8

import requests
from bs4 import BeautifulSoup
import sys

base = sys.argv[1]
page = requests.get(base)
page = page.text

soup = BeautifulSoup(page, "html.parser")

a_list = soup.find_all('a')
link_list = []
for each in a_list:
    link_list.append(base + each.get("href"))

link_list = link_list[1:]
# print link_list
for each in link_list:
    print each