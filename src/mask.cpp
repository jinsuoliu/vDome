#include "mask.h"
extern float projWidth;
extern float projHeight;

/******************************************
 
 CONSTRUCTOR
 
 ********************************************/

Mask::Mask(){
    erase = false;
    
    mouseX = 0;
    mouseY = 0;
    
    width = 1024;
    height = 768;
    
    mouseDown = false;
    
    brushImage.setUseTexture(true);
    brushImage.setImageType(OF_IMAGE_COLOR_ALPHA);
    maskFboImage.bpp = 16;
    brushImage.loadImage("brushes/brush.png");
    brushWidth = brushImage.getWidth();
    brushHeight = brushImage.getHeight();
    brushScale = 1;
    brushOpacity = 50;
    
    brush = ofMesh::plane(brushWidth, brushHeight, 2, 2, OF_PRIMITIVE_TRIANGLES);
    
    lastFIndex = -1;
}

/******************************************
 
 INIT
 
 ********************************************/

void Mask::init(int i){
    pIndex = i;
}

/******************************************
 
 SETUP
 
 ********************************************/

void Mask::setup(){
    width = projWidth;
    height = projHeight;
    
    if (maskFbo.getWidth() != projWidth || maskFbo.getHeight() != projHeight)
        maskFbo.allocate(width, height, GL_RGBA32F_ARB);
    
    maskFbo.begin();
        ofClear(255,255,255,0);
    maskFbo.end();
    
    if (maskFboImage.isAllocated()) {
    
        ofDisableAlphaBlending();
        ofDisableNormalizedTexCoords();
        maskFbo.begin();
        ofSetColor(255, 255, 255, 255);
            maskFboImage.draw(0,0, 1024, 768);
        maskFbo.end();
        ofEnableNormalizedTexCoords();
        ofEnableAlphaBlending();
    }
}

/******************************************
 
 DRAW
 
 ********************************************/

void Mask::draw(){
    if (mouseDown) {
        if (erase)
            ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        else
            ofEnableBlendMode(OF_BLENDMODE_SCREEN);

        maskFbo.begin();
        brushImage.bind();
        ofSetColor(brushOpacity, brushOpacity, brushOpacity, brushOpacity);
        ofPushMatrix();
            ofTranslate(mouseX-(width*pIndex), mouseY);
            ofScale(brushScale, brushScale);
            brush.draw();
        ofPopMatrix();
        brushImage.unbind();
        maskFbo.end();
        
        ofDisableBlendMode();
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    }    
}

/******************************************
 
 MOUSE
 
 ********************************************/

void Mask::mousePressed(ofMouseEventArgs& mouseArgs){
    mouseDown = true;
    mouseX = mouseArgs.x;
    mouseY = mouseArgs.y;
    prestore();
}

void Mask::mouseDragged(ofMouseEventArgs& mouseArgs){
    mouseX = mouseArgs.x;
    mouseY = mouseArgs.y;
}

void Mask::mouseReleased(ofMouseEventArgs& mouseArgs){
    mouseDown = false;
}

/******************************************
 
 KEYBOARD
 
 ********************************************/

void Mask::keyPressed(int key){
    if (key == OF_KEY_CONTROL || key == OF_KEY_COMMAND) {
        erase = true;
    }
}

void Mask::keyReleased(int key){
    if (key == OF_KEY_CONTROL || key == OF_KEY_COMMAND) {
        erase = false;
    }
}

/******************************************
 
 SETTINGS
 
 ********************************************/

void Mask::load(){
    string filename;
    filename = "masks/mask-" + ofToString(pIndex) + ".png";
    read(filename);
}

void Mask::save(){
    string filename;
    filename = "masks/mask-" + ofToString(pIndex) + ".png";
    write(filename);
}

void Mask::write(string filename){
    maskFboPixels.clear();
    maskFbo.readToPixels(maskFboPixels);
    maskFboImage.setFromPixels(maskFboPixels);
    maskFboImage.saveImage(filename);
}

void Mask::read(string filename){
    maskFboImage.clear();
    if (maskFboImage.loadImage(filename))
        setup();
}

// prestore mask pixels on mouse down to record previous state
void Mask::prestore() {
    hPixels.clear();
    maskFbo.readToPixels(hPixels);
    hImage.setFromPixels(hPixels);
}

// store mask pixels on mouse up for smoother ui, called by Command
int Mask::store(int fIndex) {
    lastFIndex = fIndex;
    string filename;
    filename = "masks/tmp/mask-" + ofToString(pIndex) + "-" + ofToString(fIndex) + ".png";
    hImage.saveImage(filename);   
    //cout << "store: " << filename << endl;
    return fIndex;
}

void Mask::recall(int fIndex) {
    string filename;
    filename = "masks/tmp/mask-" + ofToString(pIndex) + "-" + ofToString(fIndex) + ".png";
    read(filename);
    //cout << "recall: " << filename << endl;
}

/******************************************
 
 RESET
 
 ********************************************/

void Mask::reset(){
    maskFbo.begin();
        ofClear(255,255,255,0);
    maskFbo.end();
}
