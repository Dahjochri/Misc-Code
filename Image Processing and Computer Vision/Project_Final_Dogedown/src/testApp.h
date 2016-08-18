#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Blobalizer.h"
#include "BGSub.h"

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

    //---------------------------------------
	//video and turn to mirrored
    ofVideoGrabber 		vidGrabber;
    int 				camWidth;
    int                 camHeight;
    unsigned char *     videoMirror;
    ofTexture           mirrorTexture;
	//---------------------------------------
	//BG SUB
	unsigned char *     BGoutgray;
	unsigned char *     BGout;
    unsigned char *     Erout;
    unsigned char *     dilout;
    ofTexture           diloutexture;

    //blobanalysis
    BGSub               bgs;
    Blobalizer          blobber;
    bool                beginBlob;
    vector<Blob>        blobs;
    //---------------------------------------
    //the gui
	bool bHide;
	ofxFloatSlider Length;
	ofxFloatSlider middelAngle;
	ofxFloatSlider spinSpeed;
	ofxButton newColor;
	ofxIntSlider endSize;
	ofxToggle BG;
	ofxPanel gui;
	//---------------------------------------
	//logging
	ofImage Save1,Save2;
	ofPixels Pixs;
	size_t framesRun;

    //---------------------------------------
	//screen size
	int screenX = 1424;
	int screenY = 768;

    //---------------------------------------
	//fractal holder
	struct FracHolder{
		    //---------------------------------------------------
		    //allowed to mess with
		    //speed is controlled but scale speed and max speed
		    //speed of charge
		    float ScaleS = 0.5f;
		    //max speed cap
		    float MaxS = 1.0f;

            //RACG implementation variables
		    bool dark;
		    float red = ofRandom(10,255);
		    float green = ofRandom(10,255);
		    float blue = ofRandom(10,255);
            bool colordone = false;
		    //---------------------------------------------------
		    //vectors for movement
            ofVec2f location;
            ofVec2f velocity;
            ofVec2f target;
            ofVec2f target_vec;
            //radius
            unsigned char radius = 40;
            //d is for automation the rotation
            float d;
            // basically 1/phi
            float magic = 1/((1+sqrt(5))/2);
            //functions
            void movement(){
                //first part of the movement: move to target
               //gets the vector between target and location
                ofVec2f dir = target-location;
                //normalizes it but dividing it but is own length
                // dir/||dir||= 1 aka unit vector
                dir.normalize();
                //scales each part of the vector
                //st
                dir.scale(ScaleS);
                //adds dir to the velocity velocity
                velocity = velocity+dir;
                //sets a limit so the velocity down just rise and rise and rise
                velocity.limit(MaxS);
                //adds the velocity to the location
                location = location+velocity;

                //second part of the movement: move beyond target
                // gets the distance from the target and location in a number
                float Length = location.distance(target);
                unsigned char sum = 5;
                if(Length < sum){
                    target.set(location);
                }
            }
            void fractalUno(int x, int y,float L,float EL,float MA){
                ofPushMatrix();
                ofTranslate(x, y, 0);

                topSticks(L,EL,MA);
                lowSticks(L,EL,MA);
                ofPopMatrix();
            }
            //recursive functions
            void topSticks(float L,float EL,float MA){
            //shortens the lengh
            L*=magic;
            //end con
            if(L>EL){
                // left stick
                ofPushMatrix();
                ofRotate(d);
                //---------------

                ofLine(0,0,0,-L);
                //---------------
                //translate point
                ofTranslate(0,-L,0);
                topSticks(L,EL,MA);
                ofPopMatrix();

                // right stick
                ofPushMatrix();
                ofRotate(d-MA);
                //---------------
                ofLine(0,0,0,-L);
                //---------------
                //translate point
                ofTranslate(0,-L,0);
                topSticks(L,EL,MA);
                ofPopMatrix();
            }
        }
            void lowSticks(float L,float EL,float MA){
            //shortens the lengh
            L*=magic;
            //end con
            if(L>EL){
                // left stick
                ofPushMatrix();
                ofRotate(d);
                //---------------
                ofLine(0,0,0,L);
                //---------------
                //translate point
                ofTranslate(0,L,0);
                lowSticks(L,EL,MA);
                ofPopMatrix();

                // right stick
                ofPushMatrix();
                ofRotate(d-MA);

                ofLine(0,0,0,L);
                //translate point
                ofTranslate(0,L,0);
                lowSticks(L,EL,MA);
                ofPopMatrix();
            }
        }

    };
    //vector to hold the FHolds
    vector<FracHolder>FHolds;

    //---------------------------------------
    struct Timer{
    //has to be set to 0 due to c++ giving random derps
    int savedtime = 0;
    // the amount of time between ticks in milliseconds
    int totaltime = 0;
    void sTart(){
        savedtime = clock();
    }
    bool isFinished(){
        //get the clock/milliseconds from the pc
        float passedtime = clock() - savedtime;
        //checks if the clock is higher than the 5 sec then tisks and returns true
        if(passedtime>totaltime){
            return true;
        }else{
            return false;
        }
    }
};

    //---------------------------------------
    //IDLE STATE TIMERS
    Timer SpinChange;
    Timer AngleChange;
    Timer LengthChange;
    Timer ColorChange;

    //---------------------------------------
    //IDLE STATE SECTION
    //GLOBAL
    //to controll the spin speed
    float MinS,MaxS;
    float sS;
    //to controll the angle
    float MinA,MaxA;
    float A;
    //to controll the size of each fractal
    int MinLe,MaxLe;
    float L,e;
    // adding/removing
    int MinF,MaxF;
    bool spawn;
    //---------------------------------------
    //RACG global implementation variables
    //begin color
    int R = 0;
    int G = 0;
    int B = 0;
    //actual color
    float Ar = 0;
    float Ag = 0;
    float Ab = 0;
    //---------------------------------------
    //fbo ofr making the entire screen into pictures
    ofFbo fbo;
    //---------------------------------------
    //fucntions
    void FractalDisplay_RACG(unsigned char r,unsigned char g, unsigned char b,unsigned char alpha){

            //-------------------------------------------
            //sets the initial color and fractal
            FHolds[FHolds.size()-1].red = r;
            FHolds[FHolds.size()-1].green = g;
            FHolds[FHolds.size()-1].blue = b;
            ofSetColor(ofColor((int)FHolds[FHolds.size()-1].red,(int)FHolds[FHolds.size()-1].green,(int)FHolds[FHolds.size()-1].blue,alpha));
            FHolds[FHolds.size()-1].fractalUno(FHolds[FHolds.size()-1].location.x,FHolds[FHolds.size()-1].location.y,Length,endSize,middelAngle);
            //color display
            for (int i= FHolds.size()-2;i>=0;i--) {
                // RACG
                //makes sure that it only runs once for eash fractal
                if(!FHolds[i].colordone){
                    //the next two if decide w
                    if(FHolds[FHolds.size()-1].dark){
                            //ORIGINAL: Ar = (FHolds[FHolds.size()-1].red/255)*FHolds[i].red;
                        Ar = (FHolds[FHolds.size()-1].red/255)*FHolds[i].red;
                        Ag = (FHolds[FHolds.size()-1].green/255)*FHolds[i].green;
                        Ab = (FHolds[FHolds.size()-1].blue/255)*FHolds[i].blue;
                        //one of the formulas are worng
                        FHolds[FHolds.size()-1].dark = false;
                    }
                    else{
                        Ar = (FHolds[FHolds.size()-1].red-((FHolds[FHolds.size()-1].red/255)*FHolds[i].red))+FHolds[FHolds.size()-1].red;
                        Ag = (FHolds[FHolds.size()-1].green-((FHolds[FHolds.size()-1].green/255)*FHolds[i].green))+FHolds[FHolds.size()-1].green;
                        Ab = (FHolds[FHolds.size()-1].blue-((FHolds[FHolds.size()-1].blue/255)*FHolds[i].blue))+FHolds[FHolds.size()-1].blue;
                        FHolds[FHolds.size()-1].dark = true;
                    }
                    //makes sure we dont go out of scale
                    if(Ar>255){
                        Ar=255;
                    }
                    if(Ar<0){
                        Ar=0;
                    }
                    if(Ag>255){
                        Ag=255;
                    }
                    if(Ag<0){
                        Ag=0;
                    }
                    if(Ab>255){
                        Ab=255;
                    }
                    if(Ab<0){
                        Ab=0;
                    }
                    //sets the internal colors to the global
                    FHolds[i].red = Ar;
                    FHolds[i].green = Ag;
                    FHolds[i].blue = Ab;
                    //debug
                    //cout<<FHolds[FHolds.size()-1].red<<"-----"<<FHolds[i].red<<endl;
                    //sets it true so it only happens once
                    FHolds[i].colordone = true;
                }


                //}
                //set the color to the modified color
                ofSetColor(ofColor((int)FHolds[i].red,(int)FHolds[i].green,(int)FHolds[i].blue,alpha));
                FHolds[i].fractalUno(FHolds[i].location.x,FHolds[i].location.y,Length,endSize,middelAngle);
            }

    }
    void autorotate(unsigned char i){


        //-------------------------------------------
                //AUTOROTATION
                if(FHolds[i].d<=720){
                    FHolds[i].d-=spinSpeed;
                }
                if(FHolds[i].d>720){
                    FHolds[i].d=0;
                }

    }
    void edgeBounce(unsigned char i){
            //bounce on the edges
        int FracHalfRadius = FHolds[i].radius/2;
        if(FHolds[i].location.x> screenX-FracHalfRadius || FHolds[i].location.x< FracHalfRadius){
          FHolds[i].velocity.x = FHolds[i].velocity.x * -1;

        }
        if(FHolds[i].location.y>screenY-FracHalfRadius || FHolds[i].location.y<FracHalfRadius){
          FHolds[i].velocity.y = FHolds[i].velocity.y * -1;
        }
    }
    void edgeWrap(unsigned char i){
                //EDGEWRAP
                int FracHalfRadius = FHolds[i].radius/2;

                if(FHolds[i].location.x> screenX-FracHalfRadius){
                    FHolds[i].location.x = FracHalfRadius;
                    FHolds[i].target.x = FracHalfRadius;
                }
                if(FHolds[i].location.x< FracHalfRadius){
                    FHolds[i].location.x = screenX-FracHalfRadius;
                    FHolds[i].target.x = screenX-FracHalfRadius;
                }
                if(FHolds[i].location.y> screenY-FracHalfRadius){
                    FHolds[i].location.y = FracHalfRadius;
                    FHolds[i].target.y = FracHalfRadius;
                }
                if(FHolds[i].location.y < FracHalfRadius){
                    FHolds[i].location.y = screenY-FracHalfRadius;
                    FHolds[i].target.y = screenY-FracHalfRadius;
                }
    }
    void collision(unsigned char i){

                 for(int j = 0; j<FHolds.size();j++){
                    //so it dosent check for collision with it self
                    if(i!=j){

                        float Length = FHolds[i].location.distance(FHolds[j].location);
                        int sum = 50;

                        if(Length < sum){
                            //what happens on collision big ball
                            ofVec2f between;
                            between.set(FHolds[i].location.x-FHolds[j].location.x,FHolds[i].location.y-FHolds[j].location.y);
                            ofVec2f between2;
                            between2.set(FHolds[j].location.x-FHolds[i].location.x,FHolds[j].location.y-FHolds[i].location.y);
                            between.scale(50);
                            between2.scale(50);
                            FHolds[i].target.set(FHolds[j].location.x+between.x,FHolds[j].location.y+between.y);
                            FHolds[i].target_vec.set(FHolds[i].target-FHolds[i].location);
                            FHolds[j].target.set(FHolds[i].location.x+between2.x,FHolds[i].location.y+between2.y);
                            FHolds[j].target_vec.set(FHolds[j].target-FHolds[j].location);


                        }
                    }
                }
    }
    void IdleChange(){
        if (SpinChange.isFinished()) {
            //in here the tick happens
            spinSpeed = ofRandom(MinS,MaxS);
            //resets the timer
            SpinChange.sTart();
        }
        if (AngleChange.isFinished()) {
            //random angle
            middelAngle = ofRandom(MinA,MaxA);
            //resets the timer
            AngleChange.sTart();
        }
        if (LengthChange.isFinished()) {
            //initital length of the sticks
            Length = ofRandom(MinLe,MaxLe);
            //end length, the length of the smallest possible stick
            endSize = Length*0.20;
            //resets the timer
            LengthChange.sTart();
        }
        if (ColorChange.isFinished()) {
            //color of the first one
            R = ofRandom(10,205);
            G = ofRandom(10,205);
            B = ofRandom(10,205);
            //cout<<"cut"<<endl;
            //resets to possibility to change the colors for each
            for (int i= FHolds.size()-2;i>=0;i--) {
                FHolds[i].colordone = false;
                FHolds[i].red = ofRandom(10,255);
                FHolds[i].green = ofRandom(10,255);
                FHolds[i].blue = ofRandom(10,255);
            }

            //resets the timer
            ColorChange.sTart();
        }
    }
    void DoTheFlip(unsigned char * output,int W,int H){
            //gets all the pixels
            unsigned char* pixels = vidGrabber.getPixels();
            //do the flip
            for (int i = 0; i < H; i++) {
                for (int j = 0; j < W*3; j+=3) {
                    // pixel number
                    int pix1 = (i*W*3) + j;
                    int pix2 = (i*W*3) + (j+1);
                    int pix3 = (i*W*3) + (j+2);
                    // mirror pixel number
                    int mir1 = (i*W*3)+1 * (W*3 - j-3);
                    int mir2 = (i*W*3)+1 * (W*3 - j-2);
                    int mir3 = (i*W*3)+1 * (W*3 - j-1);
                    // swap pixels
                    output[pix1] = pixels[mir1];
                    output[pix2] = pixels[mir2];
                    output[pix3] = pixels[mir3];
                }
            }
    }
    void PolyCollision(unsigned char i){
        for(int j = 0; j<blobs.size();j++){

          Blob tBlob = blobs[j];

          //Make everything proportional to screen instead of cam
          tBlob.bounding_rect.top_left.x *=  screenX/camWidth;
          tBlob.bounding_rect.top_left.y *=  screenY/camHeight;
          tBlob.bounding_rect.bottom_right.x *= screenX/camWidth;
          tBlob.bounding_rect.bottom_right.y *= screenY/camHeight;

          if(FHolds[i].location.x + FHolds[i].radius < tBlob.bounding_rect.top_left.x-1
             || FHolds[i].location.y + FHolds[i].radius < tBlob.bounding_rect.top_left.y-1
             || FHolds[i].location.x - FHolds[i].radius > tBlob.bounding_rect.bottom_right.x+1
             || FHolds[i].location.y - FHolds[i].radius > tBlob.bounding_rect.bottom_right.y+1){

                //Ignore this
                return;
          }

          for(unsigned int q=0; q < tBlob.convex_points.size(); q++)
          {
              tBlob.convex_points[q].x *= screenX/camWidth;
              tBlob.convex_points[q].y *= screenY/camHeight;
          }

          //Check collision
          for(unsigned int l=0; l < tBlob.convex_points.size(); l++)
          {
              //Loope through all convex points in the blob

              //Make sure we loop around (so last line is from last point to first)
              signed int n;
              if(l == tBlob.convex_points.size()-1)
              {
                  n = 0 - tBlob.convex_points.size() + 1;
              }
              else
              {
                  n = 1;
              }

              float blobx1, bloby1, blobx2, bloby2, fracTarVecX, fracTarVecY;
              blobx1 = tBlob.convex_points[l].x;
              bloby1 = tBlob.convex_points[l].y;
              blobx2 = tBlob.convex_points[l+n].x;
              bloby2 = tBlob.convex_points[l+n].y;
              fracTarVecX = FHolds[i].target_vec.x;
              fracTarVecY = FHolds[i].target_vec.y;


              //Vector for convex line
              float ux = blobx2 - blobx1;
              float uy = bloby2 - bloby1;


              //Vector to circle
              float rx,ry;
              rx = FHolds[i].location.x - blobx1;
              ry = FHolds[i].location.y - bloby1;

              //Normal vector
              float dp = ux * rx + uy*ry;
              float nx, ny;

              nx = rx - dp;
              ny = ry - dp;

              if(sqrt(pow(nx,2)+pow(ny,2))<=40)
              {
                  //COLL COLL MUTHAFUCKA
                  nx /= sqrt(pow(nx,2)+pow(ny,2));
                  ny /= sqrt(pow(nx,2)+pow(ny,2));
                  float dpvn, dpnn;
                  dpvn = fracTarVecX * nx + fracTarVecY * ny;
                  dpnn = nx*nx + ny*ny;

                  float vux, vuy;
                  vux = (dpvn/dpnn) * nx;
                  vuy = (dpvn/dpnn) * ny;

                  float vwx, vwy;
                  vwx = fracTarVecX - vux;
                  vwy = fracTarVecY - vuy;

                  float newvx, newvy;
                  newvx = vwx - vux;
                  newvy = vwy - vuy;

                  FHolds[i].target.set(FHolds[i].location.x+newvx, FHolds[i].location.y+newvy);
                  FHolds[j].target_vec.set(FHolds[i].target-FHolds[i].location);
              }


          }

        }
    }
    void Erosion(unsigned char ELS,unsigned char * input,unsigned char * output,int W,int H){

            //EROSION SIZE with fit
            for (int y = ELS/2 ; y < H-ELS/2 ; y++){
                for (int x =ELS/2 ; x < W-ELS/2 ; x++){

                    float sum = 0.0f;

                    for (char ky = -ELS/2 ; ky <= ELS/2 ; ky++){
                        for (char kx = -ELS/2 ; kx <= ELS/2 ; kx++){

                            sum += input[((y+ky)*W + (x+kx))]/255;

                        }
                    }
                    if(sum ==(ELS*ELS)){
                        output[(y*W + x)] = 255;
                    }
                    else{
                        output[(y*W + x)] = 0;

                    }
                }
            }
		}
    void Dilation(unsigned char DLS,unsigned char * input,unsigned char * output,int W,int H){

            for (int y = DLS/2 ; y < H-DLS/2 ; y++){
                for (int x =DLS/2 ; x < W-DLS/2 ; x++){

                    float sum = 0.0f;

                    for (char ky = -DLS/2 ; ky <= DLS/2 ; ky++){
                        for (char kx = -DLS/2 ; kx <= DLS/2 ; kx++){

                            sum += input[((y+ky)*W + (x+kx))]/255;

                        }
                    }
                    if(sum >=1){
                        output[(y*W + x)] = 255;
                    }
                    else{
                       output[(y*W + x)] = 0;
                    }
                }
            }
        }
    //---------------------------------------





};
