class Bird
{
  public int id;
  color c;
  public float xpos;
  public float ypos;
  public PVector dir;
  float speed;
  float _speed;
  float critical_range;
  float short_range;
  float long_range;
  
  Bird(int _x, int _y, int _id)
  {
    id = _id;
    c = color(random(255), random(255), random(255));
    xpos = _x;
    ypos = _y;
    dir = new PVector(random(-1,1), random(-1,1));
    dir.normalize();
    
    //Rotate
    speed = 0.1;
    
    //move
    _speed = 0.5;
    
    //In pixels
    critical_range = 35;
    short_range = 50;
    long_range = 80;
  }
  
  void display()
  {
    noStroke();
    fill(c);
    ellipse(xpos, ypos, 10,10);
    ellipse(xpos + dir.x * 15, ypos + dir.y * 15, 3, 3);
    
    textFont(font, 12);
    textAlign(RIGHT);
    text(String.format("id:\n%d\nx: %f\ny:%f\nheading:%f", id, xpos, ypos, getHeadingInDegrees()), xpos + 10, ypos + 10);
  }
  
  void move(Bird[] bird_list)
  {
    float distance;
    boolean critical;
    PVector modify = new PVector(0,0);
    
    //Check if one is critically close
    for(int i=0; i<bird_list.length; i++)
    {
      if(bird_list[i].id == id)
        continue;
        
      distance = PVector.dist(dir, bird_list[i].dir)
      if( distance < critical_range)
      {
        critical = true;
      }
      
    }
    
    
    //Obtain average heading
    if(!critical)
    {
      for(int i=0; i<bird_list.length; i++)
      {
        if(bird_list[i].id == id)
          continue;
          
        distance = PVector.dist(dir, bird_list[i].dir)
        if( distance < long_range && distance  > short_range)
        {
          modify.add(bird_list[i].dir);
        }
        
    }
    }
    
    //Go clear of nearby neighbors
    
    if(!critical)
    {
      for(int i=0; i<bird_list.length; i++)
      {
        if(bird_list[i].id == id)
          continue;
          
          
        if(PVector.dist(dir, bird_list[i].dir)  < short_range)
        {
          modify.add(bird_list[i].dir);
        }
      }
    }
    
    if(critical)
    {
      for(int i=0; i<bird_list.length; i++)
      {
        if(bird_list[i].id == id)
          continue;
          
        if(PVector.dist(dir, bird_list[i].dir)  < critical_range)
        {
          modify.add(bird_list[i].dir);
        }
      }
    }
    
    //
    //Rotate towards angle of modified heading.
    dir.normalize();
    modify.normalize();
    float angle = acos(dir.x * modify.x + dir.y * modify.y);
    
    rotateTowards(true, angle);
    dir.normalize();
    
    xpos += dir.x * _speed;
    ypos += dir.y * _speed;
    
  }
  
  private void steerLeft()
  {
    //Steer left by deg degrees
    //Using turnTo and getHeading
  }
  
  private void rotateTowards(boolean wayone, float angle)
  {
    dir.x += wayone ? speed * ((dir.x - xpos) * cos(angle)) - ((ypos - dir.y) * sin(angle)) : -speed * ((dir.x - xpos) * cos(angle)) - ((ypos - dir.y) * sin(angle));
    dir.y += ((ypos - dir.y) * cos(angle)) - ((dir.x - xpos) * sin(angle));
  }
  
  private void turnTo(float deg)
  {
  }
  
  private float getHeadingInDegrees()
  {
    float a = atan2(dir.x, dir.y);
    if(a<0)
    {
      a += 2 * PI;
    }
    return a * 57.2957795;
  }
}
