//Kit Pollinger
//210 - Midterm 2


//210 - Midterm 2 starter code
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <fstream>
using namespace std;

const int MIN_NR = 10, MAX_NR = 99, MIN_LS = 5, MAX_LS = 20;

class DoublyLinkedList
{
private:
    struct Node
    {
        int data; // Index of the customer in the names vector
        bool isVIP;
        Node *prev;
        Node *next;
        Node(int val, bool vip = false, Node *p = nullptr, Node *n = nullptr)
        {
            data = val;
            isVIP = vip;
            prev = p;
            next = n;
        }
    };
    Node *head;
    Node *tail;

public:
    // Initializes an empty list
    DoublyLinkedList()
    {
        head = nullptr;
        tail = nullptr;
    }

    // Adds a customer to the end of the list
    void push_back(int v)
    {
        Node *newNode = new Node(v);
        if (!tail)
            head = tail = newNode;
        else
        {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    // Adds a customer to the front of the list (for VIPs)
    void push_front(int v, bool isVIP = false)
    {
        Node *newNode = new Node(v, isVIP);
        if (!head)
            head = tail = newNode;
        else
        {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
    }

    // Removes the customer at the front of the line
    void pop_front()
    {
        if (!head)
        {
            cout << "List is empty." << endl;
            return;
        }
        Node *temp = head;
        if (head->next)
        {
            head = head->next;
            head->prev = nullptr;
        }
        else
            head = tail = nullptr;
        delete temp;
    }

    // Removes the customer at the back of the line
    void pop_back()
    {
        if (!tail)
        {
            cout << "List is empty." << endl;
            return;
        }
        Node *temp = tail;
        if (tail->prev)
        {
            tail = tail->prev;
            tail->next = nullptr;
        }
        else
            head = tail = nullptr;
        delete temp;
    }

    // Removes the customer at a specific position
    void delete_pos(int pos)
    {
        if (!head)
        {
            cout << "List is empty." << endl;
            return;
        }
        if (pos == 1)
        {
            pop_front();
            return;
        }
        Node *temp = head;
        for (int i = 1; i < pos; i++)
        {
            if (!temp)
            {
                cout << "Position doesn't exist." << endl;
                return;
            }
            else
                temp = temp->next;
        }
        if (!temp)
        {
            cout << "Position doesn't exist." << endl;
            return;
        }
        if (!temp->next)
        {
            pop_back();
            return;
        }
        Node *tempPrev = temp->prev;
        tempPrev->next = temp->next;
        temp->next->prev = tempPrev;
        delete temp;
    }

    // Checks if the line is empty
    bool lineEmpty() const
    {
        return head == nullptr;
    }

    // Gets the customer at the back of the line
    int backLine() const
    {
        if (lineEmpty())
        {
            throw runtime_error("List is empty");
        }
        return tail->data;
    }

    // Gets the customer at the front of the line
    int frontLine() const
    {
        if (lineEmpty())
        {
            throw runtime_error("List is empty");
        }
        return head->data;
    }

    // Get customer at a specific position
    int get(int pos) const
    {
        if (pos < 1 || pos > size())
        {
            throw runtime_error("Invalid position: " + to_string(pos));
        }

        Node *current = head;
        for (int i = 1; i < pos; ++i)
        {
            current = current->next;
        }
        return current->data;
    }

    // Gets the size of the list
    int size() const
    {
        Node *current = head;
        int count = 0;
        while (current != nullptr)
        {
            ++count;
            current = current->next;
        }
        return count;
    }

    // Prints the list elements in forward order
    void print(const vector<string> &names) const
    {
        Node *current = head;
        if (!current)
        {
            cout << "List is empty." << endl;
            return;
        }

        while (current)
        {
            cout << names[current->data];
            if (current->isVIP)
                cout << " (VIP)";
            cout << " ";
            current = current->next;
        }
        cout << endl;
    }
};

int main()
{
    srand(time(nullptr)); // Seed random number generator

    // Read names from file
    vector<string> names;
    ifstream inFile("C:/Lab210/names.txt");
    if (!inFile.is_open())
    {
        cout << "Error opening names file." << endl;
        return 1;
    }

    string name;
    while (getline(inFile, name))
    {
        names.push_back(name);
    }
    inFile.close();

    DoublyLinkedList line;
    int currentTime = 1;
    int totalTime = 20;

    // Store opens with initial 5 customers
    cout << "Store opens:" << endl;
    for (int i = 0; i < 5; ++i)
    {
        int randomIndex = rand() % names.size();
        line.push_back(randomIndex); // Store index in the list
        cout << names[randomIndex] << " joined the line." << endl;
    }
    // Print the resulting line
    cout << "Resulting line: " << endl;
    line.print(names); // Print the line with customer names
    cout << endl;

    while (currentTime <= totalTime)
    {
        cout << "Time step #" << currentTime << endl;

        // Customer served at the front
        if (rand() % 100 + 1 <= 40 && !line.lineEmpty())
        {
            cout << names[line.frontLine()] << " is served." << endl;
            line.pop_front();
        }

        // New customer joins the end
        if (rand() % 100 + 1 <= 60)
        {
            int randomIndex = rand() % names.size();
            line.push_back(randomIndex); // Store index
            cout << names[randomIndex] << " joined the line." << endl;
        }

        // Customer at the end leaves
        if (rand() % 100 + 1 <= 20 && !line.lineEmpty())
        {
            cout << names[line.backLine()] << " (at the rear) left the line." << endl;
            line.pop_back();
        }

        // Random customer leaves
        for (int i = 1; i <= line.size(); ++i)
        {
            if (rand() % 100 + 1 <= 10)
            {
                cout << names[line.get(i)] << " left the line." << endl;
                line.delete_pos(i);
                break; // Only one customer can leave per time step
            }
        }

        // VIP customer joins the front
        if (rand() % 100 + 1 <= 10)
        {
            int randomIndex = rand() % names.size();
            line.push_front(randomIndex, true); // Store index and set VIP
            cout << names[randomIndex] << " (VIP) joined the front of the line." << endl;
        }

        // Print the current line
        cout << "Resulting line:" << endl;
        line.print(names);
        cout << endl;

        ++currentTime;
    }

    return 0;
}
