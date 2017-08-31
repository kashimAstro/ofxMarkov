#include "ofMain.h"
#include "ofxMarkov.h"

class Particle {
    public:
        ofVec2f pos, vec;
        double rot;
        deque<ofVec2f> hist;

        Particle() : vec(0, 0),pos(0,0),rot(0) {
            hist.resize(0);
        }

        void move(ofVec2f next){
            while(hist.size() > 500){
                hist.pop_front();
            }
            hist.push_back(pos);
            pos = next;
        }

        ~Particle(){
            hist.clear();
        }
};

class ofApp : public ofBaseApp
{
	public:
	ofxMarkov *mm;
	double lambda = 6.2;
	double T      = 500;

	vector<double> P;
	vector<Particle> particles;

	void setup()
	{
		/* poisson process */
		P = mm->poissP(lambda,T);
		particles.resize(P.size());
		for(int i =0 ; i < P.size(); i++ ) {
	            particles[i].pos = ofVec2f(ofGetWidth()/2,ofGetHeight()/2);
        	    particles[i].vec = ofVec2f(ofRandom(-5, 5), ofRandom(-5, 5));
        	    particles[i].rot = 0.05;
        	}
	}

	void draw()
	{
		ofSetWindowTitle(to_string(ofGetFrameRate()));
		ofBackgroundGradient(ofColor::grey,ofColor::black);
		for(int i = 0; i < particles.size(); i++)
		{
			ofSetColor(ofColor((int)P[i]%255,(int)P[i]%255,(int)P[i]%200));

			ofDrawEllipse(particles[i].pos.x, particles[i].pos.y,5,5);
			particles[i].vec.rotateRad((particles[i].rot * P[i]) * P[i]);
			particles[i].move((particles[i].pos + particles[i].vec));

		}
	}
};

int main()
{
	ofSetupOpenGL(500,500, OF_WINDOW);
	ofRunApp( new ofApp());
}
