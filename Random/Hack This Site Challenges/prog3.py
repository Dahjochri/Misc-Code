
import md5
h2d = {"0":0,"1":1,"2":2,"3":3,"4":4,"5":5,"6":6,"7":7,"8":8,"9":9,"A":10,"B":11,"C":12,"D":13,"E":14,"F":15}

def evalCrossTotal(strMD5):
	global h2d
	intTotal = 0
	print list(strMD5)
	for char in strMD5:
		intTotal += h2d[char]
	return intTotal

def Encrypt(strString, strPassword):
	global h2d
	strPasswordMD5 = md5.new(strPassword).hexdigest().upper()
	intMD5Total = evalCrossTotal(strPasswordMD5)
	arrEncryptedValues = []
	intStrlen = len(strString)
	for i in range(0,intStrlen):
		arrEncryptedValues.append(ord(strString[i]) + (h2d[strPasswordMD5[i%32]]) - intMD5Total)
		intMD5Total = evalCrossTotal(md5.new(strString[0:i+1]).hexdigest().upper()[0:16] + md5.new(str(intMD5Total)).hexdigest().upper()[0:16])
	
	return str(arrEncryptedValues).replace("[","").replace("]","").replace(",","")

def DecryptBrutestring(strEncrypted):
	strString = "000-000-OEM-000-1.1\n000-000-OEM-000-1.1\n000-000-OEM-000-1.1\n000-000-OEM-000-1.1\n000-000-OEM-000-1.1\n"

def DecryptBrutepass(arrEncrypted):
	pass

def DoEncrypt():
	code_file = open("thecode.txt", "r")
	strString = code_file.read()
	strPassword = raw_input("password:")
	print Encrypt(strString, strPassword)

def DoDecrypt():
	EncryptedValues = raw_input("What to decrypt?:")
	mode = raw_input("brute string or pass?:")
	if mode == "string":
		print DecryptBrutestring(EncryptedValues)
	elif mode == "pass":
		print DecryptBrutepass(EncryptedValues.split(" "))
		
def Main():
	mode = raw_input("encrypt or decrypt?:")
	if mode == "encrypt":
		DoEncrypt()
	elif mode == "decrypt":
		DoDecrypt()
	

if __name__=="__main__":
	try:
		import psyco
		psyco.full()
		print "using psyco"
	except ImportError:
		pass
	Main()