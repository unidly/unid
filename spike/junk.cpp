#include <iostream>
using namespace std;

class CircularQueue {
 private:
  int *arr;
  int front, rear, size;

 public:
  CircularQueue(int s) {
    size = s;
    arr = new int[size];
    front = rear = -1;
  }

  // Check if the queue is full
  bool isFull() { return (front == (rear + 1) % size); }

  // Check if the queue is empty
  bool isEmpty() { return (front == -1); }

  // Insert an element (Enqueue)
  void enqueue(int value) {
    if (isFull()) {
      cout << "Queue Overflow\n";
      return;
    }
    if (isEmpty()) front = 0;  // Initialize front on first element

    rear = (rear + 1) % size;  // Modular wrap-around logic
    arr[rear] = value;
    cout << "Inserted: " << value << endl;
  }

  // Remove an element (Dequeue)
  int dequeue() {
    if (isEmpty()) {
      cout << "Queue Underflow\n";
      return -1;
    }
    int data = arr[front];

    if (front == rear) {  // Only one element was present
      front = rear = -1;
    } else {
      front = (front + 1) % size;  // Move front forward circularly
    }
    return data;
  }

  void display() {
    if (isEmpty()) {
      cout << "Queue is empty\n";
      return;
    }
    int i = front;
    cout << "Queue elements: ";
    while (true) {
      cout << arr[i] << " ";
      if (i == rear) break;
      i = (i + 1) % size;
    }
    cout << endl;
  }
};

int main() {
  CircularQueue cq(5);
  cq.enqueue(10);
  cq.enqueue(20);
  cq.enqueue(30);
  cq.display();
  cout << "Dequeued: " << cq.dequeue() << endl;
  cq.display();
  return 0;
}
