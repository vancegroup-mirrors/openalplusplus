#include "error.h"

namespace openalpp {

//##ModelId=3BDD35A001D5
ostream &Error::Put(std::ostream &stream) const {
  return (stream << errorstring_);
}

std::ostream &operator<<(std::ostream &stream,const Error &error) {
  return error.Put(stream);
}

}
