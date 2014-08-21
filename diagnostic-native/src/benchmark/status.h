#ifndef __STATUS_H_002ec00416d0bb9fe059ff731dccb22e60e633a2__
#define __STATUS_H_002ec00416d0bb9fe059ff731dccb22e60e633a2__

#include "types.h"
#include "../diagnostic_system.h"
#include "../utils/time.h"


#define STATUS_KEY_ITERATION "iteration"
#define STATUS_KEY_GENERATOR "generator"
#define STATUS_KEY_RANKER "ranker"

namespace diagnostic {
namespace benchmark {
class t_status {
public:
    virtual void prepare_entry (t_entry & entry) const = 0;
    inline virtual ~t_status () {}
protected:
    inline t_status () {}
};

class t_status_iteration_init : public t_status {
public:
    t_status_iteration_init (t_id iteration_id,
                             t_time_interval start,
                             const t_const_ptr<t_spectrum> & spectrum,
                             const t_const_ptr<t_candidate> & correct);

    t_id get_iteration_id () const;

    t_time_interval get_iteration_start () const;

    const t_spectrum & get_spectrum () const;
    const t_candidate & get_correct () const;

    virtual void prepare_entry (t_entry & entry) const;
    inline virtual ~t_status_iteration_init () {}

private:
    t_id iteration_id;
    t_time_interval start;
    t_const_ptr<t_spectrum> spectrum;
    t_const_ptr<t_candidate> correct;
};

class t_status_post_gen : public t_status_iteration_init {
public:
    t_status_post_gen (const t_status_iteration_init & status,
                       std::string name,
                       t_time_interval start,
                       t_time_interval end,
                       const t_const_ptr<t_candidate_generator::t_ret_type> & candidates);

    virtual const std::string & get_gen_name () const;

    virtual t_time_interval get_gen_duration () const;
    virtual t_time_interval get_gen_start () const;
    virtual t_time_interval get_gen_end () const;

    virtual const t_candidate_generator::t_ret_type & get_candidates () const;

    virtual void prepare_entry (t_entry & entry) const;
    inline virtual ~t_status_post_gen () {}

private:
    std::string name;
    t_time_interval start;
    t_time_interval end;

    t_const_ptr<t_candidate_generator::t_ret_type> candidates;
};

class t_status_post_rank : public t_status_post_gen {
public:
    t_status_post_rank (const t_status_post_gen & status,
                        std::string name,
                        t_time_interval start,
                        t_time_interval end,
                        const t_const_ptr<t_candidate_ranker::t_ret_type> & probs);

    virtual const std::string & get_rank_name () const;

    virtual t_time_interval get_rank_duration () const;
    virtual t_time_interval get_rank_start () const;
    virtual t_time_interval get_rank_end () const;

    virtual const t_candidate_ranker::t_ret_type & get_probs () const;

    virtual void prepare_entry (t_entry & entry) const;
    inline virtual ~t_status_post_rank () {}

private:
    std::string name;
    t_time_interval start;
    t_time_interval end;

    t_const_ptr<t_candidate_ranker::t_ret_type> probs;
};
}
}
#endif