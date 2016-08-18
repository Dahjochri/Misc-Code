import multiprocessing, os

def lol(a):
	print "Process:", os.getpid()
	print "Value:", a


if __name__ == "__main__":
	p = multiprocessing.Pool(processes = 1)
	p.map_async(lol, range(200), 10).get(9999999)