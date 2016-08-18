import rb, gtk, rhythmdb, os, time, gobject, random
import pygame.mixer as mixer

class PlayrandomSource(rb.BrowserSource):
	def __init__(self):
		rb.BrowserSource.__init__(self)
		
class playrandomPlugin(rb.Plugin):
	def __init_(self):
		rb.Plugin.__init__(self)
	
	def activate(self, shell):
		print "playrandom plugin activated"
		self.shell = shell
		self.on = False
		self.slept = 0.0
		self.to_sleep = 0.0
		
		ui_str = "<ui><menubar name=\"MenuBar\"><menu name=\"ToolsMenu\" action=\"Tools\"><placeholder name=\"ToolsMenuModePlaceholder\"><menuitem name=\"ToolsMenuPlayrandom\" action=\"Playrandom\"/></placeholder></menu></menubar><toolbar name=\"ToolBar\"><placeholder name=\"ToolBarPluginPlaceholder\"><toolitem name=\"Playrandom Toggle\" action=\"Playrandom\"/></placeholder></toolbar></ui>"
		action = gtk.Action("Playrandom", "Playrandom", "Play songs in random intervals!", "")
		action.connect("activate", self.playrandom, shell)
		action_group = gtk.ActionGroup("PlayrandomActionGroup")
		action_group.add_action(action)
		manager = shell.get_ui_manager()
		manager.insert_action_group(action_group, 0)
		manager.add_ui_from_string(ui_str)
		manager.ensure_update()
		mixer.init(44100)
		mixer.music.load("/home/daniel/.gnome2/rhythmbox/plugins/playrandom/1.mp3")
	
	def deactivate(self, shell):
		"playrandom plugin deactivated"
		del self.shell
		del self.on
		del self.slept
		del self.to_sleep
		mixer.quit()
		
	def playrandom(self, event, shell):
		if self.on is True:
			self.on = False
			print "playrandom plugin off"
		else:
			self.on = True
			print "playrandom plugin on"
			self.slept = 0.0
			self.to_sleep = self.get_random_sleep()
			gobject.idle_add(self.idle_cb)
	
	def get_random_sleep(self):
		random.seed(time.time())
		sleeper = random.uniform(120,300)
		print "Time to sleep: %f" % (sleeper)
		return sleeper
		
	def idle_cb(self):
		gtk.gdk.threads_enter()
		if not self.on:
			cleanup()
			gtk.gdk.threads_leave()
			return False
		
		
		time.sleep(0.1)
		self.slept += 0.1
		if self.slept > self.to_sleep:
			print "Self.slept was: %f when do_random_play was called" % (self.slept)
			print "Self.to_sleep was: %f when do_random_play was called" % (self.to_sleep)
			self.do_random_play()
			self.slept = 0.0
			self.to_sleep = self.get_random_sleep()
			
		gtk.gdk.threads_leave()
		return True
			
	def do_random_play(self):
		self.shell.props.shell_player.playpause()
		chance = random.randint(0,100)
		mixer.music.play()
		time.sleep(25)
		mixer.music.stop()
		mixer.music.rewind()
		self.shell.props.shell_player.playpause()
