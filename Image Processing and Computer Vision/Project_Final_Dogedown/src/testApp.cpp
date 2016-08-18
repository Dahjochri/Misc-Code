#include "testApp.h"
#include <math.h>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>


//--------------------------------------------------------------
void testApp::setup(){
    srand(time(NULL));
	ofSetVerticalSync(true);
	//video

	camWidth = 320;
    camHeight = 240;

    vidGrabber.initGrabber(camWidth,camHeight);
    vidGrabber.setDesiredFrameRate(30);
    videoMirror = new unsigned char[camWidth*camHeight*3];
    //---------------------------------------
    //

	//
	//---------------------------------------
	//BS sub and blobs
    BGoutgray = new unsigned char[camWidth*camHeight];
	BGout = new unsigned char[camWidth*camHeight*3];;
    Erout = new unsigned char[camWidth*camHeight];
    dilout = new unsigned char[camWidth*camHeight];

    bgs.init(videoMirror,BGout,camHeight,camWidth,3,BGSUB_RUNNING_GAUSSIAN_AVG);
    blobber.init(dilout,camHeight,camWidth);

    //---------------------------------------
    //IDLE STATE SECTION
    //SETUP
    //----------
    //start amount of fractals
    MinF = 10;
    MaxF = 10;
    int AOF = ofRandom(MinF,MaxF);
    //----------
    //min and max for L + e
    MinS = 0;
    MaxS = 0.6;
    //random start spin speed
    sS = ofRandom(MinS,MaxS);
    //----------
    //min and max for L + e
    MinA = 0;
    MaxA = 360;
    //random angle
    A = ofRandom(MinA,MaxA);
    //----------
    //min and max for L + e
    MinLe = 50;
    MaxLe= 1000;
    //initital length of the sticks
    L = ofRandom(MinLe,MaxLe);
    //end length, the length of the smallest possible stick
    e = L*0.10;
    //begin color
    R = ofRandom(10,255);
    G = ofRandom(10,255);
    B = ofRandom(10,255);
     //----------
    //timers
    //type in time between the ticks seconds,in milliseconds
    SpinChange.totaltime        = 5000;
    SpinChange.sTart();
    AngleChange.totaltime       = 15000;
    AngleChange.sTart();
    LengthChange.totaltime      = 20000;
    LengthChange.sTart();
    ColorChange.totaltime       = 10000;
    ColorChange.sTart();
    //----------
    //---------------------------------------


    //the gui menu setup
    //set its up and connects it to the global control variables for the fractals(L,A,sS...etc)
	gui.setup(); // most of the time you don't need a name
	gui.add(BG.setup("Clear", false));
	gui.add(Length.setup( "Length", L, 50, 1000 ));
	gui.add(middelAngle.setup( "middelAngle", A, 0, 360 ));
	gui.add(spinSpeed.setup( "spinSPeed", sS, 0, 0.6 ));
	gui.add(newColor.setup("New Color"));
	gui.add(endSize.setup( "endSize", e, 5, 100 ));
	bHide = true;


    //---------------------------------------
    //turns off the auto background display
	ofSetBackgroundAuto(BG);
	//sets base background to black
    ofBackground(0,0,0);


    //


    //---------------------------------------
    //creates the initial fractals in the scene
    for(unsigned char i =0;i<AOF;i++){
        FracHolder bob;
        bob.location.set(screenX/2,screenY/2);
        bob.velocity.set(0,0);
        bob.target.set(rand()%screenX,rand()%screenY);
        bob.target_vec.set(bob.target-bob.location);
        //FHolds.push_back(bob);
        FHolds.insert(FHolds.begin()+i,bob);

    }
    //--------------------------------------
    //some debugs
    /*cout<<FHolds[FHolds.size()-1].red<<endl;
    cout<<FHolds[FHolds.size()-2].red<<endl;
    cout<<FHolds[FHolds.size()-3].red<<endl;
    cout<<FHolds[FHolds.size()-4].red<<endl;*/

    //---------------------------------------
    //sets the first to dark, meaning that the next will be light, then dark, then light etc..
    //can be set to false with no trouble
    FHolds[FHolds.size()-1].dark = false;
    //---------------------------------------
    //allocates memory for the fbo
    fbo.allocate(screenX,screenY,GL_RGB);
}


