from Tkinter import *
import random
from PIL import Image, ImageTk

colors = []
colorone = []
colortwo = []
color_set = 0
seed=0

def DrawImage(doRandom):
	global le_image
	global origpix
	global panel1
	global root
	global color_set
	global colors
	global colorone
	global seed
	global colortwo
	
	pix = le_image.load()
	
	if color_set == 0:
		for y in range(0,100):
			for x in range(0,200):
				if y>49:
					pix[x,y] = colors[0]
				else:
					pix[x,y] = colors[1]
	else:
		if doRandom==1:
			c1=colorone[:]
			c2=colortwo[:]
			for y in range(0,len(c1)):
				therand = random.randint(0,len(c1)-1)
				for x in range(0,200):
					pix[x,y]=origpix[x, c1[therand]]
					if pix[x,y]!=colors[0]: pix[x,y]=0
				del c1[therand]
			for y in range(49,49+len(c2)):
				therand = random.randint(0,len(c2)-1)
				for x in range(0,200):
					pix[x,y]=origpix[x, c2[therand]]
					if pix[x,y]!=colors[1]: pix[x,y]=0
				del c2[therand]
		elif doRandom==2:
			rows1=[]
			rows2=[]
			for y in range(0,len(colorone)):
				rows1.append([])
				for x in range(0,200):
					if pix[x,y]==colors[0]: rows1[y].append(1)
					else: rows1[y].append(0)
			for y in range(49,49+len(colortwo)):
				rows2.append([])
				for x in range(0,200):
					if pix[x,y]==colors[0]: rows2[y-49].append(1)
					else: rows2[y-49].append(0)
			
			
			for x in range(0,200):
				
				for y in range(0,len(colorone)):
					pass
				for y in range(49,49+len(colortwo)):
					pass
		else:
			colorone=[]
			colortwo=[]
			for y in range(0,100):
				for x in range(0,200):
					if origpix[x,y]==colors[0] or origpix[x,y]==colors[1]:
						if origpix[x,y]==colors[0]:
							colorone.append(y)
						if origpix[x,y]==colors[1]:
							colortwo.append(y)
						break
			for y in range(0,len(colorone)):
				for x in range(0,200):
					pix[x,y]=origpix[x, colorone[y]]
					if pix[x,y]!=colors[0]: pix[x,y]=0
			for y in range(49,49+len(colortwo)):
				for x in range(0,200):
					pix[x,y]=origpix[x, colortwo[y-49]]
					if pix[x,y]!=colors[1]: pix[x,y]=0
	
	
	newimtk = ImageTk.PhotoImage(le_image.resize((400,200)))
	panel1.config(image=newimtk)
	panel1.pack(side='top', fill='both', expand='yes')
	panel1.image = newimtk
	root.update()

def keypress(event):
	global root
	global seed
	global color_set
	if event.keysym == 'Escape':
		root.destroy()
	
	x = event.char
	if x == "2":
		DrawImage(0)
	elif x == "1":
		color_set = abs(color_set-1)
		print color_set
	elif x == "3":
		DrawImage(1)
	elif x == "4":
		seed+=1
	elif x == "5":
		DrawImage(2)

def Click(event):
	global origpix
	colors[0]=origpix[event.x/4, event.y/4]

def Click2(event):
	global origpix
	colors[1]=origpix[event.x/4, event.y/4]

def Main():
	DrawImage()


def Init():
	global root
	global le_image
	global orig
	global panel1
	global origpix
	global colors
	
	root = Tk()
	root.title("HTS Prog7")
	root.geometry("800x600")
	orig = Image.open("BMP.png")
	le_image = orig.copy()
	origpix = orig.load()
	colors.append(origpix[0,0])
	colors.append(origpix[1,0])
	clickFrame = Frame(root, height=400, width=800)
	clickFrame.bind_all('<1>', Click);
	clickFrame.bind_all('<3>', Click2);
	clickFrame.pack(side='bottom')
	image1 = ImageTk.PhotoImage(le_image)
	image2 = ImageTk.PhotoImage(orig.resize((800,400)))
	panel2 = Label(clickFrame, image=image2)
	panel2.pack(side='bottom', fill='both', expand='yes')
	panel2.image = image2
	panel1 = Label(root, image=image1)
	panel1.pack(side='top', fill='both', expand='yes')
	panel1.image = image1
	root.bind_all('<Key>', keypress)
	root.mainloop()




if __name__=="__main__":
	try:
		import psyco
		psyco.full()
		print "using psyco"
	except ImportError:
		pass
	Init()