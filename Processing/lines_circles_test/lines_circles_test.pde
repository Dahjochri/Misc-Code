void setup()
{
  size(480,120);
  background(180,50,50);
}

int mx_prev = 0;
int my_prev = 0;
int mx_chosen = 0;
int my_chosen = 0;
int timer = 0;
boolean drawing = false;

void draw()
{
  if(timer++ == 5)
  {
    stroke(255,0,0);
    ellipse(mx_chosen, my_chosen,8,8);
    
    if(drawing==true)
    {
      stroke(random(250), random(250), random(250));
      line(mx_chosen,my_chosen,mouseX,mouseY);
      ellipse(mouseX, mouseY, 5, 5);
      
      mx_prev = mouseX;
      my_prev = mouseY;
    }
    if(mouseButton != LEFT)
      drawing = false;
     
    timer = 0;
  }
}

void mousePressed()
{
  if(mouseButton == LEFT)
  {
    drawing = true;
  }
  else if(mouseButton == RIGHT)
  {
    mx_chosen = mx_prev;
    my_chosen = my_prev;
  }
  else if (mouseButton == CENTER)
  {
    background(180,50,50);
    mx_chosen = mouseX;
    my_chosen = mouseY;
  }
}


void keyPressed()
{
  if(key==ENTER)
  {
    saveFrame("output.png");
  }
}
