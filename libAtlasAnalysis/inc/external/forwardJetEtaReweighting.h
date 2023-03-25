double getForwardJetEtaWeight(double eta) {


  double weight = 1;
  
  // calculate bin
  const int NBINS = 50;
  const double RANGE = 5;
  

  int ibin = int((eta + RANGE) * (double) NBINS/(2*RANGE));
  //  cout << ibin << endl;

double scale[NBINS] = {0.00,0.00,1.42,1.33,1.04,1.23,1.27,1.23,1.08,1.16,1.15,0.97,0.95,1.01,0.96,0.98,0.93,0.93,1.01,0.99,0.93,0.97,0.98,1.04,0.99,0.99,1.04,0.97,0.92,0.97,0.98,1.02,0.98,0.87,0.98,1.01,1.00,1.03,1.08,1.18,1.12,1.04,1.16,1.08,1.20,1.17,1.36,1.66,0.00,0.00};


  weight = scale[ibin];


  return weight;


}
