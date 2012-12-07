#ifndef __INSTR_DATASTORE_H__
#define __INSTR_DATASTORE_H__

#include "datastore/allocationmonitor.h"
#include "datastore/observation.h"
#include "datastore/threadinfo.h"
#include "datastore/transaction.h"

#include "types.h"

#include <boost/shared_ptr.hpp>
#include <map>


using namespace std;

class DataStore{
public:
  typedef map<thread_id_t, ThreadInfo::ptr> thread_info_map_t;
  typedef map<thread_id_t, Observation::ptr> observation_buffer_t;
  typedef map<pthread_t, thread_id_t> thread_mappings_t;

  typedef AllocationMonitor<Observation, thread_id_t> observation_storage_t;
  typedef AllocationMonitor<Transaction, thread_id_t> transaction_storage_t;

  /* Threads */
  time_interval_t i_time;

                  /* Threads */
  thread_id_t thread_count;

  thread_mappings_t thread_mappings;
  thread_info_map_t thread_info;

  /* Storage */
  size_t max_storage_size;

  observation_storage_t observation_storage;
  transaction_storage_t transaction_storage;

  /* Observations */
  observation_buffer_t observation_buffer;


  DataStore(time_interval_t time,
            pthread_t pthread_id,
            size_t max_storage_size);

  //Thread Related
  void registerMainThread(time_interval_t time,
                          pthread_t t_id);

  void registerThread(time_interval_t time,
                      pthread_t t_id,
                      pthread_t launcher_id);

  void registerThreadEnd(time_interval_t time,
                         pthread_t t_id);

  //Transaction Related
  void registerTransaction(time_interval_t time,
                           pthread_t pthread_id,
                           transaction_gate_id_t tr_id,
                           bool explicit_end);

  void registerTransactionEnd(time_interval_t time,
                              pthread_t pthread_id,
                              transaction_gate_id_t tr_id);

  //Oracle Related
  void registerHealth(time_interval_t time,
                      pthread_t pthread_id,
                      oracle_id_t o_id,
                      float health,
                      float confidence);


  //Observation Related
  void registerObservation(time_interval_t time,
                           pthread_t t_id,
                           probe_id_t p_id);

  void commitObservation(pthread_t t_id);

  void discardObservation(pthread_t t_id);

  void readVariable(pthread_t t_id,
                    const void * ptr,
                    size_t size);


private:
  inline thread_id_t getThreadMapping(pthread_t pthread_id) const {
    //Maybe we shouldn't be that strict. Ignore unkown threads with a debug output.
    assert(thread_mappings.count(pthread_id) == 1);
    return thread_mappings.find(pthread_id)->second;
  }

  inline void updateStorageCapacity() {
    observation_storage.resize(max_storage_size - transaction_storage.getOccupancy());
    transaction_storage.resize(max_storage_size - observation_storage.getOccupancy());
  }

  inline size_t getStorageOccupancy() const {
    return observation_storage.getOccupancy() + transaction_storage.getOccupancy();
  }
};


#endif
