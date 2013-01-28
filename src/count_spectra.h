#ifndef __COUNT_SPECTRA_H__
#define __COUNT_SPECTRA_H__

#include "spectra.h"
#include "types.h"

#include <memory>

class t_count_spectra: public t_basic_spectra <t_count> {
  typedef std::unique_ptr<t_count[]> t_activity_ptr;
  t_activity_ptr activity;

  virtual void set_element_count(t_count component_count, 
                                 t_count transaction_count);
public:

  inline t_count_spectra() {
  }
  
  inline t_count_spectra(t_count component_count, 
                         t_count transaction_count){
    set_element_count(component_count, transaction_count);

  }

  virtual t_count get_count(t_component_id component,
                            t_transaction_id transaction) const;

  virtual const t_count & get_activity(t_component_id component,
                                       t_transaction_id transaction) const;

  void hit(t_component_id component,
           t_transaction_id transaction,
           t_count count = 1) const;
  
  virtual std::ostream & print(std::ostream & out, 
                               const t_spectra_filter * filter = NULL) const;
  virtual std::istream & read(std::istream & in);
};

#endif
