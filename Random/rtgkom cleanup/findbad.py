#!/usr/bin/env python
import os,sys,time,datetime,stat,pwd,ldap


# Too speed up, files or dirs can be 1 (too large), 2 (symlinks), 3 (empty) or 4 (too deep)  (Didn't bother using an enum pt.)
# The size in Megabytes will be rounded in this script (I'm keeping it an "int")

class report:
	def __init__(self):
		self.badguys = [0]
		self.empties   = ['']
		self.toolarge  = ['']
		self.totalsize = 0
		self.symlinks  = ['']
		self.toodeep = ['']
		self.output = "\n- Find bad stuff on rtgkom.dk by Daniel 3.3f -\n\n"
	
	def report(self,where, why, info, owner):
		if owner!=None:
			self.badguys.append(owner);
		if why==1:
			self.totalsize += info
			self.toolarge.append("%s - %dMB large (uid: %d)" % (where,info>>20,owner))
		elif why==2:
			self.symlinks.append("%s - Points to %s" % (where,info))
		elif why==3:
			self.empties.append("%s (uid: %d)" % (where,owner))
		elif why==4:
			self.toodeep.append("%s (uid: %d)" % (where,owner))
	
	def WriteToFile(self):
		try:
			OutputFile.write(self.output)
		except IOError,error:
			print error
			exit()
	
	def Print(self):
		print self.output
		
	def PrepareOutput(self,users):
		global folders_traversed
		self.badguys = list(set(self.badguys[0:]))
		self.badguys.remove(0)
		
		for i in range(len(self.badguys)):
			self.badguys[i] = "%s (%d)" % (pwd.getpwuid(self.badguys[i]).pw_name, self.badguys[i])
		self.badguys.sort()
		
		self.output += "Too large files     : %d\n" % (len(self.toolarge)-1)
		self.output += "\tGiving a total of %dMB\n" % (self.totalsize>>20)
		self.output += "Empty directories   : %d\n" % (len(self.empties)-1)
		self.output += "Broken symlinks     : %d\n" % (len(self.symlinks)-1)
		self.output += "Too deep directories: %d\n" % (len(self.toodeep)-1)
		
		if len(self.toolarge)==1:
			self.output += "\n\nWe didn't find any files larger than %f MB\n" % (MaxSize>>20)
		else:
			self.output += "\n\nFiles that are too large(%d):\n" % (len(self.toolarge)-1)
			self.output += "\n".join(self.toolarge)
		
		if len(self.empties)==1:
			self.output+="\n------------------------------------------------\n\n\nWe didn't find any empty directories\n"
		else:
			self.output+="\n------------------------------------------------\n\n\nEmpty directories(%d):\n" % (len(self.empties)-1)
			self.output += "\n".join(self.empties)
			
		if len(self.symlinks)==1:
			self.output+="\n------------------------------------------------\n\n\nWe didn't find any broken symlinks\n"
		else:
			self.output+="\n------------------------------------------------\n\n\nBroken symlinks(%d):\n" % (len(self.symlinks)-1)
			self.output += "\n".join(self.symlinks)
			
		if len(self.toodeep)==1:
			self.output+="\n------------------------------------------------\n\n\nWe didn't find any directories that was too deep\n"
		else:
			self.output+="\n------------------------------------------------\n\n\nToo deep directories(%d):\n" % (len(self.toodeep)-1)
			self.output += "\n".join(self.toodeep)
		
		if len(self.badguys)==0:
			self.output+="\n------------------------------------------------\n\n\nApparently there are no bad guys on our server. Wooohoo!\n"
		else:
			self.output+="\n------------------------------------------------\n\n\nAll the bad guys are:\n\n"
			self.output+= "\n".join(self.badguys)
		
		endtime = time.time()
		
		self.output += "\n\n\nTotal folders traversed: %d\n" % (folders_traversed)
		self.output += "Execution took %f seconds.\n" % (endtime-starttime)


def wfunc(users, directory, filelist):
	global folders_traversed
	folders_traversed+=1
	
	try:
		folder_status = os.stat(directory)
	except OSError,err:
		print err
		exit()
	
	folder_owner=None
	
	if not classname is None:
		folder_owner = folder_status.st_uid
		if not folder_owner in users:
			return
		
	if str(directory).count("/")>MaxDepth:
		if folder_owner is None:
			folder_owner = folder_status.st_uid
		rep.report(directory, 4, None,folder_owner)
	
	if len(filelist)==0:
		if folder_owner is None:
			folder_owner = folder_status.st_uid
		rep.report(directory, 3,None,folder_owner)
		return
	
	for fil in filelist:
		try:
			file_status = os.stat(os.path.join(directory,fil))
			file_owner = file_status.st_uid	
			if file_status.st_size>MaxSize:
				rep.report(os.path.join(directory,fil),1, file_status.st_size,file_owner)
			
		except OSError,err:
			if str(err).find("[Errno 2]")==0:
				rep.report(os.path.join(directory,fil), 2,None,None)
			else:
				print err
				quit()

