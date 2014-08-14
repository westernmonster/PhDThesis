#ifndef __VERBOSE_HOOK_H__55534f6145f92c9bf1fd001e9682291868dc8331__
#define __VERBOSE_HOOK_H__55534f6145f92c9bf1fd001e9682291868dc8331__

#include "benchmark_hook.h"

namespace diagnostic {
namespace benchmark {
class t_verbose_hook : public t_benchmark_hook {
protected:
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