//--------------------------------------------------------------
void testApp::update(){
    //ofBackground(0,0,0);
    //ofSetBackgroundAuto(BG);
    vidGrabber.update();

    fbo.begin();
    ofSetLineWidth(2);
    FractalDisplay_RACG(R,G,B,30);
    ofSetLineWidth(1);
    for(unsigned char i = 0; i<FHolds.size();i++){
        //fractal physics
        FHolds[i].movement();
        autorotate(i);
        //idle to active change
        if(blobs.size()>0){
            Length = 50;
            endSize = Length*0.20;
            edgeWrap(i);
        }else{
            edgeBounce(i);
        }

        collision(i);
        PolyCollision(i);
        //debug target line

        /*ofSetColor(255,255,0);
        ofLine(FHolds[i].location.x,FHolds[i].location.y,FHolds[i].target.x,FHolds[i].target.y);*/
    }
    ofSetColor(255,255,255);
    mirrorTexture.draw(0, screenY-camHeight, camWidth, camHeight);
    fbo.end();

    if (vidGrabber.isFrameNew()){


        //---------------------------------------
        //logger

        //---------------------------------------

        //the fbo
        fbo.begin();
        //---------------------------------------
        //fractal part of the fbo

        //updates the fractal color and position


        //---------------------------------------
        //video part of the fbo
        //displayes the mirrored image in the lower left corner
        //the video
        ofSetColor(255,255,255);
        //the frame.. for now
        //---------------------------------------
        IdleChange();
        //---------------------------------------

        DoTheFlip(*&videoMirror,camWidth,camHeight);
        mirrorTexture.loadData(videoMirror, camWidth, camHeight, GL_RGB);

        //---------------------------------------
        bgs.updatePointer(videoMirror);
        bgs.update();
        //---------------------------------------
        for(unsigned int y=0;y<camHeight;y++){
            for(unsigned int x=0; x<camWidth;x++){
                unsigned int i = y*camWidth+x;
                BGoutgray[i] = BGout[i*3+0];
            }
        }
         //---------------------------------------
        //ero and dila
        Erosion(3,BGoutgray,Erout,camWidth, camHeight);
        Dilation(3,Erout,dilout,camWidth, camHeight);
        diloutexture.loadData(dilout, camWidth, camHeight, GL_LUMINANCE);
         //---------------------------------------
        //

        ofSetColor(255,255,255);
        diloutexture.draw(camWidth, screenY-camHeight, camWidth, camHeight);


        if(beginBlob)
        {
            blobber.updatePointer(dilout);
            blobber.detectBlobs();
        }
        blobs = blobber.getBlobs();
         //---------------------------------------


        fbo.end();

    }

	//---------------------------------------
	//logger
	/*Save1.setFromPixels(videoMirror,camWidth,camHeight,OF_IMAGE_COLOR);
	fbo.readToPixels(Pixs);
	Save2.setFromPixels(Pixs.getPixels(),screenX,screenY,OF_IMAGE_COLOR);
	Save2.resize(screenX/2,screenY/2);


	if(framesRun==0)
        logger.update();
    framesRun = (framesRun+1) % 5;*/

}

//--------------------------------------------------------------
void testApp::draw(){
    //draws the fbo
    fbo.draw(0,0);


    //draws the gui unless "h" is toggled
    if( bHide ){
		gui.draw();
	}


    for(unsigned int i=0; i < blobs.size(); i++){
        ofSetColor(0,0,255);
        for(unsigned int j=0; j < blobs[i].convex_points.size(); j++){
            int n=1;
            if(j == blobs[i].convex_points.size()-1){
                //To #0
                //and end! break;
                n = 0 - blobs[i].convex_points.size() + 1;
            }
            ofLine(blobs[i].convex_points[j].x * (screenX/camWidth), blobs[i].convex_points[j].y * (screenY/camHeight), blobs[i].convex_points[j+n].x * (screenX/camWidth), blobs[i].convex_points[j+n].y * (screenY/camHeight));
            //ofCircle(20+blobs[i].convex_points[j].x, 20+blobs[i].convex_points[j].y, 1);
            //ofCircle(20+camWidth+blobs[i].convex_points[j].x, 20+camHeight+blobs[i].convex_points[j].y, 1);
        }
        ofSetColor(255,255,255);
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    //hides the gui on toggle
	if( key == 'h' ){
		bHide = !bHide;
	}
	 //---------------------------------------
	 //done callibration
    if(key == ' ')
    {
    if(beginBlob)
        {
            bgs.reCalibrate();
            beginBlob = false;
            cout << "Calibrating" << endl;
        }
        {
            bgs.fgaDoneCalibrating();
            beginBlob = true;
            cout << "Not calibrating" << endl;
        }
    }
    if(beginBlob)
        {
            blobber.updatePointer(dilout);
            blobber.detectBlobs();
        }


}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){



}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    //debug color change on right click
    if(button==2){
        R = ofRandom(10,255);
        G = ofRandom(10,255);
        B = ofRandom(10,255);
        //cout<<"cut"<<endl;
        //resets the possibility to change color as well as giving all the other fractals new random base vaules
        for (int i= FHolds.size()-2;i>=0;i--) {
            FHolds[i].colordone = false;
            FHolds[i].red = ofRandom(10,255);
            FHolds[i].green = ofRandom(10,255);
            FHolds[i].blue = ofRandom(10,255);
        }

    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}
