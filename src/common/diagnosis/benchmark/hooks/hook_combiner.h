#ifndef __HOOK_COMBINER_H__20b4fccd457d03a26bb7b24e73393845efee8c11__
#define __HOOK_COMBINER_H__20b4fccd457d03a26bb7b24e73393845efee8c11__

#include "diagnosis/benchmark/hooks/benchmark_hook.h"

namespace diagnosis {
namespace benchmark {
class t_hook_combiner : public t_benchmark_hook, public std::list<t_const_ptr<t_benchmark_hook> > {
public:
    virtual void trigger_event (t_collector & collector,
                                const t_status_iteration_init & status) const;

    virtual void trigger_event (t_collector & collector,
                                const t_status_post_gen & status) const;

    virtual void trigger_event (t_collector & collector,
                                const t_status_post_rank & status) const;
};
}
}

#endif