#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

using CallbackFunc = std::function<void(int)>;

class EventEmitter {
private:
  struct CallbackEntry {
    int id;
    CallbackFunc func;
  };
  std::vector<CallbackEntry> listeners;
  int next_id = 0;

public:
  // Safe Register: Returns an ID for unregistering
  int registerCallback(CallbackFunc cb) {
    if (!cb)
      return -1;
    int id = next_id++;
    listeners.push_back({id, cb});
    return id;
  }

  // Safe Unregister: Removes listener by ID
  void unregisterCallback(int id) {
    listeners.erase(
        std::remove_if(listeners.begin(), listeners.end(),
                       [id](const CallbackEntry &e) { return e.id == id; }),
        listeners.end());
  }

  // Safe Emit: Does nothing if no one is listening
  void emit(int value) {
    if (listeners.empty())
      return;
    for (const auto &entry : listeners) {
      entry.func(value);
    }
  }
};

/* How to use
int main() {
    EventEmitter emitter;

    // 1. Initial emit - no listeners, no crash
    emitter.emit(10);

    // 2. Register a listener
    int id1 = emitter.registerCallback([](int v) {
        std::cout << "Received: " << v << "\n";
    });

    emitter.emit(20); // Output: Received: 20

    // 3. Unregister
    emitter.unregisterCallback(id1);
    emitter.emit(30); // No output, no crash
}
*/
