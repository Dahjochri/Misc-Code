# Misc-Code
Just safe-keeping a bunch of random code and small projects (some "completed" others not even half done). I had so much old dumb code and near-empty projects lying around. These are the "somewhat finished or 'at least code-filled' projects". Make of it what you wish.
Most of it is probably sparsely, if at all, documented. Enter the files at your own risk.



## List of projects and short descriptions
* [Interpreters][interpreters-folder] - _Programs that interpret simple esoteric languages_
  * [Brainfuck Interpreter][BF-interpreter] : Simple program (in [C][BF-interpreter-C] and [C++][BF-interpreter-C++]) to interpret [brainfuck code][BF-description].
  * [HQ9+ Interpreter][HQ9+-interpreter] : Simple program (in [C][HQ9+-interpreter-C] and [C++][HQ9+-interpreter-C++]) to interpret [HQ9+ code][HQ9+-description].


* [Image Processing and Computer Vision][ip-and-cv] - _Some small projects with image processing and computer vision_
  * [30 OpenCV Filters][30-filters] : Showcase of 30 filtering methods using [OpenCV](http://opencv.org/), written in C++. I made [a video explaining them](https://www.youtube.com/watch?v=r6dPw1uhvj4).
  * ["OpenCV Play"][hog-people] : Aptly names project, where I play around with people detection using [HOG-descriptors](https://en.wikipedia.org/wiki/Histogram_of_oriented_gradients) in [OpenCV](http://opencv.org/), written in C++.
  * ["Project Final Dogedown"][dogedown] : Even more aptly named project. Also C++. This was some application using webcam input, [background subtraction](https://www.ics.uci.edu/~dramanan/teaching/cs117_spring13/lec/bg.pdf) and [blob analysis](http://what-when-how.com/introduction-to-video-and-image-processing/blob-analysis-introduction-to-video-and-image-processing-part-1/) to make some weird fractal-like things move when you're in the camera. Yah.


* [Networking][networking] - _Projects relating to networking stuff_
  * [Client Pooling][pooling] : Playing around with server application pooling clients, with and without threads, in C.


* [Processing][processing] - _stuff written in [Processing language/IDE](https://processing.org/)_
  * [Flocking][flocking] : Some unfinished code for playing around with [flocking behavior](http://www.red3d.com/cwr/boids/).
  * [Lines and Circles][lines_circles] : Simple drawing lines and circles code.


* [Random][random] - _Stuff I didn't know where to put I guess_
  * [99 Bottles of Beer][99] : Classic simple programming challenge, in [C][99-c], [Python][99-p], and, yes, [Brainfuck][99-bf] (and yes, it works xD).
  * [C++ Classes and Templates][classes_templates] : Playing around with classes and templates in C++.
  * [Hack This Site Challenges][hts] : Code for completing a couple of challenges and [Hack This Site!](https://www.hackthissite.org/).
  * ["napGet"][napget] and ["napGet2"][napget2] : the National Academic Press once had free books so obviously I wrote a script to get them all. I really can't remember if it worked, or why I made 2 of the project (one Python, one C#).
  * [NOTES][notes] : A notes application (appends to file). Simple file I/O play in [C][notes-c] and [C++][notes-cpp].
  * [rtgkom cleanup][rtgkom] : Python script I once wrote for cleaning up large files, identifying "problematic" users, etc., for a server (uses ldap).
  * [WIN32 GUI by rc file][guibyrc] : Testing working with WIN32 GUI in rc files or something.


* [Sound][sound] - _Projects related to sound obviously_
  * [PlayRandom][playrandom] : A simple plugin for the [Rhythmbox](https://wiki.gnome.org/Apps/Rhythmbox) player that, within bracketed random interval, plays a random song. Used for a fun drinking game once (Y).


* [Utility][utility] - _Projects that are "utility projects". I don't even know anymore man_
  * [MyShell][myshell] : some basic groundwork for a windows shell I made, I guess for practicing C#.


[//]: # (Links to stuff, for easier future manipulation)

[interpreters-folder]: <./Interpreters/>

[BF-interpreter]: <./Interpreters/BF%20Executer/>
[BF-interpreter-C]: <./Interpreters/BF%20Executer/C/>
[BF-interpreter-C++]: <./Interpreters/BF%20Executer/C%2B%2B/>
[BF-description]: <https://esolangs.org/wiki/Brainfuck>

[HQ9+-interpreter]: <./Interpreters/HQ9%2B%20Executer/>
[HQ9+-interpreter-C]: <./Interpreters/HQ9%2B%20Executer/C/>
[HQ9+-interpreter-C++]: <./Interpreters/HQ9%2B%20Executer/C%2B%2B/>
[HQ9+-description]: <https://esolangs.org/wiki/HQ9%2B>



[ip-and-cv]: <./Image%20Processing%20and%20Computer%20Vision/>
[30-filters]: <./Image%20Processing%20and%20Computer%20Vision/vs10_OpenCV%20IP%2030%20Filters/>
[hog-people]: <./Image%20Processing%20and%20Computer%20Vision/vs10_OpenCV%20Play/>
[dogedown]: <./Image%20Processing%20and%20Computer%20Vision/Project_Final_Dogedown/>



[networking]: <./Networking/>
[pooling]: <./Networking/Client%20Pooling/>



[processing]: <./Processing/>
[flocking]: <./Processing/flocking%20test/>
[lines_circles]: <./Processing/lines_circles_test/>



[random]: <./Random/>
[99]: <./Random/99%20Bottles%20of%20Beer/>
[99-bf]: <./Random/99%20Bottles%20of%20Beer/Brainfuck/>
[99-c]: <./Random/99%20Bottles%20of%20Beer/C/>
[99-p]: <./Random/99%20Bottles%20of%20Beer/Python/>

[classes_templates]: <./Random/C%2B%2B%20Classes%20%26%20Templates/>

[hts]: <./Random/Hack%20This%20Site%20Challenges/>

[napget]: <./Random/napGet/>
[napget2]: <./Random/napGet2>

[notes]: <./Random/NOTES/>
[notes-c]: <./Random/NOTES/C/>
[notes-cpp]: <./Random/NOTES/C%2B%2B/>

[rtgkom]: <./Random/rtgkom%20cleanup/>

[guibyrc]: <./Random/WIN32%20GUI%20by%20rc%20file/>



[sound]: <./Sound/>
[playrandom]: <./Sound/Rhythmbox%20Plugins/playrandom/>



[Utility]: <./Utility>
[myshell]: <./Utility/MyShell/>
