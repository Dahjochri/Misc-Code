import os, os.path, urllib,urllib2,re,sys, multiprocessing, glob
import reportlab

def getBook(id):
	
	try:
		print "\n\n--------\nID: %d" % (id)
		url = "http://www.nap.edu/catalog.php?record_id=%d" % (id)
		iwantname = urlopen(url)
		page = iwantname.read()
		
		
		if name == "":
			#Not a book we want.
			return
		print name
		#remove some dirty chars
		for char in ('/','\\','?','*',':','|','"','<','>'):
			name = name.replace(char,"_")
		
		print "filename: \"" + name + "\""
		if os.path.isfile(name + ".pdf"):
			print "File already exists.. Neeevermind"
			return
		pass
		#Make sure it's free (else we hax)
		
		if res != None:
			#http://www.nap.edu/openbook/0309036380/gifmid/21.gif
			pattern = re.compile("<meta property=\"og:image\" content=\"http://images.nap.edu/images/minicov/(0309036380).gif\"/>")
			res = pattern.search(page)
			if res == None:
				#error
				exit(1)
			else:
				theId = res.group(1)
				print theId
				dirname = str(id)
				if not os.path.isdir(os.path.join(os.getcwd(), dirname)):
					os.mkdir(os.path.join(os.getcwd(), dirname))
				counter = 1
				while(1):
					
					gurl = "http://www.nap.edu/openbook/0309036380/gifmid/" + str(counter) + ".gif"
					thegif = urlopen(gurl).read()
					if "<html" in thegif:
						break
					fout = open("./" + str(id) + "/" + str(counter) + ".gif","wb")
					fout.write(thegif)
					fout.close()
					counter += 1
				canv = canvas.Canvas(name + ".pdf")
				for fn in glob.glob("./" + str(id) + "/*.gif"):
					dim = c.drawImage(fn, 0, 0)
					canv.setPageSize(dim)
					canv.showPage()
				canv.save()
				exit(0)
				
		url = "http://download.nap.edu/cart/download.cgi?&record_id=%d&free=1" % (id)
		req = Request(url, data, headers)
		pdf = urlopen(req)
		pdf_read = pdf.read()
		
		if len(name)>0:
			name += ".pdf"
			f = open(name, "wb")
			f.write(pdf_read)
			filesize = f.tell()
			f.close()
		# and open it to return a handle on the url	
	except IOError, e:
		print 'We failed to open "%s".' % url
		if hasattr(e, 'code'):
			print 'We failed with error code - %s.' % e.code
		elif hasattr(e, 'reason'):
			print "The error object has the following 'reason' attribute :"
			print e.reason
			print "This usually means the server doesn't exist,"
			print "is down, or we don't have an internet connection."
		sys.exit()

class napGet:
	def __init__(self):
		try:
			import cookielib
		except ImportError:
			# If importing cookielib fails
			# let's try ClientCookie
			try:
				import ClientCookie
			except ImportError:
				# ClientCookie isn't available either
				urlopen = urllib2.urlopen
				request = urllib2.Request
			else:
				# imported ClientCookie
				urlopen = ClientCookie.urlopen
				request = ClientCookie.Request
				cj = ClientCookie.LWPCookieJar()
		else:
			# importing cookielib worked
			urlopen = urllib2.urlopen
			request = urllib2.Request
			cj = cookielib.LWPCookieJar()
			# This is a subclass of FileCookieJar
			# that has useful load and save methods
		if cj is not None:
		# we successfully imported
		# one of the two cookie handling modules
			# Now we need to get our Cookie Jar
			# installed in the opener;
			# for fetching URLs
			if cookielib is not None:
				# if we use cookielib
				# then we get the HTTPCookieProcessor
				# and install the opener in urllib2
				opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cj))
				urllib2.install_opener(opener)
			else:
				# if we use ClientCookie
				# then we get the HTTPCookieProcessor
				# and install the opener in ClientCookie
				opener = ClientCookie.build_opener(ClientCookie.HTTPCookieProcessor(cj))
				ClientCookie.install_opener(opener)
		else:
			exit(1)
	
	user_agent = 'Mozilla/4.0 (compatible; MSIE 5.5; Windows NT)'
	values = {"action":"signin",
				"record_id":"%d" % (id),
				"page":"/catalog.php?record_id=%d" % (id),
				"email":"lol@lol.com",
				"first_name":"hai",
				"last_name":"bai",
				"sector":"Student"}
	headers = { 'User-Agent' : user_agent }
	data = urllib.urlencode(values)
	
	self.urlopen = urlopen
	self.request = request
	self.cj = cj
	self.data = data
	self.headers = headers
	
	
	def getBook(self, id):
		self.id = id
		self.data = downloadItem("http://www.nap.edu/catalog.php?record_id=%d" %(id))
		self.name = getName()
		if self.name == "":
			exit(1)
			
		if isFree():
			getBookNormal()
		else:
			getBookGif()
			makePdf()
		
	def getBookNormal(self):
		pass

	def getBookGif(self):
		pass
	
	def getName(self):
		pattern = re.compile("div id=\"biblio_1\".+?<h1>(.+?)</h1>.+?<h2>(.*?)</h2>", re.DOTALL)
		res = pattern.search(self.data)
		name1 = ""
		name2 = ""
		if res != None:
			(name1,name2) = res.groups()
		name = name1 + name2
		return name
		
	def isFree(self):
		pattern = re.compile("No free downloads available for this title", re.DOTALL)
		res = pattern.search(self.data)
		if res == None:
			return True
		else:
			return False
	
	def makePdf(self):
		canv = canvas.Canvas(self.name + ".pdf")
		for fn in glob.glob("./" + str(self.id) + "/*.gif"):
			dim = c.drawImage(fn, 0, 0)
			canv.setPageSize(dim)
			canv.showPage()
		canv.save()
	
	def downloadItem(self, url):
		req = Request(url, self.data, self.headers)
		lol = urlopen(req)
		return lol.read()

if __name__ == "__main__":

		
	getBook(1, urlopen, request, cj)