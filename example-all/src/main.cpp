#include "ofMain.h"
#include "ofxMarkov.h"

class ofApp : public ofBaseApp
{
	public:
	ofxMarkov *mm;

	void setup()
	{
		double lambda = 6.2;
		double T      = 5;
		double mu     = 0.8;
		double sigma  = 0.2;
		double S0     = 5;
		double T1     = 8.5;
		int steps     = 500;
		int steps1    = 100;
		int start1    = 0;

		vector< vector<double> > matrix(3,vector<double> (3));
  		matrix[0][1] = 1;
		matrix[1][2] = 1;
		matrix[2][0] = 1;

		/* poisson process */
		vector<double> P = mm->poissP(lambda,T);
		for(int i = 0; i < P.size(); i++)
		{
			cout << "poiss: " << P[i] << endl;
		}

		/* brownian motion */
		vector<double> B = mm->brownian(mu,sigma,T,steps);
		for(int i = 0; i < B.size(); i++)
		{
			cout << "brownian: " << B[i] << endl;
		}

		/* geometric brownian motion */
		vector<double> G = mm->geoBrownian(S0,mu,sigma,T,steps);
		for(int i = 0; i < G.size(); i++)
		{
			cout << "g-brownian: " << G[i] << endl;
		}	

		/* discrete time markov chains */
  		vector<int> M = mm->DTMC(matrix,steps1,start1);
		for(int i = 0; i < M.size(); i++)
		{
			cout << "d-markov: "<< M[i] << endl;
		}

		/* continuous time markov chains */
  		mm = new ofxMarkov(matrix);
		mm->simulate(T1,start1);
		vector<double> time = mm->transTimes();
		vector<int> state   = mm->getStates();
		for(int i = 0; i < time.size(); i++)
			printf("c-markov: time[%f] state[%i]\n",time[i],state[i]);
		ofExit(0);
	}
	void update(){}
	void draw(){}
};

int main()
{
	ofSetupOpenGL(1024,768, OF_WINDOW);
	ofRunApp( new ofApp());
}
