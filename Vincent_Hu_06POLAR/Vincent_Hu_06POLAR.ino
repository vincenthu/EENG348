/* Vincent Hu */

/* Constants */
float r1 = 5;                        //radius of smaller circle
float r2 = 10;                       //radius of larger circle
float pi=3.1415926;
int realrow = 0, realcolumn = 0;     //running values of real row and column values (based on serial plot)
float scalar_x = 10 ,scalar_y = 4;

/* Variables */
float x,y,x1,y1,x2,y2;

void printDot(int row, int column)
{
  /* Go to the row */
  while(realcolumn < column)
  {
    Serial.print(' ');
    realcolumn++;
  }
  
  /* Go to the column */
  while(realrow < row)
  {
    Serial.println();
    realcolumn = 0;
    realrow++;
  }

  Serial.print('.');
}

void setup() 
{
  Serial.begin(115200);
}

void loop()
{
  for (int i1 = 90, i2 = 90; i1 > -90 || i2 > -90; )
  {
    /*Calculate coords of small circle*/
    x1=r1*cos(i1*pi/180);
    y1=r2-r1*sin(i1*pi/180);

    /*Calculate coords of big circle*/
    x2=r2*cos(i2*pi/180);
    y2=r2-r2*sin(i2*pi/180);
    
    /* Determine which point to be printed first */
    if((int)(y1*scalar_y)<(int)(y2*scalar_y))  // weighted y1 < y2
    {
      x=x1;  
      y=y1;
      i1 -= 6;
    }
    else if((int)(y1*scalar_y)>(int)(y2*scalar_y)) // weighted y1 > y2
    {
      x=x2;  
      y=y2;
      i2 -= 6;
    }
    else if((int)(x1*scalar_x)>(int)(x2*scalar_x)) // weighted x1 > x2
    {
      x=x2;  
      y=y2;
      i2 -= 6;
    }
    else  // weighted x1 < x2
    {
      x=x1;  
      y=y1;
      i1 -= 6;
    }
    
    /* Set next point for larger circle if smaller than -90 degrees */
    if(i1 <= -90)
    {
      x=x2;  
      y=y2;
      i2 -= 6;
    }
    
    printDot(y*scalar_y,x*scalar_x);
  }
  
  while(1); //loop forever when finished
}



