#pragma once

#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

#include "quill/detail/Config.h"

namespace quill::detail
{

/** forward declaration **/
class ThreadContextCollection;
class ThreadContext;
class RecordBase;

class BackendWorker
{
public:
  /**
   * Constructor
   */
  explicit BackendWorker(Config const& config, ThreadContextCollection& thread_context_collection);

  /**
   * Destructor
   */
  ~BackendWorker();

  /**
   * Returns the status of the backend worker thread
   * @return true when the worker is running, false otherwise
   */
  [[nodiscard]] bool is_running() const noexcept;

  /**
   * Starts the backend worker thread
   * @throws
   */
  void run();

  /**
   * Stops the backend worker thread
   */
  void stop() noexcept;

private:
  /**
   * Sets the cpu affinity of the backend thread based on the configured value
   * @throws if fails to set cpu affinity
   */
  void _set_cpu_affinity() const;

  /**
   * Backend worker thread main function
   */
  void _main_loop();

  /**
   * Logging thread exist function that flushes everything after stop() is called
   */
  void _exit();

  /**
   * Checks for records in all queues and processes the one with the minimum timestamp
   * @return true if one record was found and processed
   */
  [[nodiscard]] bool _process_record(std::vector<ThreadContext*> const& thread_contexts);

private:
  Config const& _config;
  ThreadContextCollection& _thread_context_collection;

  std::thread _backend_worker_thread; /** the backend thread that is writing the log to the sinks */
  std::once_flag _start_init_once_flag; /** flag to start the thread only once, in case start() is called multiple times */

  /** Config values **/
  std::chrono::nanoseconds _backend_thread_sleep_duration; /** backend_thread_sleep_duration from config **/
  uint16_t _backend_thread_cpu_affinity; /** _backend_thread_cpu_affinity from config **/

  std::atomic<bool> _is_running{false}; /** The spawned backend thread status */
};

} // namespace quill::detail