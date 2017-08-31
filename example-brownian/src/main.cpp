#include "ofMain.h"
#include "ofxMarkov.h"

class ofApp : public ofBaseApp
{
	public:
	ofxMarkov *mm;
	double mu     = 0.8;
	double sigma  = 0.2;
	double S0     = 5;
	double T      = 10.5;
	int steps     = 250;
	ofImage img,gimg;

	void setup()
	{
		img.allocate(steps,steps,OF_IMAGE_COLOR);
		gimg.allocate(steps,steps,OF_IMAGE_COLOR);

		/* brownian motion */
		vector<double> B = mm->brownian(mu,sigma,T,steps);
		for(int i = 0; i < B.size(); i++)
		{
			for( int k = 0; k < steps; k++ )
			{
				img.setColor(i,k,ofColor(
					ofMap(B[i],0.f,9.f,0,255)
				));
			}
		}
		img.update();

		/* geometric brownian motion */
		vector<double> G = mm->geoBrownian(S0,mu,sigma,T,steps);
		for(int i = 0; i < G.size(); i++)
		{
			for( int k = 0; k < steps; k++ )
			{
				gimg.setColor(i,k,ofColor(
					ofMap(G[i],0.f,9.f,0,255)
				));
			}
		}
		gimg.update();

	}

	void draw()
	{
		ofSetWindowTitle(to_string(ofGetFrameRate()));
		img.draw(0,0);
		ofDrawBitmapStringHighlight("brownian motion",10,20);
		gimg.draw(img.getWidth(),0);
		ofDrawBitmapStringHighlight("geometric brownian",gimg.getWidth()+10,20);
	}
};

int main()
{
	ofSetupOpenGL(500,250, OF_WINDOW);
	ofRunApp( new ofApp());
}
