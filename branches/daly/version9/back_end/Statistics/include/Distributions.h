//----------------------------------------------------------------------------//
// class NormalDistribution: normal (Gaussian) distributed random numbers     //
//----------------------------------------------------------------------------//
class Distribution {
}

class NormalDistribution : public Distribution
{
  protected:
    double m, s, scale, cacheval;
    int cached;
  public:
    NormalDistribution (void) {}
    NormalDistribution (double mean, double stdev)
        { cached = 0; m = mean; s = stdev; scale  = 2.0 / max_val; }
    double operator () (void);
    double Mean (void) const { return m; }
    double Stdev (void) const { return s; }
};

