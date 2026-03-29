#include <functional>
#include <iostream>
#include <string>

// The "Caller" class that accepts and stores a callback
class Caller {
public:
  // Define the type for the callback: a function taking a string and returning
  // void
  using CallbackFunction = std::function<void(const std::string &)>;

  // Method to set the callback
  void setCallback(CallbackFunction cb) { callback_ = cb; }

  // Method to trigger the callback
  void performTask() {
    std::cout << "Caller: Performing a task..." << std::endl;
    if (callback_) {
      // Invoke the stored callback function
      callback_("Task completed successfully!");
    }
  }

private:
  CallbackFunction callback_ =
      nullptr; // Stores the callback, initialized to nothing
};

// A "Client" class that will provide the callback logic
class Client {
public:
  void handleNotification(const std::string &message) {
    std::cout << "Client: Received notification - " << message << std::endl;
  }
};

int main() {
  Caller caller;
  Client client;

  // 1. Create a lambda function that calls the client's member function
  //    [&client] captures the 'client' object by reference
  //    (const std::string& msg) is the lambda's parameter list
  auto lambda_callback = [&client](const std::string &msg) {
    client.handleNotification(msg);
  };

  // 2. Pass the lambda (which is compatible with std::function) to the Caller
  caller.setCallback(lambda_callback);

  // 3. The Caller uses the callback when it performs its task
  caller.performTask();

  return 0;
}
