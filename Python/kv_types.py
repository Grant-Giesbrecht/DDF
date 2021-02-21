DOUBLE = 'd'
STRING = 's'
BOOL = 'b'
ERR = 'e'


FLAT = 0
M1D = 1
M2D = 2

class KvItem():

	def __init__(self, value, name:str, desc:str=""):

		#Get depth, type
		td = getType(value)

		self.type = td[1]
		self.dimension = td[0]
		self.val = value
		self.name = name
		self.desc = desc

		#If error occured in interpreting type, add message to value
		if td[1] == ERR:
			self.val = td[0]
			self.dimension = -1

def getType(value):

	it_val = value

	T = type(value)
	depth = 0
	while (T == list or T == tuple):

		depth += 1
		try:
			it_val = it_val[0]
			T = type(it_val)
		except:
			return ("Empty list provided. Cannot get type." ,ERR)

		if depth > 10:
			return ("Matrix depth exceeded 10. Aborting.", ERR)

	if (T == int or T == float):
		return (depth, DOUBLE)
	elif (T == str):
		return (depth, STRING)
	elif (T == bool):
		return (depth, BOOL)
	else:
		return (f"Unrecognized type {T}.", ERR)
