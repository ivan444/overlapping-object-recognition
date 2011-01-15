#ifndef math_laTnt_hpp
#define math_laTnt_hpp

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

namespace math{
namespace tnt{

  double luTNT(
    const boost::numeric::ublas::matrix<double>& A,  
    boost::numeric::ublas::matrix<double>& L,
    boost::numeric::ublas::matrix<double>& U);

  void svdTNT(
    const boost::numeric::ublas::matrix<double>& A,  
    boost::numeric::ublas::matrix<double>& U,
    boost::numeric::ublas::matrix<double>& S,
    boost::numeric::ublas::matrix<double>& V);

}
}

#endif
