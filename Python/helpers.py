from itertools import groupby
from .kv_types import *
def parseTwoIdx(s, delims:str):
	p = 0
	for k, g in groupby(s, lambda x:x in delims):
		q = p + sum(1 for i in g)
		if not k:
			yield (p, q) # or p, q-1 if you are really sure you want that
		p = q

def parseIdx(input:str, delims:str=" "):

	out = []

	sections = list(parseTwoIdx(input, delims))
	for s in sections:
		out.append(StringIdx(input[s[0]:s[1]], s[0]))
	return out
