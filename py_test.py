from Python.pykv import *

kv = KVFile();
kv.add(3.14159, "pi")
kv.add(12.05, "Vin", "Input Voltage (V)")
kv.add([12.05, 12.03, 11.97], "Vin_t", "Vin logged (V)")
if not kv.add(None, "This_fails"):
	print(kv.err())
if not kv.add([[[1]]], "This_also_fails"):
	print(kv.err())
if not kv.add([1], "pi"):
	print(kv.err())
print(kv("pi"))
print(kv("Vin"))
print(kv("Vin_t"))
kv.clear()
if not kv.readKV1_V2("./examples/read_test.kv"):
	print(kv.err())
print("\n")
print(f"Version: {kv.getVersion()}")
print(f"\"{kv.getHeader()}\"")
kv.show()
