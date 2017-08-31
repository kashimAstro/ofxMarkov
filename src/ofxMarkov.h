#include <math.h>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

class ofxMarkov 
{
	public:
        vector< vector<double> > matrix;
        vector<int> states;
        vector<double> times;

	~ofxMarkov(){ }
	ofxMarkov(){ }
	ofxMarkov(vector< vector<double> > initMatrix){ matrix = initMatrix; }
	static bool sortAsc(double i,double j){ return (i<j); }
	void setup(vector< vector<double> > initMatrix){ matrix = initMatrix; }

	/* poisson process */
	vector<double>poissP(double lambda, double T){
		random_device rd;
	  	mt19937 gen(rd());
	  	poisson_distribution<> P(lambda*T);
	  	int n = P(gen);
	  	vector<double> myVec(n);
	  	for (int i=0; i<n; i++){
	    		myVec[i] = generate_canonical<double,10>(gen)*T;
	  	}
	  	sort(myVec.begin(),myVec.end(),sortAsc);
	  	return myVec;
	}

	/* brownian motion */
	vector<double> brownian(double mu, double sigma, double T, int steps){
		double dt = T/steps;
	  	vector<double> myVec(steps+1);
	  	random_device rd;
		mt19937 gen(rd());
		normal_distribution<>norm(mu*dt,sigma*sqrt(dt));
		myVec[0] = 0;
		for(int i=0; i<steps; i++){
			myVec[i+1] = myVec[i] + norm(gen);
		}
		return myVec;
	}

	/* geometric brownian motion */
	vector<double> geoBrownian(double S0,double mu,double sigma, double T, int steps){
		vector<double> myVec = brownian(mu - (pow(sigma,2)/2),sigma,T,steps);
		for(int i=0; i<myVec.size();i++){
			myVec[i] = S0*exp(myVec[i]);
		}
		  return myVec;
	}

	/* discrete time markov chains */
	vector<int> DTMC (vector< vector<double> > trans, int steps, int start){
		random_device rd;
		mt19937 gen(rd());
		vector<int> myVec(steps+1);
		myVec[0] = start;
		int count;
		double sum,U;
		for(int i=0; i<steps; i++){
		    count = 0;
		    sum = 0;
		    U = generate_canonical<double,10>(gen);
		    while(sum < U){
		      sum += trans[myVec[i]][count];
		      if(sum > U){
			myVec[i+1] = count;
		      }
		      count++;
		    }
		}
		return myVec;
	}

	/* continuous time markov chains */
	vector<int> getStates(){
		return states;
	}

	vector<double> transTimes(){
		return times;
	}

	void simulate(double T, int state){
		random_device rd;
	  	mt19937 gen(rd());
	  	times.push_back(0);
	  	states.push_back(state);
	  	double t = 0;
		double lambda;
		double U;
		double sum;
		int j;
		vector< vector<double> > trans = matrix;
	  	while (t < T){
	    		lambda = 0;
			state = states.back();
			for (int i=0; i<trans[state].size();i++){
				lambda += trans[state][i];
	    		}
			exponential_distribution<> stepT(lambda);
			t += stepT(gen);
			if (t < T){
	      			j = 0;
	      			sum = 0;
		 	        U = generate_canonical<double,10>(gen);
				while(sum < U){
					sum += trans[state][j]/lambda;
					if (sum > U){
		  				times.push_back(t);
						states.push_back(j);
					}
					else{
		  				j++;
					}
	      			}
	    		}
	  	}
	}
};
