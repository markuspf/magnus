// class NormalDistribution						      //
//----------------------------------------------------------------------------//
//								       	      //
// Return normal (Gaussian) distributed random deviates			      //
// with mean "m" and standard deviation	"s" according to the density:	      //
//								       	      //
//                                           2			       	      //
//			1               (x-m)    			      //
//  p   (x) dx =  ------------  exp( - ------- ) dx   			      //
//   m,s          sqrt(2 pi) s          2 s*s				      //
//								       	      //
//----------------------------------------------------------------------------//

double NormalDistribution::operator () (void)
{
    // We don't have an extra deviate
    if  (cached == 0) {

	// Pick two uniform numbers in the square extending from -1 tp +1
	// in each direction and check if they are in the unit circle
	double v1, v2, r;
	do {
	    v1 = scale * gen->Long() - 1; // scale maps the random long to [0,2]
	    v2 = scale * gen->Long() - 1;
	    r = v1 * v1 + v2 * v2;
	} while (r >= 1.0);

	double f = sqrt( (-2 * log(r)) / r);

	// Make Box-Muller transformation to get two normal deviates.
	// Return one and save the other for the next time.
	cacheval = v1 * f;
	cached = 1;
	return (v2 * f * s + m);

    // We have an extra deviate, so unset the flag and return it
    } else {
	cached = 0;
	return (cacheval * s + m);
    }
}

//----------------------------------------------------------------------------//
