/* 
 * File:   StringSplit.h
 * Author: arka
 *
 * Created on December 1, 2012, 10:40 AM
 */

#ifndef STRINGSPLIT_H
#define	STRINGSPLIT_H

#include <sstream>
#include <string>
#include <cstddef>
#include <iostream>
#include <sstream>
#include <fstream>

/********
 *   CODE TAKEN FROM http://www.cplusplus.com/faq/sequences/strings/split/
 */
struct split
{
  enum empties_t { empties_ok, no_empties };
};

template <typename Container>
Container& split(
  Container&                                 result,
  const typename Container::value_type&      s,
  typename Container::value_type::value_type delimiter
   )
{
  result.clear();
  std::istringstream ss( s );
  while (!ss.eof())
  {
    typename Container::value_type field;
    getline( ss, field, delimiter );
    if (field.empty()) continue;
    result.push_back( field );
  }
  return result;
}
/******  
 * CODE TAKEN FROM http://www.cplusplus.com/faq/sequences/strings/split/
 */

#endif	/* STRINGSPLIT_H */

