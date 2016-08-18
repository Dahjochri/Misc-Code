from PIL import Image
im = Image.open("PNG.png")
key = {"-----":"0", ".----":"1", "..---":"2", "...--":"3", "....-":"4", ".....":"5", "-....":"6", "--...":"7", "---..":"8", "----.":"9", ".-":"A", "-...":"B", "-.-.":"C", "-..":"D", ".":"E", "..-.":"F", "--.":"G", "....":"H", "..":"I", ".---":"J", "-.-":"K", ".-..":"L", "--":"M", "-.":"N", "---":"O", ".--.":"P", "--.-":"Q", ".-.":"R", "...":"S", "-":"T", "..-":"U", "...-":"V", ".--":"W", "-..-":"X", "-.--":"Y", "--..":"Z"}

sent = []
lold = 0
lnew = 0
for y in range(0,29):
	for x in range(0,99):
		if im.getpixel((x,y)):
			sent.append(chr(lnew-lold))
			lold=lnew
		lnew += 1
	lnew+=1

sent = "".join(sent).split(" ")
print sent[0:-1]
for i in range(0,len(sent[0:-1])):
	sent[i] = key[sent[i]]

print "".join(sent)

