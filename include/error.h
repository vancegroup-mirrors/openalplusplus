#ifndef ERROR_H_INCLUDED_C416C52E
#define ERROR_H_INCLUDED_C416C52E

#include <iostream>
#include <string>

namespace openalpp {

/**
 * Error class for throwing.
 * The descendants of this class are different error types, and the exact
 * error can be displayed by using "cout << error;" where error is an instance
 * of Error (or one of its descendants)
 */
class Error {
 public:
  /**
   * Constructor.
   * Will use a default error message.
   */
  Error() : errorstring_("No error description") {}

  /**
   * Constructor.
   * @param description is error message to use.
   */
  Error(const char *description) : errorstring_(description) {}

  /**
   * Copy constructor.
   */
  Error(const Error &error) : errorstring_(error.errorstring_) {}

  /**
   * Function used for printing.
   * @param stream is stream to print to
   * @return the stream with the error message appended.
   */
  std::ostream &Put(std::ostream &stream) const;

 protected:
  /**
   * A description of the error
   */
  std::string errorstring_;
};

/**
 * Fatal error.
 * Caused by error in implementation, corrupted memory etc.
 */
class FatalError : public Error {
 public:
  /**
   * Constructor.
   * @param description is error message to use.
   */
  FatalError(const char *description) : Error(description) {}
};

/**
 * File error.
 * Caused by wrong file permissions, missing files etc.
 */
class FileError : public Error {
 public:
  /**
   * Constructor.
   * @param description is error message to use.
   */
  FileError(const char *description) : Error(description) {}
};

/**
 * Memory error.
 * Caused by insufficient memory etc.
 */
class MemoryError : public Error {
 public:
  /**
   * Constructor.
   * @param description is error message to use.
   */
  MemoryError(const char *description) : Error(description) {}
};

/**
 * Name error.
 * Caused by invalid (OpenAL) names.
 */
class NameError : public Error {
 public:
  /**
   * Constructor.
   * @param description is error message to use.
   */
  NameError(const char *description) : Error(description) {}
};

/**
 * Value error.
 * Caused by values out of range etc.
 */
class ValueError : public Error {
 public:
  /**
   * Constructor.
   * @param description is error message to use.
   */
  ValueError(const char *description) : Error(description) {}
};

/**
 * Init error.
 * Caused by trying to do actions without proper initialization.
 */
class InitError : public Error {
 public:
  /**
   * Constructor.
   * @param description is error message to use.
   */
  InitError(const char *description) : Error(description) {}
};

/**
 * Out stream operator.
 * Used to print error messages to a stream (i.e. "cerr << error;").
 * @param stream is the stream to print to.
 * @param error is the error to print.
 * @return the stream.
 */
std::ostream &operator<<(std::ostream &stream,const Error &error);

}

#endif /* ERROR_H_INCLUDED_C416C52E */
