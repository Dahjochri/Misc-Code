
Bird[] birds;
PFont font;

void setup()
{
  size(640, 480);
  font = createFont("Courier", 16, true);
  
  initBirds();
}

void draw()
{
  background(255,255,255);

  
  drawBirds();
  
  textFont(font, 24);
  fill(0);
  textAlign(RIGHT);
  text(String.format("Total birds count: %d",birds.length), width/2, 100);
  
}

void drawBirds()
{
  for(int i=0; i<birds.length; i++)
  {
    birds[i].move(birds);
    birds[i].display();
  }
  
}

void mousePressed()
{
  if(mouseButton == LEFT)
  {
    birds = (Bird[]) expand(birds, birds.length + 1);
    birds[birds.length-1] = new Bird(mouseX, mouseY, birds.length-1);
  }
}

void keyPressed()
{
  if(key == ' ')
  {
    initBirds();
  }
}

void initBirds()
{
  birds = new Bird[3];
  birds[0] = new Bird(100,100,0);
  birds[1] = new Bird(164,10,1);
  birds[2] = new Bird(440,300,2);
}
