#include <functional>
#include <iostream>
#include <string>

// 1. The template class that accepts and stores the callback.
// It is templated on the callback's signature (return type and arguments).
template <typename ReturnType, typename... Args>
class TemplatedCallbackNotifier {
public:
  // Define the callback type using std::function
  using CallbackType = std::function<ReturnType(Args...)>;

  // Method to set/register the callback
  void setCallback(CallbackType callback_func) {
    callback_ = callback_func;
    is_callback_set_ = true;
  }

  // Method to trigger the callback
  void triggerCallback(Args... args) const {
    if (is_callback_set_) {
      std::cout << "Triggering callback: ";
      callback_(std::forward<Args>(args)...);
    } else {
      std::cout << "Callback not set." << std::endl;
    }
  }

private:
  CallbackType callback_;
  bool is_callback_set_ = false;
};

// 2. Example free function to be used as a callback
void freeFunctionCallback(const std::string &message) {
  std::cout << "Message from free function: " << message << std::endl;
}

// 3. Example class with a member function
class MyClass {
public:
  void memberFunctionCallback(const std::string &message) const {
    std::cout << "Message from member function: " << message << std::endl;
  }
};

int main() {
  // Instantiate the notifier for a specific signature:
  // void return type, const std::string& argument
  TemplatedCallbackNotifier<void, const std::string &> notifier;

  // --- Example 1: Using a free function as a callback ---
  notifier.setCallback(freeFunctionCallback);
  notifier.triggerCallback("Hello World!");

  // --- Example 2: Using a member function with a lambda ---
  MyClass obj;
  // Lambdas can capture objects/variables and call member functions,
  // making them a powerful glue for callbacks.
  auto lambda_callback = [&](const std::string &msg) {
    obj.memberFunctionCallback(msg);
  };

  notifier.setCallback(lambda_callback);
  notifier.triggerCallback("Hello again, via lambda!");

  // --- Example 3: Using an inline lambda ---
  notifier.setCallback([](const std::string &msg) {
    std::cout << "Message from inline lambda: " << msg << std::endl;
  });
  notifier.triggerCallback("One last time, inline!");

  return 0;
}
