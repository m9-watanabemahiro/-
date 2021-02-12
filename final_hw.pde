float baseH =10;
float armL1 = 40;
float armL2 = 40;
float armL3 = 40;
float armL4 = 40;
float armL5 = 40;
float armW1 = 10;
float armW2 = 10;
float armW3 = 10;
float armW4 = 10;
float armW5 = 5;
float angle1 = 0;
float angle2 = 0;
float angle3 = 0;
float angle4 = 0;
float angle5 = 0;
float l1 = 0;
float dif = 1.0;

void setup() {
  size(1200, 800, P3D);
  //background(255);

  camera(200, 200, 200, 0, 0, 0, 0, 0, -1);

  noStroke();
}

void draw() {

  background(255);

  if (keyPressed) {
    if (key == '1') {
      angle1 = angle1 + dif;
    }
    if (key == 'q') {
      angle1 = angle1 - dif;
    }
    if (key == '2') {
      angle2 = angle2 + dif;
    }
    if (key == 'w') {
      angle2 = angle2 - dif;
    }
    if (key == '3') {
      angle3 = angle3 + dif;
    }
    if (key == 'e') {
      angle3 = angle3 - dif;
    }
    if (key == '4') {
      angle4 = angle4 + dif;
    }
    if (key == 'r') {
      angle4 = angle4 - dif;
    }
    if (key == '5') {
      angle5 = angle5 + dif;
    }
    if (key == 't') {
      angle5 = angle5 - dif;
    }
    if (l1>10) {
      l1=l1+0;
    } else if (keyPressed) {
      if (key == '6') {
        l1 = l1 + dif;
      }
    }
    if (l1<-10) {
      l1=l1-0;
    } else if (keyPressed) {
      if (key == 'y') {
        l1 = l1 - dif;
      }
    }
  }

  //base
  translate(0, 0, baseH/2);
  fill(175);
  box(30, 30, baseH);

  //1st link
  rotateZ(radians(angle1));
  translate(0, 0, baseH/2+armL1/2);
  fill(150);
  box(armW1, armW1, armL1);

  //2nd link
  //go to 2nd joint
  translate(armW2, armL2/2-2*armW1, armL1/2-armW2/2);
  rotateX(radians(angle2));  
  //go to center of 2nd joint
  translate(0, armL2/2-armW1/2, 0);
  fill(135);
  box(armW2, armL2, armW2);

  //3nd link
  //go to 3nd joint
  translate(armW3, armL3-2*armW2-armW2/2, 0);
  rotateX(radians(angle3));  
  //go to center of 3nd joint
  translate(0, armL3/2-armW2/2, 0);
  fill(120);
  box(armW3, armL3, armW3);

  //4rd link
  //go to 4rd joint
  translate(0, armL4-2*armW3-armW3/2, armW3);
  rotateZ(radians(angle4));  
  //go to center of 4rd joint
  translate(0, armW4+armW3/2, 0);
  fill(110);
  box(armW4, armL4, armW4);

  //5th link Rotate
  //go to 5th joint
  translate(0, armL4/2-armW4/2, 0);
  rotateZ(radians(angle5));  

  //5th link Straight
  translate(0, 0, l1);
  fill(100);
  box(armW5, armW5, armL5);
  translate(0, 0, -armL5/2-2.5);
  fill(50);
  box(5, 10, 5);
  translate(0, 3.5, -6.5);
  fill(50);
  box(5, 3, 8);
  translate(0, -7, 0);
  fill(50);
  box(5, 3, 8);
}
