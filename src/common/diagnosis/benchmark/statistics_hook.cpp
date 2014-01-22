#include "statistics_hook.h"
#include "boost/date_time/posix_time/posix_time.hpp"

#include <fstream>

using namespace boost::posix_time;

namespace diagnosis {
namespace benchmark {
t_statistics_hook::t_statistics_hook (std::string d) : t_writer_hook(d) {}

void t_statistics_hook::_init (const structs::t_spectra & spectra,
                               const structs::t_candidate & correct) {
    std::ofstream f;


    // Print header on first iteration
    if (open_file("spectra.stats.csv", f, true))
        f << "date, system, iteration, component_count, transaction_count, suspicious_component_count, activation_rate, error_count, error_rate" << std::endl;

    f << "\"" << to_simple_string(second_clock::local_time()) << "\", ";
    f << get_system_count() << ", ";
    f << get_iteration_count() << ", ";
    f << spectra.get_component_count() << ", ";
    f << spectra.get_transaction_count() << ", ";
    f << spectra.get_suspicious_components_count() << ", ";
    f << spectra.get_activation_rate() << ", ";
    f << spectra.get_error_count() << ", ";
    f << spectra.get_error_rate() << std::endl;

    f.close();
}

void t_statistics_hook::_cleanup () {}

void t_statistics_hook::_pre_gen () {}

void t_statistics_hook::_post_gen (t_candidate_generator::t_ret_type & D,
                                   t_time_interval duration) {
    std::ofstream f;


    // Print header on first iteration
    if (open_file("D.stats.csv", f, true))
        f << "date, system, iteration, generator, duration" << std::endl;

    f << "\"" << to_simple_string(second_clock::local_time()) << "\", ";
    f << get_system_count() << ", ";
    f << get_iteration_count() << ", ";
    f << get_generator_name() << ", ";
    f << duration << std::endl;

    f.close();
}

void t_statistics_hook::_pre_rank () {}

void t_statistics_hook::_post_rank (const t_candidate_ranker::t_ret_type & probs,
                                    t_time_interval duration) {
    std::ofstream f;


    // Print header on first iteration
    if (open_file("probs.stats.csv", f, true))
        f << "date, system, iteration, generator, ranker, duration" << std::endl;

    f << "\"" << to_simple_string(second_clock::local_time()) << "\", ";
    f << get_system_count() << ", ";
    f << get_iteration_count() << ", ";
    f << get_generator_name() << ", ";
    f << get_ranker_name() << ", ";
    f << duration << std::endl;

    f.close();
}
}
}