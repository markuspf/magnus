// Contents: Computer Project 14
//           Queueing system
//
// Principal Authors: Dmitry Bormotov
//


#include "global.h"
#include "Random.h"

void main( )
{
  UniformRandom r(23152);
  const int numOfCustomers = 20;
  double lambda[numOfCustomers], t[numOfCustomers];
  double x[numOfCustomers], u[numOfCustomers];
  double service[numOfCustomers], departure[numOfCustomers];
  int bound = 21;

  // generate t_n

  double L = 3;
  int count = 0;
  while( count < numOfCustomers ) {
    double time = 1 + r.rand() * (bound-1);
    if( bound * r.rand() <= L / pow(time,4) ) {
      //cout << time << "  " << L * exp(-L * time) << endl;
      t[count++] = time;;
    }
  }

  
  double mean = 0;
  for( int i = 0; i < numOfCustomers; ++i )
    mean += t[i];
  mean /= numOfCustomers;
  cout << endl << "mean = " << mean << endl;
  

  // generate x_n

  L = 4.0 / 3;
  count = 0;
  while( count < numOfCustomers ) {
    double xx = r.rand() * bound;
    if( bound * r.rand() <= L * exp(-L * xx) ) {
      //cout << xx << "  " << L * exp(-L * xx) << endl;
      x[count++] = xx;;
    }
  }

  
  mean = 0;
  for( int i = 0; i < numOfCustomers; ++i )
    mean += x[i];
  mean /= numOfCustomers;
  cout << endl << "mean = " << mean << endl;
  

  // compute lambda_n

  lambda[0] = t[0];
  for( int i = 1; i < numOfCustomers; ++i )
    lambda[i] = t[i] + lambda[i-1];

  // compute u_n

  u[numOfCustomers-1] = 0;
  for( int i = 0; i < numOfCustomers-1; ++i )
    u[i] = x[i] - t[i+1];

  // compute service time and departure time

  double time = lambda[0];
  int customer = 0;

  while( true ) {
    service[customer] = time;
    time += x[customer];
    departure[customer] = time;
    ++customer;
    if( customer >= numOfCustomers ) break;
    if( lambda[customer] > time )
      time = lambda[customer];
  }

  // compute w_n and s_n
  
  double w[numOfCustomers], s[numOfCustomers];
  for( int i = 0; i < numOfCustomers; ++i ) {
    w[i] = service[i] - lambda[i];
    s[i] = departure[i] - lambda[i];
  }

  // compute v_n
 
  double v[numOfCustomers];
  for( int i = 0; i < numOfCustomers; ++i ) {
    v[i] = 0;
    for( int j = 0; j < numOfCustomers; ++j ) 
      if( lambda[j] > service[i] && lambda[j] < departure[i] )
	++v[i];
  }

  // compute g_n
    
  double g[numOfCustomers];
  for( int i = 0; i < numOfCustomers; ++i ) {
    g[i] = 0;
    for( int j = 0; j < numOfCustomers; ++j ) 
      if( lambda[j] < departure[i] && departure[j] > departure[i] )
	++g[i];
  }

  // compute g'_n
    
  double g1[numOfCustomers];
  for( int i = 0; i < numOfCustomers; ++i ) {
    g1[i] = 0;
    for( int j = 0; j < numOfCustomers; ++j ) 
      if( lambda[j] < lambda[i] && departure[j] > lambda[i] )
	++g1[i];
  }

  // compute v'_n+1

  double v1[numOfCustomers];
  v1[0] = 0;
  for( int i = 1; i < numOfCustomers; ++i ) {
    v1[i] = 0;
    for( int j = 0; j < numOfCustomers; ++j ) 
      if( departure[j] > lambda[i-1] && departure[j] < lambda[i] )
	++v1[i];
  }
    
  // print results

  for( int i = 0; i < numOfCustomers; ++i ) {
    cout << "n = " << i+1;
    if( i < 9 ) cout << " ";
    cout << " lambda_n = " << lambda[i] << "  t_n = " << t[i]
	 << "  x_n = " << x[i] << "  u_n = " << u[i] << endl
	 << "       w_n = " << w[i] 
	 << "  s_n = " << s[i] << "  v_n = " << v[i] << "  g_n = " << g[i]
	 << "  g'_n = " << g1[i] << "  v'_n+1 = " << v1[i]
      // << "  service = " << service[i] << "  departure = " << departure[i]
	 << endl << endl << endl; 
  }
}
