import processing.core.*; 
import processing.data.*; 
import processing.opengl.*; 

import java.applet.*; 
import java.awt.Dimension; 
import java.awt.Frame; 
import java.awt.event.MouseEvent; 
import java.awt.event.KeyEvent; 
import java.awt.event.FocusEvent; 
import java.awt.Image; 
import java.io.*; 
import java.net.*; 
import java.text.*; 
import java.util.*; 
import java.util.zip.*; 
import java.util.regex.*; 

public class lines_circles_test extends PApplet {

public void setup()
{
  size(480,120);
  background(180,50,50);
}

int mx_prev = 0;
int my_prev = 0;
int mx_chosen = 0;
int my_chosen = 0;
int timer = 0;

public void draw()
{
  if(timer++ == 10)
  {
    stroke(random(250), random(250), random(250));
    
    line(mx_chosen,my_chosen,mouseX,mouseY);
    ellipse(mouseX, mouseY, 5, 5);
    mx_prev = mouseX;
    my_prev = mouseY;
    timer = 0;
  }
}

public void mousePressed()
{
  if(mouseButton == LEFT)
  {
    mx_chosen = mx_prev;
    my_chosen = my_prev;
  }
  else if(mouseButton == RIGHT)
  {
    background(180,50,50);
  }
  else
  {
    saveFrame("output.png");
  }
}
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "lines_circles_test" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
