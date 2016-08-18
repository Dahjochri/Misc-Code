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

void draw()
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

void mousePressed()
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
