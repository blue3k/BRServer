/* -----------------------------------------------------------------------------
 * director.swg
 *
 * This file contains support for director classes so that PHP proxy
 * methods can be called from C++.
 * ----------------------------------------------------------------------------- */

#ifndef SWIG_DIRECTOR_PHP_HEADER_
#define SWIG_DIRECTOR_PHP_HEADER_

#include <string>
#include <exception>
#include <map>

namespace Swig {

  /* memory handler */
  struct GCItem {
    virtual ~GCItem() {
    }

    virtual int get_own() const {
      return 0;
    }
  };

  struct GCItem_var {
    GCItem_var(GCItem *item = 0) : _item(item) {
    }

    GCItem_var& operator=(GCItem *item) {
      GCItem *tmp = _item;
      _item = item;
      delete tmp;
      return *this;
    }

    ~GCItem_var() {
      delete _item;
    }

    GCItem * operator->() const {
      return _item;
    }

    private:
    GCItem *_item;
  };

  struct GCItem_Object : GCItem {
    GCItem_Object(int own) : _own(own) {
    }

    virtual ~GCItem_Object() {
    }

    int get_own() const {
      return _own;
    }

    private:
    int _own;
  };

  template <typename Type>
  struct GCItem_T : GCItem {
    GCItem_T(Type *ptr) : _ptr(ptr) {
    }

    virtual ~GCItem_T() {
      delete _ptr;
    }

    private:
    Type *_ptr;
  };

  class Director {
    protected:
      zval *swig_self;
      typedef std::map<void *, GCItem_var> swig_ownership_map;
      mutable swig_ownership_map swig_owner;
#ifdef ZTS
      // Store the ZTS context so it's available when C++ calls back to PHP.
      void *** swig_zts_ctx;
#endif
    public:
      Director(zval *self TSRMLS_DC) : swig_self(self) {
        TSRMLS_SET_CTX(swig_zts_ctx);
      }

      static bool swig_is_overridden_method(char *cname, char *lc_fname TSRMLS_DC) {
        zend_class_entry **ce;
        zend_function *mptr;

        if (zend_lookup_class(cname, strlen(cname), &ce TSRMLS_CC) != SUCCESS) {
          return false;
        }
        if (zend_hash_find(&(*ce)->function_table, lc_fname, strlen(lc_fname) + 1, (void **) &mptr) != SUCCESS) {
          return false;
        }
        // common.scope points to the declaring class
        return strcmp(mptr->common.scope->name, cname);
      }

      template <typename Type>
      void swig_acquire_ownership(Type *vptr) const {
        if (vptr) {
          swig_owner[vptr] = new GCItem_T<Type>(vptr);
        }
      }
  };

  /* base class for director exceptions */
  class DirectorException : public std::exception {
  protected:
    std::string swig_msg;
  public:
    DirectorException(int code, const char *hdr, const char *msg TSRMLS_DC) : swig_msg(hdr) {
      if (msg[0]) {
        swig_msg += " ";
        swig_msg += msg;
      }
      SWIG_ErrorCode() = code;
      SWIG_ErrorMsg() = swig_msg.c_str();
    }

    virtual ~DirectorException() throw() {
    }

    const char *what() const throw() {
      return swig_msg.c_str();
    }

    static void raise(int code, const char *hdr, const char *msg TSRMLS_DC) {
      throw DirectorException(code, hdr, msg TSRMLS_CC);
    }
  };

  /* attempt to call a pure virtual method via a director method */
  class DirectorPureVirtualException : public DirectorException {
  public:
    DirectorPureVirtualException(const char *msg TSRMLS_DC)
      : DirectorException(E_ERROR, "SWIG director pure virtual method called", msg TSRMLS_CC) {
    }

    static void raise(const char *msg TSRMLS_DC) {
      throw DirectorPureVirtualException(msg TSRMLS_CC);
    }
  };

  /* any php exception that occurs during a director method call */
  class DirectorMethodException : public DirectorException
  {
  public:
    DirectorMethodException(const char *msg TSRMLS_DC)
      : DirectorException(E_ERROR, "SWIG director method error", msg TSRMLS_CC) {
    }

    static void raise(const char *msg TSRMLS_DC) {
      throw DirectorMethodException(msg TSRMLS_CC);
    }
  };
}

// DirectorMethodException() is documented to be callable with no parameters
// so use a macro to insert TSRMLS_CC so any ZTS context gets passed.
#define DirectorMethodException() DirectorMethodException("" TSRMLS_CC)

#endif
