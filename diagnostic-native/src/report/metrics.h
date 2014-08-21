#ifndef __METRICS_H_d89493d817d7ef64219d0d614f7ef3b07eb21297__
#define __METRICS_H_d89493d817d7ef64219d0d614f7ef3b07eb21297__

#include "ranking.h"
#include "../diagnostic_system.h"
#include "../candidate.h"
#include "../utils/boost.h"
#include "../utils/iostream.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>

namespace diagnostic {
namespace benchmark {
class t_metric {
public:
    typedef std::map<std::string, std::string> t_arguments;

    virtual const std::string & key () const = 0;

    virtual std::string operator () (const t_spectrum & spectrum,
                                     const t_candidate & correct,
                                     const t_candidate_generator::t_ret_type & D,
                                     const t_candidate_ranker::t_ret_type & probs,
                                     const t_ranking & r,
                                     const t_arguments & arguments) const = 0;
    template <class T>
    T get_argument (std::string name, const t_arguments & arguments) const {
        t_arguments::const_iterator it = arguments.find(name);


        assert(it != arguments.end());

        return boost::lexical_cast<T> (it->second);
    }

    inline virtual ~t_metric () {}
};

class t_Cd : public t_metric {
public:
    virtual const std::string & key () const;

    virtual std::string operator () (const t_spectrum & spectrum,
                                     const t_candidate & correct,
                                     const t_candidate_generator::t_ret_type & D,
                                     const t_candidate_ranker::t_ret_type & probs,
                                     const t_ranking & r,
                                     const t_arguments & ret) const;
private:
    static std::string __KEY__;
};

class t_wasted_effort : public t_metric {
public:
    virtual const std::string & key () const;

    virtual std::string operator () (const t_spectrum & spectrum,
                                     const t_candidate & correct,
                                     const t_candidate_generator::t_ret_type & D,
                                     const t_candidate_ranker::t_ret_type & probs,
                                     const t_ranking & r,
                                     const t_arguments & ret) const;
private:
    static std::string __KEY__;
};

class t_entropy : public t_metric {
public:
    virtual const std::string & key () const;

    virtual std::string operator () (const t_spectrum & spectrum,
                                     const t_candidate & correct,
                                     const t_candidate_generator::t_ret_type & D,
                                     const t_candidate_ranker::t_ret_type & probs,
                                     const t_ranking & r,
                                     const t_arguments & ret) const;
private:
    static std::string __KEY__;
};

class t_quality : public t_metric {
public:
    t_quality (std::string target_metric);

    virtual const std::string & key () const;

    virtual std::string operator () (const t_spectrum & spectrum,
                                     const t_candidate & correct,
                                     const t_candidate_generator::t_ret_type & D,
                                     const t_candidate_ranker::t_ret_type & probs,
                                     const t_ranking & r,
                                     const t_arguments & ret) const;
private:
    std::string target_metric;
    std::string result_metric;
    static std::string __KEY__;
};

class t_quality_fair : public t_metric {
public:
    t_quality_fair (std::string target_metric);

    virtual const std::string & key () const;

    virtual std::string operator () (const t_spectrum & spectrum,
                                     const t_candidate & correct,
                                     const t_candidate_generator::t_ret_type & D,
                                     const t_candidate_ranker::t_ret_type & probs,
                                     const t_ranking & r,
                                     const t_arguments & ret) const;
private:
    std::string target_metric;
    std::string result_metric;
    static std::string __KEY__;
};
}
}


#endif