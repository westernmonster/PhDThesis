#ifndef __CONFIGURE_MHS_H__
#define __CONFIGURE_MHS_H__

#include "diagnosis/algorithms/barinel.h"
#include "diagnosis/algorithms/mhs.h"

#include "utils/opt.h"
#include "types.h"

using namespace diagnosis;
using namespace diagnosis::heuristics;

class t_diag_options : public t_options {
public:
    t_diag_options (std::string app_name);
    virtual bool short_opt (int c, char * param);
    virtual std::ostream & print (std::ostream & out) const;

public:
    bool has_confidence;
    bool print_spectra;

    t_count fork_level;
    t_count stride;
    t_count threads;

    bool do_ranking;
    bool do_generation;

    bool ambiguity_groups;

    algorithms::t_mhs mhs;
    algorithms::t_barinel barinel;
};

#endif