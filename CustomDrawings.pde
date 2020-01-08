
x=0;
y=0;
x1=0;
y1=0;
x2 = 0; 
y2 = 0;
k = 0;
angleList = { };
distList = { };
w = 0;

void setup() {
	size(800, 800);
	background(0);
}

void mousePressed() {
	
}

void keyPressed() {
	w = 0;
  list = { };
	background(0);
}

void draw() {
	if (mousePressed == true) {
  w = 1;
	x = mouseX;
	y = mouseY;
	x1old = x1;
	y1old = y1;
	x1x = mouseX/8;
	y1x = (800 - mouseY)/8;
	x1 = 90-degrees(atan(y1x/x1x));
	y1 = sqrt(sq(x1x)+sq(y1x));
	if(!(x1 == x1old)){
	angleList = append(angleList, round(x1));}
	if(!(y1 == y1old)){
	distList = append(distList, round(y1));}
	print("angle:"+str(angleList));
	print("dist:"+str(distList));
	k++;
	}
	if (w == 1){
	strokeWeight(5);
	ellipse(x,y,3,3);
	strokeWeight(1);
	stroke(255);
	if(k > 1){
	line(x2, y2, x, y);
	}
	x2 = x;
	y2 = y;
	
	}
}