import processing.serial.*;

Serial myPort;
String data_string;
String[] data_arr;
int dist_cm;
float angle_deg;
float actual_step_angle = 1.8*0.75/4;
float angle_rad;
float x;
float y;
ArrayList <PVector> points = new ArrayList <PVector> ();
boolean clean_flag = false;

void setup() {
  myPort = new Serial(this, "COM15", 230400);
  
  size(820, 820);
  noSmooth();
  background(0);
  translate(410, 410);
}

void draw() {
  while (myPort.available() > 0) {
    data_string = myPort.readStringUntil(10);
    if (data_string != null) {
      data_arr = splitTokens(data_string, " ");   
      if(data_arr.length>1){
        stroke(255);
        strokeWeight(5);
        dist_cm = int(data_arr[0]);
        angle_deg = float(data_arr[1]);
        angle_rad = angle_deg*PI/180;
        
        x = dist_cm*cos(angle_rad) + 410;
        // minus sign is needed to align with the processing reference system
        y = -dist_cm*sin(angle_rad) + 410; 
        
        if(clean_flag) {
          stroke(0);
          strokeWeight(5);
          point(points.get(0).x,points.get(0).y);
          points.remove(0);
        }
        
        if(angle_deg + actual_step_angle >= 360) {
          clean_flag = true;
        }

        stroke(255);
        strokeWeight(5);
        points.add(new PVector(x,y));
        point(x,y);
      }
    }
  }
  myPort.clear();
}