def PrintHelp():
	print "\nUsage:python findbad.py [Options]\n"
	print "Options:\n   [size = MaxFileSize]\n   [depth = MaxFolderDepth]\n   [file = filename]\n   [noterm]\n   [class = classname]\n"
	print "The first 2 arguments defines when something is reported. In Megabytes and amounts of folders\n"
	print "Write to file:\n   file = [filename]\n"
	print "Don't output in terminal:\n   noterm\n"
	print "Only display a specific class:\n   class = [classname] (ie. 3.3, 2.1, etc.)\n"
	print "The script can be used without arguments, and will then default to:"
	print "\tMaxFileSize: 100MB"
	print "\tMaxFolderDepth: 10"
	print "\tOutput to terminal only"
	print "\tReport all users"
	exit()

def GetUsers(c_high, c_low):
	now = datetime.datetime.now()
	calced_class = "%d_%d" % (now.year - c_high, c_low)
	
	server = "10.0.0.18"
	port = 389
	dn = "ou=Group, dc=rtgkom"
	filter="(&(objectClass=posixGroup)(cn=%s))" % (calced_class)
	justthese = ["cn", "gidnumber"]
	
	ldapconn = ldap.open(server, port)
	ldapconn.simple_bind_s()
	
	result = ldapconn.search_s(dn, ldap.SCOPE_SUBTREE, filter, None)
	
	users = result[0][1]["memberUid"]
	
	for i in range(len(users)):
		user_info = pwd.getpwnam(users[i])
		users[i] = user_info.pw_uid
	return users

def Main():
	global folders_traversed
	folders_traversed = 0
	
	global MaxSize
	MaxSize = 100
	
	global MaxDepth
	MaxDepth = 10
	
	global OutputFile
	
	global classname
	classname = None
	users = []
	
	global folders
	folders=0
	
	global starttime
	starttime=0
	global endtime
	endtime=0
		
	#Wether to write to file/terminal
	wtofile = 0
	wtoterm = 1
	passfirst = 0
		
	#Handle arguments. This isn't pretty :o
	for arg in sys.argv:
		arg = arg.lower()
		if arg=="noterm":
			wtoterm = 0
		#People are used to a --help because of standard option handling
		elif arg=="help" or arg=="--help":
			PrintHelp()
		elif arg.find("file=")==0:
			temp = arg.split("=")
			try:
				OutputFile = open(temp[1],"w")
				wtofile=1
			except IOError,error:
				print error
				PrintHelp()
		elif arg.find("size=")==0:
			temp = arg.split("=")
			try:
				int(temp[1])
				MaxSize = temp[1]
			except ValueError,error:
				print error
				PrintHelp()
		elif arg.find("depth=")==0:
			temp=arg.split("=")
			try:
				int(temp[1])
				MaxDepth = temp[1]
			except ValueError,error:
				print error
				PrintHelp()
		elif arg.find("class=")==0:
			temp = arg.split("=")
			classname = temp[1]
		elif passfirst==0:
			passfirst=1
			continue
		else:
			PrintHelp()
	
	MaxSize = float(MaxSize)
	MaxDepth = int(MaxDepth)
	
	if(classname!=None):
		if len(classname)>3 or classname.find(".")==-1:
			PrintHelp()
		else:
			temp = classname.split(".")
			if len(temp[0])>1 or len(temp[1])>1 or not temp[0].isdigit() or not temp[1].isdigit():
				PrintHelp()
			else:
				users = GetUsers(int(temp[0]), int(temp[1]))
				print "\nOnly reporting users from class %s" % classname
	else:
		print "\nPrinting for every user"
		
	print "Reporting folders deeper than %d" % (MaxDepth)
	print "Reporting files larger than %d MB" % (MaxSize)
	print "Reporting broken symlinks and empty folders."
	
	if wtofile:
		print "We will be writing the data to \"%s\"" % (OutputFile.name)
	else:
		print "It will not be output to a file"
		
	if wtoterm:
		print "Data will be written to terminal"
	else:
		print "We will not write to the terminal"
	
	while 1:
		inp = raw_input("Do you accept? (yes/no):")
		inp = inp.lower()
		if inp=="yes" or inp=="y":
			print "Ok. We will start now."
			break
		elif inp=="no" or inp=="n":
			print "Ok. Program will exit now.\n"
			if wtofile:
				print "Ok then. Goodbye :)"
				#Delete file
			exit()
	
	starttime = time.time()
	
	global rep
	rep = report()
	
	
	
	#Make it be B instead of MB :)
	MaxSize *= (1024*1024)
	MaxSize = int(MaxSize)
	
	print "Searching... Please wait"
	#Travels every directory from current and calls wfunc in each dir
	try:
		os.path.walk(".", wfunc, users)
	except OSError,err:
		print err
		print "You probably got \"[Errno 40]\". That's most likely a circular symlink :/"
		print "Exiting"
		exit()
	
	if wtoterm or wtofile:
		rep.PrepareOutput(users)
	
	if wtoterm:
		rep.Print()
	if wtofile:
		rep.WriteToFile()
	
	if not endtime:
		endtime = time.time()
	if not wtoterm:
		print "Done."
		print "Folders traversed: %d" % (folders_traversed)
		print "Execution took: %f Seconds" % (endtime-starttime)
		print "Time per folder: %f Seconds (Fun Fact)" % ((endtime-starttime)/folders_traversed)

if __name__=="__main__":
	try:
		import psyco
		psyco.full()
	except ImportError:
		pass
	Main()
