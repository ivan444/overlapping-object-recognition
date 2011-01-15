#include "math_tnt.hpp"

#include <tnt/tnt_array2d.h>
#include <tnt/tnt_array2d_utils.h>

#include <tnt/jama_svd.h>
#include <tnt/jama_lu.h>


//////////////////////////////////
// TNT helpers for math_la.h

typedef TNT::Array2D<double>  TNTMatrix;
typedef boost::numeric::ublas::matrix<double> UblasMatrix;
typedef boost::numeric::ublas::vector<double> UblasVector;

namespace{
  void ublas2tnt(
    const UblasMatrix& A,
    TNTMatrix& Atnt)
  {
    TNTMatrix Atntwork((int)A.size1(), (int)A.size2());
    
    for (int i=0; i<Atntwork.dim1(); ++i)
      for (int j=0; j<Atntwork.dim2(); ++j)
        Atntwork[i][j]=A(i,j);
    
    Atnt=Atntwork;
  }
  void tnt2ublas(
    const TNTMatrix& Atnt,
    UblasMatrix& A)
  {
    A.resize(Atnt.dim1(), Atnt.dim2());
    for (unsigned i=0; i<A.size1(); ++i)
      for (unsigned j=0; j<A.size2(); ++j)
        A(i,j)=Atnt[i][j];
  }
}


double math::tnt::luTNT(
  const UblasMatrix& A,  
  UblasMatrix& L, UblasMatrix& U)
{
  assert(A.size1()>=A.size2());

  TNTMatrix Atnt;
  ublas2tnt(A, Atnt);

  JAMA::LU<double> lu(Atnt);
  TNTMatrix Ltnt(lu.getL());
  TNTMatrix Utnt(lu.getU());

  tnt2ublas(Ltnt,L);
  tnt2ublas(Utnt,U);
  return lu.det();
}

void math::tnt::svdTNT(
  const UblasMatrix& A,  
  UblasMatrix& U, UblasMatrix& S, UblasMatrix& V)
{
  assert(A.size1()>=A.size2());

  TNTMatrix Atnt;
  ublas2tnt(A, Atnt);
  JAMA::SVD<double> svd(Atnt);

  TNTMatrix Utnt, Stnt, Vtnt;
  svd.getU(Utnt); 
  svd.getS(Stnt); 
  svd.getV(Vtnt);
  tnt2ublas(Utnt,U);
  tnt2ublas(Stnt,S);
  tnt2ublas(Vtnt,V);
}

