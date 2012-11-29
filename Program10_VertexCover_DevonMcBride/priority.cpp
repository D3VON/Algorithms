//priority_queue05.cpp
//Based on an example of Herbert Schildt.
#include <iostream>
#include <queue>
#include <string>
using namespace std;
//A simple "event organizer" class
class Event
{
public:
    Event()
    {
        name = "";
        priority = 0;
    }
    Event(string name, int priority)
    {
        this->name = name;
        this->priority = priority;
    }
    string getEventName()  const { return name;     }
    int getEventPriority() const { return priority; }
    
private:   
    int priority;
    string name;
};
//Overload operator< to determine priority:
bool operator<(const Event &e1, const Event &e2)
{
    return e1.getEventPriority() < e2.getEventPriority();
}

int main()
{
    cout << "\nThis program illustrates a simple priority queue ";
    cout << "of class objects,\nin which the priority has been  ";
    cout << "defined by overloading \"operator<\".";
    cout << "\nPress Enter to continue ... ";  cin.ignore(80, '\n');
    priority_queue<Event> pq;
    pq.push(Event("Phone rings", 3));
    pq.push(Event("Fire!", 10));
    pq.push(Event("Mail arrives", 2));
    pq.push(Event("Knock is heard on door", 4));
    //Display the list of events, with their priorities
    cout << "\nPriorities:\n\n";
    while(!pq.empty() )
    {
        cout << pq.top().getEventName() << "\n";
        pq.pop();
    }
    cout << "\nPress Enter to continue ... ";  cin.ignore(80, '\n');
}

